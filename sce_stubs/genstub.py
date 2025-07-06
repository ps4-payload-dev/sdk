#!/usr/bin/env python3
# encoding: utf-8
# Copyright (C) 2025 John Törnblom
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING. If not see
# <http://www.gnu.org/licenses/>.

import argparse
import os
import string
import struct

from elftools.elf.elffile import ELFFile


tmpl_func = string.Template('''
asm(".global $name\\n"
    ".type $name @function\\n"
    "$name:\\n");''')


tmpl_obj = string.Template('''
asm(".global $name\\n"
    ".type   $name, @object\\n"
    "$name:\\n");''')


# read NIDs from aerolib.csv
NID_DB = (os.path.dirname(__file__) or '.') + '/aerolib.csv'
nid_map = {}
with open(NID_DB) as f:
    for line in f.readlines():
        nid, sym = line.strip().split(' ', 1)
        nid_map[nid] = sym


def symbols(sym_type, filename):
    '''
    yield symbol names in dynamic segments using the NID lookup table
    'aerolib.csv'. Based on:
    https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/blob/master/scripts/symbol_entries.py
    '''
    with open(filename, 'rb') as f:
        elf = ELFFile(f)

        dynamic = None
        dynlibdata = None
        for segment in elf.iter_segments():
            if segment.header.p_type == 'PT_DYNAMIC':
                dynamic = segment.header

            elif segment.header.p_type == 0x61000000: # PT_SCE_DYNLIBDATA
                dynlibdata = segment.header

        if not dynamic:
            return

        dyncnt = int(dynamic.p_filesz / struct.calcsize('<QQ'))
        symtab = strtab = dynlibdata.p_offset
        symcnt = 0

        for i in range(dyncnt):
            f.seek(dynamic.p_offset + (i * struct.calcsize('<QQ')))
            d_tag, d_val = struct.unpack('<QQ', f.read(struct.calcsize('<QQ')))

            if d_tag == 0x61000039: # DT_SCE_SYMTAB
                symtab += d_val

            if d_tag == 0x61000035: # DT_SCE_STRTAB
                strtab += d_val

            if d_tag == 0x6100003F: # DT_SCE_SYMTABSZ
                symcnt = int(d_val / struct.calcsize('<IBBHQQ'))

        for i in range(symcnt):
            f.seek(symtab + (i * struct.calcsize('<IBBHQQ')))
            st_name, st_info, st_other, st_shndx, st_value, st_size = \
                struct.unpack('<IBBHQQ', f.read(struct.calcsize('<IBBHQQ')))

            st_bind = st_info >> 4
            st_type = st_info & 0xF

            if st_shndx == 0: # SHN_UNDEF
                continue

            if st_bind == 0: # STB_LOCAL
                continue

            if sym_type == 'STT_OBJECT' and st_type != 1:
                continue

            if sym_type == 'STT_FUNC' and st_type != 2:
                continue

            f.seek(strtab + st_name)
            symname = f.read(14).decode('ascii')
            nid, lid, mid = symname.split('#')

            if nid in nid_map:
                yield nid_map[nid]


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('SPRX_FILE')
    cli_args = parser.parse_args()

    funcs = sorted(set(symbols('STT_FUNC', cli_args.SPRX_FILE)))
    gvars = sorted(set(symbols('STT_OBJECT', cli_args.SPRX_FILE)))

    for name in sorted(funcs):
        print(tmpl_func.substitute(name=name))

    for name in sorted(gvars):
        print(tmpl_obj.substitute(name=name))
