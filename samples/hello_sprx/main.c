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

#include <stdint.h>
#include <stdio.h>
#include <string.h>


int sceRandomGetRandomNumber(uint8_t* buf, size_t size);


static void
hexdump(uint8_t* buf, size_t size) {
  for(int i=0; i<size; i++) {
    if(i%16 == 0 && i!= 0) {
      printf("\n");
    }
    printf("%02x ", buf[i]);
  }
  printf("\n");
}


int main() {
  uint8_t buf[0x40];

  memset(buf, 0, sizeof(buf));
  if(!sceRandomGetRandomNumber(buf, sizeof(buf))) {
    hexdump(buf, sizeof(buf));
  }

  return 0;
}
