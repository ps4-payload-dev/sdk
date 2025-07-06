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

#include "kernel.h"
#include "klog.h"
#include "rtld.h"
#include "syscall.h"


/**
 * Dependencies provided by the ELF linker.
 **/
extern void (*__init_array_start[])(int, char**, char**) __attribute__((weak));
extern void (*__init_array_end[])(int, char**, char**) __attribute__((weak));

extern void (*__fini_array_start[])(void) __attribute__((weak));
extern void (*__fini_array_end[])(void) __attribute__((weak));

extern unsigned char __bss_start[] __attribute__((weak));
extern unsigned char __bss_end[] __attribute__((weak));


/**
 * Entry point to the main program.
 **/
extern int main(int argc, char* argv[], char *envp[]);


/**
 * Terminate the payload.
 **/
static int
payload_terminate(int status) {
  void (*_exit)(int) = 0;

  // we are running inside a hijacked process, just return
  if(!DYNLIB_DLSYM(0x2001, "_exit", &_exit)) {
    return status;
  }

  // resolve and run _exit
  if(!DYNLIB_DLSYM(0x1, "_exit", &_exit)) {
    _exit(status);
  }

  // should not happend
  __builtin_trap();

  return -1;
}


/**
 * Initialize the payload.
 **/
static int
payload_init(void) {
  int err = 0;

  if((err=__klog_init())) {
    return err;
  }
  if((err=__kernel_init())) {
    return err;
  }
  if((err=__rtld_init())) {
    return err;
  }

  return err;
}



/**
 * Entry-point invoked by the ELF loader.
 **/
int
_start(void) {
  char** (*getargv)(void) = 0;
  int (*getargc)(void) = 0;
  unsigned long count = 0;
  char** envp = 0;
  char** argv = 0;
  int argc = 0;
  int err = 0;

  // clear .bss section
  for(unsigned char* bss=__bss_start; bss<__bss_end; bss++) {
    *bss = 0;
  }

  // initialize crt
  if((err=payload_init())) {
    return payload_terminate(err);
  }

  // get argc, argv, and envp
  if(DYNLIB_DLSYM(0x2001, "getargc", &getargc)) {
    DYNLIB_DLSYM(0x1, "getargc", &getargc);
  }
  if(DYNLIB_DLSYM(0x2001, "getargv", &getargv)) {
    DYNLIB_DLSYM(0x1, "getargv", &getargv);
  }
  if(DYNLIB_DLSYM(0x2001, "environ", &envp)) {
    DYNLIB_DLSYM(0x1, "environ", &envp);
  }
  if(getargc && getargv) {
    argc = getargc();
    argv = getargv();
  }

  // Run .init functions
  count = __init_array_end - __init_array_start;
  for(int i=0; i<count; i++) {
    __init_array_start[i](argc, argv, envp);
  }

  // Run the payload
  err = main(argc, argv, envp);

  // Run .fini functions
  count = __fini_array_end - __fini_array_start;
  for(int i=0; i<count; i++) {
    __fini_array_start[count-i-1]();
  }

  return payload_terminate(err);
}
