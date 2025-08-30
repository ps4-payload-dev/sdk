/* Copyright (C) 2025 John Törnblom

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

#include <elf.h>
#include <stdio.h>
#include <unistd.h>

#include <ps4/kernel.h>


int
main() {
  uint8_t buf[0x4000];
  size_t len;

  for(size_t i=0; i<KERNEL_IMAGE_SIZE; i+=sizeof(buf)) {
    len = KERNEL_IMAGE_SIZE - i;
    if(len > sizeof(buf)) {
      len = sizeof(buf);
    }

    if(kernel_copyout(KERNEL_ADDRESS_IMAGE_BASE+i, buf, len)) {
      perror("kernel_copyout");
      return -1;
    }

    if(write(STDOUT_FILENO, buf, len) != len) {
      perror("write");
      return -1;
    }
  }

  return 0;
}

