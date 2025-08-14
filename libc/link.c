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
#include <unistd.h>

#include <sys/syscall.h>


int
symlink(const char *target, const char *linkpath) {
  int err;

  if((err=syscall(SYS_symlink, target, linkpath))) {
    errno = -err;
    return -1;
  }

  return 0;
}



int
link(const char *oldpath, const char *newpath) {
  int err;

  if((err=syscall(SYS_link, oldpath, newpath))) {
    errno = -err;
    return -1;
  }

  return 0;
}


ssize_t
readlink(const char *pathname, char *buf, size_t bufsize) {
  int err;

  if((err=syscall(SYS_readlink, pathname, buf, bufsize))) {
    errno = -err;
    return -1;
  }

  return 0;
}
  
