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

#include <errno.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>


#define SYS_dynlib_dlsym   591
#define SYS_dynlib_load    594
#define SYS_dynlib_unload  595


static char dlerror_str[0x100] = {0};


void*
dlopen(const char *filename, int flags) {
  int handle;
  int err;

  if((err=syscall(SYS_dynlib_load, filename, 0, &handle, 0))) {
    strerror_r(-err, dlerror_str, sizeof(dlerror_str));
    return 0;
  }

  dlerror_str[0] = 0;
  return (void*)(long)handle;
}


void*
dlsym(void *handle, const char *symbol) {
  void* addr = 0;
  int err;

  if((err=syscall(SYS_dynlib_dlsym, handle, symbol, &addr))) {
    strerror_r(-err, dlerror_str, sizeof(dlerror_str));
    return 0;
  }

  dlerror_str[0] = 0;
  return addr;
}


int
dlclose(void *handle) {
  int err;

  if((err=syscall(SYS_dynlib_unload, handle, 0, 0, 0, 0, 0))) {
    strerror_r(-err, dlerror_str, sizeof(dlerror_str));
    return -1;
  }

  dlerror_str[0] = 0;
  return 0;
}


int
dladdr(const void *addr, Dl_info *info) {
#warning dladdr is not implemented
  errno = ENOTSUP;
  return -1;
}


char*
dlerror(void) {
  return dlerror_str;
}

