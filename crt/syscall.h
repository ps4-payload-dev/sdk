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

#pragma once

#define SYS_kexec        11
#define SYS_getpid       20
#define SYS_munmap       73
#define SYS_stat         188
#define SYS_sysctl       202
#define SYS_mmap         477
#define SYS_mdbg_call    573
#define SYS_dynlib_dlsym 591
#define SYS_dynlib_load  594

#define DYNLIB_LOAD(name, handle) __syscall(SYS_dynlib_load, name, 0, handle, 0)
#define DYNLIB_DLSYM(handle, name, addr) __syscall(SYS_dynlib_dlsym, handle, name, addr)


static inline long
__syscall(long n, ...) {
  long a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0;
  __builtin_va_list ap;
  unsigned long ret;
  char err;

  __builtin_va_start(ap, n);
  a1 = __builtin_va_arg(ap, long);
  a2 = __builtin_va_arg(ap, long);
  a3 = __builtin_va_arg(ap, long);
  a4 = __builtin_va_arg(ap, long);
  a5 = __builtin_va_arg(ap, long);
  a6 = __builtin_va_arg(ap, long);
  __builtin_va_end(ap);

  register long r10 asm("r10") = a4;
  register long r8  asm("r8")  = a5;
  register long r9  asm("r9")  = a6;

  asm("syscall"
      : "=a"(ret), "=@ccc"(err), "+r"(r10), "+r"(r8), "+r"(r9)
      : "a"(n), "D"(a1), "S"(a2), "d"(a3)
      : "rcx", "r11", "memory");

  return err ? -ret : ret;
}
