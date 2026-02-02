#   Copyright (C) 2025 John Törnblom
#
# This file is free software; you can redistribute it and/or modify it
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

DESTDIR ?= /opt/ps4-payload-sdk

TOOLCHAINS := toolchain/orbis.sh \
              toolchain/orbis.mk \
              toolchain/orbis.cmake \
              toolchain/orbis.ini

TOOLS := bin/orbis-cmake \
         bin/orbis-llvm-wrapper \
         bin/orbis-meson \
         bin/orbis-llvm-config \
         bin/orbis-clang \
         bin/orbis-clang++ \
         bin/orbis-deploy \
         bin/orbis-ld \
         bin/orbis-pkg-config \
         bin/orbis-nid \
         bin/orbis-cross-fix-root

LDSCRIPTS := elf_x86_64.x

.PHONY: all clean install

all: bin/orbis-nid

clean:
	rm -f bin/orbis-nid

bin/orbis-nid: bin/orbis-nid.c
	$(CC) -o $@ $<

install: install-tools install-symlinks install-ldscripts install-toolchains

install-ldscripts: $(LDSCRIPTS)
	install -d $(DESTDIR)/ldscripts
	install $^ $(DESTDIR)/ldscripts
	install -d $(DESTDIR)/target/lib
	ln -sf ../../ldscripts/elf_x86_64.x $(DESTDIR)/target/lib/main.script
	ln -sf ../../ldscripts/elf_x86_64.x $(DESTDIR)/target/lib/prx.script

install-toolchains: $(TOOLCHAINS)
	install -d $(DESTDIR)/toolchain
	install $^ $(DESTDIR)/toolchain

install-tools: $(TOOLS)
	install -d $(DESTDIR)/bin
	install $^ $(DESTDIR)/bin

install-symlinks:
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/llvm-ar
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/llvm-nm
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/llvm-objcopy
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/llvm-ranlib
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/llvm-strip
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/clang
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/clang++
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/clang-cpp
	ln -sf orbis-llvm-wrapper $(DESTDIR)/bin/ld.lld
	ln -sf llvm-ar $(DESTDIR)/bin/orbis-ar
	ln -sf llvm-nm $(DESTDIR)/bin/orbis-nm
	ln -sf llvm-objcopy $(DESTDIR)/bin/orbis-objcopy
	ln -sf llvm-ranlib $(DESTDIR)/bin/orbis-ranlib
	ln -sf llvm-strip $(DESTDIR)/bin/orbis-strip
	ln -sf orbis-clang $(DESTDIR)/bin/orbis-as
