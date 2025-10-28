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

#include <stdio.h>

#ifndef PS4_URL
#define PS4_URL "http://192.168.1.1"
#endif


int sceUserServiceInitialize(void*);
int sceUserServiceTerminate(void);

int sceSystemServiceLaunchWebBrowser(const char *uri, void*);


int
main() {
  if(sceUserServiceInitialize(0)) {
    perror("sceUserServiceInitialize");

  } else if(sceSystemServiceLaunchWebBrowser(PS4_URL, 0)) {
    perror("sceSystemServiceLaunchWebBrowser");
  }

  sceUserServiceTerminate();

  return 0;
}

