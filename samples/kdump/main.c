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


static ssize_t
kernel_get_image_size(void) {
  size_t min_vaddr = -1;
  size_t max_vaddr = 0;
  Elf64_Ehdr ehdr;
  Elf64_Phdr phdr;

  // copy out ELF header from kernel
  if(kernel_copyout(KERNEL_ADDRESS_IMAGE_BASE, &ehdr, sizeof(ehdr))) {
    perror("kernel_copyout");
    return -1;
  }

  if(ehdr.e_ident[0] != 0x7f || ehdr.e_ident[1] != 'E' ||
     ehdr.e_ident[2] != 'L'  || ehdr.e_ident[3] != 'F') {
    puts("not an ELF file");
    return -1;
  }

  // Compute size of virtual memory region.
  for(int i=0; i<ehdr.e_phnum; i++) {
    if(kernel_copyout(KERNEL_ADDRESS_IMAGE_BASE + ehdr.e_phoff + i*sizeof(Elf64_Phdr),
		      &phdr, sizeof(phdr))) {
      perror("kernel_copyout");
      return -1;
    }

    if(phdr.p_vaddr < min_vaddr) {
      min_vaddr = phdr.p_vaddr;
    }

    if(max_vaddr < phdr.p_vaddr + phdr.p_memsz) {
      max_vaddr = phdr.p_vaddr + phdr.p_memsz;
    }
  }

  return sizeof(ehdr) + ehdr.e_phnum*sizeof(phdr) + max_vaddr - min_vaddr;
}


int
main() {
  uint8_t buf[0x4000];
  ssize_t imgsize;
  size_t len;

  if((imgsize=kernel_get_image_size()) < 0) {
    return -1;
  }

  for(size_t i=0; i<imgsize; i+=sizeof(buf)) {
    len = imgsize - i;
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

