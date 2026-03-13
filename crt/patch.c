/* Copyright (C) 2026 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

#include "kernel.h"
#include "patch.h"


/**
 * Patch kernel so payloads can mmap SELF files for decryption.
 **/
static int
patch_mmap(void) {
  unsigned long addr;
  int err = 0;

  // pattern confirmed working on fw 9.00
  if((addr=kernel_find_pattern(KERNEL_ADDRESS_IMAGE_BASE, KERNEL_IMAGE_SIZE,
			       "554889e5f647??04740bf646??08b801000000750231c05dc3"))) {
    if(kernel_patch(addr, 0, "\xb8\x01\x00\x00\x00\xc3", 6)) {
      err = -1;
    }
  }

  if((addr=kernel_find_pattern(KERNEL_ADDRESS_IMAGE_BASE, KERNEL_IMAGE_SIZE,
			       "4883c6??8b78??e8????????"))) {
    if(kernel_patch(addr+7, 0, "\x31\xc0\xeb\x01", 4)) {
      err = -1;
    }
  }

  return err;
}


int
__patch_init(void) {
  return patch_mmap();
}

