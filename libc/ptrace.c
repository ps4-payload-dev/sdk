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

#include <sys/ptrace.h>
#include <sys/syscall.h>

#include <ps4/authid.h>
#include <ps4/kernel.h>


int
ptrace(int request, pid_t pid, caddr_t addr, int data) {
  uint64_t authid = kernel_get_ucred_authid(-1);
  pid_t my_pid = getpid();
  int err = 0;

  kernel_set_ucred_authid(my_pid, SCE_AUTHID_SYSCORE);
  if((err=syscall(SYS_ptrace, request, pid, addr, data))) {
    errno = -err;
  }
  kernel_set_ucred_authid(my_pid, authid);
  
  return err ? -1 : 0;
}
