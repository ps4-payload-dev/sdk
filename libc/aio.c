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

#include <aio.h>
#include <errno.h>
#include <unistd.h>

#include <sys/syscall.h>


int
aio_read(struct aiocb *iocb) {
  int err;

  if((err=syscall(SYS_aio_read, iocb))) {
    errno = -err;
    return -1;
  }

  return 0;
}


int
aio_write(struct aiocb *iocb) {
  int err;

  if((err=syscall(SYS_aio_write, iocb))) {
    errno = -err;
    return -1;
  }

  return 0;
}


int
lio_listio(int mode, struct aiocb * const list[], int nent,
	   struct sigevent *sig) {
  int err;

  if((err=syscall(SYS_lio_listio, mode, list, nent, sig))) {
    errno = -err;
    return -1;
  }

  return 0;
}


int
aio_error(const struct aiocb *iocb) {
  int err;

  if((err=syscall(SYS_aio_error, iocb))) {
    errno = -err;
    return -1;
  }

  return 0;
}


ssize_t
aio_return(struct aiocb *iocb) {
  int err;

  if((err=syscall(SYS_aio_return, iocb))) {
    errno = -err;
    return -1;
  }

  return 0;
}


int
aio_cancel(int fildes, struct aiocb *iocb) {
  int err;

  if((err=syscall(SYS_aio_cancel, fildes, iocb))) {
    errno = -err;
    return -1;
  }

  return 0;
}


int
aio_suspend(const struct aiocb * const iocbs[], int niocb,
	    const struct timespec *ts) {
  int err;

  if((err=syscall(SYS_aio_suspend, iocbs, niocb, ts))) {
    errno = -err;
    return -1;
  }

  return 0;
}


int
aio_waitcomplete(struct aiocb **aiocb, struct timespec *ts) {
  int err;

  if((err=syscall(SYS_aio_waitcomplete, aiocb, ts))) {
    errno = -err;
    return -1;
  }

  return 0;
}
