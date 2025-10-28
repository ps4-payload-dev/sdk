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
#include "syscall.h"


#define ELF64_R_SYM(info) ((info) >> 32)
#define ELF64_ST_BIND(info) ((info) >> 4)

#define STB_WEAK 2

#define DT_NULL     0
#define DT_NEEDED   1
#define DT_PLTRELSZ 2
#define DT_STRTAB   5
#define DT_SYMTAB   6
#define DT_RELA     7
#define DT_RELASZ   8
#define DT_JMPREL   23

#define R_X86_64_64       1
#define R_X86_64_GLOB_DAT 6
#define R_X86_64_JMP_SLOT 7
#define R_X86_64_RELATIVE 8

#define RTLD_LAZY     0x0001
#define RTLD_NOW      0x0002
#define RTLD_MODEMASK 0x0003
#define RTLD_GLOBAL   0x0100
#define RTLD_LOCAL    0x0000
#define RTLD_TRACE    0x0200
#define RTLD_NODELETE 0x1000
#define RTLD_NOLOAD   0x2000

#define ENOENT 2
#define EINVAL 22
#define ENOSYS 78


typedef struct {
  long d_tag;
  union {
    unsigned long d_val;
    unsigned long d_ptr;
  } d_un;
} Elf64_Dyn;


typedef struct {
  unsigned int   st_name;
  unsigned char  st_info;
  unsigned char  st_other;
  unsigned short st_shndx;
  unsigned long  st_value;
  unsigned long  st_size;
} Elf64_Sym;


typedef struct {
  unsigned long r_offset;
  unsigned long r_info;
  long          r_addend;
} Elf64_Rela;


/**
 * dependencies provided by the ELF linker.
 **/
extern unsigned char __image_start[] __attribute__((weak));
extern Elf64_Dyn _DYNAMIC[] __attribute__((weak));


/**
 * global private variables.
 **/
static unsigned int g_libkernel_handle = 1;
static unsigned int g_handles[0x1000];
static int g_nb_handles = 0;


/**
 * external dependencies.
 **/
static void* (*memcpy)(void*, const void*, unsigned long) = 0;
static int   (*strcmp)(const char*, const char*) = 0;
static int   (*sprintf)(char*, const char*, ...) = 0;
static int   (*sceSysmoduleLoadModuleInternal)(unsigned int) = 0;


/**
 * Lookup table for sceSysmoduleLoadModuleInternal().
 **/
static struct sysmodtab {
  const char* name;
  unsigned int handle;
} sysmodtab[] = {
  {"libSceNet.sprx", 0x8000001c},
  {"libSceSystemService.sprx", 0x80000010},
  {"libSceUserService.sprx", 0x80000011},
};


/**
 * Log a symbol resolution error to /dev/klog
 **/
static void
klog_resolve_error(const char *name) {
  klog_printf("Unable to resolve the symbol '%s'\n", name);
}


/**
 * Log a library loading error to /dev/klog
 **/
static void
klog_libload_error(const char *name) {
  klog_printf("Unable to load the library '%s'\n", name);
}


static int
sprx_find_file(const char *name, char* path) {
  char buf[0x1000];

  sprintf(path, "/system/priv/lib/%s", name);
  if(!__syscall(SYS_stat, path, buf)) {
    return 0;
  }

  sprintf(path, "/system/common/lib/%s", name);
  if(!__syscall(SYS_stat, path, buf)) {
    return 0;
  }

  sprintf(path, "/system_ex/priv_ex/lib/%s", name);
  if(!__syscall(SYS_stat, path, buf)) {
    return 0;
  }

  sprintf(path, "/system_ex/common_ex/lib/%s", name);
  if(!__syscall(SYS_stat, path, buf)) {
    return 0;
  }

  return -1;
}


static void*
sprx_dlsym(const char* symname) {
  void* p = 0;

  if(!DYNLIB_DLSYM(g_libkernel_handle, symname, &p)) {
    return p;
  }

  if(!DYNLIB_DLSYM(0x2, symname, &p)) {
    return p;
  }

  for(int i=0; i<g_nb_handles; i++) {
    if(!DYNLIB_DLSYM(g_handles[i], symname, &p)) {
      return p;
    }
  }

  return 0;
}


static unsigned int
sprx_open(const char* libname) {
  unsigned int handle;
  char path[1024];

  if(!strcmp(libname, "libkernel.sprx") ||
     !strcmp(libname, "libkernel_web.sprx") ||
     !strcmp(libname, "libkernel_sys.sprx")) {
    return g_libkernel_handle;
  }

  if(!strcmp(libname, "libSceLibcInternal.sprx")) {
    return 0x2;
  }

  if(sprx_find_file(libname, path)) {
    return 0;
  }

  for(int i=0; i<sizeof(sysmodtab)/sizeof(sysmodtab[0]); i++) {
    if(!strcmp(libname, sysmodtab[i].name)) {
      if(sceSysmoduleLoadModuleInternal(sysmodtab[i].handle)) {
	return 0;
      }
    }
  }

  if(!DYNLIB_LOAD(path, &handle)) {
    return handle;
  }
  
  return 0;
}


/**
 * Process the DT_NEEDED entry in the .dynamic section.
 **/
static int
rtld_dt_needed(const char* libname) {
  unsigned int handle;
  
  if((handle=sprx_open(libname))) {
    g_handles[g_nb_handles++] = handle;
    return 0;
  }

  klog_libload_error(libname);
  return -1;
}


/**
 * Process the R_X86_64_GLOB_DAT relocation type.
 **/
static int
rtld_r_glob_dat(unsigned char* img, Elf64_Sym* symtab, char* strtab, Elf64_Rela* rela) {
  Elf64_Sym* sym = symtab + ELF64_R_SYM(rela->r_info);
  const char* name = strtab + sym->st_name;
  void* loc = img + rela->r_offset;
  void* val = 0;

  if((val=sprx_dlsym(name))) {
    memcpy(loc, &val, sizeof(val));
    return 0;
  }

  // ignore unresolved weak symbols
  if(ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
    return 0;
  }

  klog_printf("Unable to resolve '%s'\n", name);

  return -1;
}


/**
 * Process the R_X86_64_JMP_SLOT relocation type.
 **/
static int
rtld_r_jmp_slot(unsigned char* img, Elf64_Sym* symtab, char* strtab, Elf64_Rela* rela) {
  return rtld_r_glob_dat(img, symtab, strtab, rela);
}


/**
 * Process the R_X86_64_64 relocation type.
 **/
static int
rtld_r_direct_64(unsigned char* img, Elf64_Sym* symtab, char* strtab, Elf64_Rela* rela) {
  Elf64_Sym* sym = symtab + ELF64_R_SYM(rela->r_info);
  const char* name = strtab + sym->st_name;
  void* loc = img + rela->r_offset;
  void* val = 0;

  if((val=sprx_dlsym(name))) {
    val += rela->r_addend;
    memcpy(loc, &val, sizeof(val));
    return 0;
  }

  // ignore unresolved weak symbols
  if(ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
    return 0;
  }

  klog_printf("Unable to resolve '%s'\n", name);

  return -1;
}


/**
 * Process the R_X86_64_RELATIVE relocation type.
 **/
static int
rtld_r_relative(unsigned char* img, Elf64_Rela* rela) {
  unsigned long* loc = (unsigned long*)(img + rela->r_offset);
  unsigned long val = (unsigned long)(img + rela->r_addend);

  memcpy(loc, &val, sizeof(val));

  return 0;
}


/**
 *
 **/
static int
rtld_load(unsigned char* image_start, Elf64_Dyn* dyn) {
  unsigned long relasz = 0;
  unsigned long pltsz = 0;
  Elf64_Sym* symtab = 0;
  Elf64_Rela* rela = 0;
  Elf64_Rela* plt = 0;
  char* strtab = 0;
  int err;

  // find lookup tables
  for(int i=0; dyn[i].d_tag!=DT_NULL; i++) {
    switch(dyn[i].d_tag) {
    case DT_SYMTAB:
      symtab = (Elf64_Sym*)(image_start + dyn[i].d_un.d_ptr);
      break;

    case DT_STRTAB:
      strtab = (char*)(image_start + dyn[i].d_un.d_ptr);
      break;

    case DT_RELA:
      rela = (Elf64_Rela*)(image_start + dyn[i].d_un.d_ptr);
      break;

    case DT_RELASZ:
      relasz = dyn[i].d_un.d_val;
      break;

    case DT_JMPREL:
      plt = (Elf64_Rela*)(image_start + _DYNAMIC[i].d_un.d_ptr);
      break;

    case DT_PLTRELSZ:
      pltsz = _DYNAMIC[i].d_un.d_val;
      break;
    }
  }

  // load needed libraries
  for(int i=0; dyn[i].d_tag!=DT_NULL; i++) {
    switch(dyn[i].d_tag) {
    case DT_NEEDED:
      if((err=rtld_dt_needed(strtab + dyn[i].d_un.d_val))) {
	return err;
      }
      break;
    }
  }

  // apply relocations
  for(int i=0; i<relasz/sizeof(Elf64_Rela); i++) {
    switch(rela[i].r_info & 0xffffffffl) {
    case R_X86_64_JMP_SLOT:
      if((err=rtld_r_jmp_slot(image_start, symtab, strtab, &rela[i]))) {
	return err;
      }
      break;

    case R_X86_64_64:
      if((err=rtld_r_direct_64(image_start, symtab, strtab, &rela[i]))) {
	return err;
      }
      break;

    case R_X86_64_GLOB_DAT:
      if((err=rtld_r_glob_dat(image_start, symtab, strtab, &rela[i]))) {
	return err;
      }
      break;

    case R_X86_64_RELATIVE:
      if((err=rtld_r_relative(image_start, &rela[i]))) {
	return err;
      }
      break;
    }
  }

  for(int i=0; i<pltsz/sizeof(Elf64_Rela); i++) {
    switch(plt[i].r_info & 0xffffffffl) {
    case R_X86_64_JMP_SLOT:
      if((err=rtld_r_jmp_slot(image_start, symtab, strtab, &plt[i]))) {
	return err;
      }
      break;

    default:
      klog_printf("Unsupported plt type %x\n", plt[i].r_info);
      break;
    }
  }

  return 0;
}


int
__rtld_init(void) {
  unsigned int libSceSysmodule;
  unsigned long jaildir;
  unsigned long rootdir;
  unsigned long prison;
  int err = 0;
  long l;

  if(!DYNLIB_DLSYM(0x1, "getpid", &l)) {
    g_libkernel_handle = 0x1;
  } else if(!DYNLIB_DLSYM(0x2001, "getpid", &l)) {
    g_libkernel_handle = 0x2001;
  } else {
    klog_puts("unable to determine libkernel handle");
    return -1;
  }

  if((err=DYNLIB_DLSYM(0x2, "memcpy", &memcpy))) {
    klog_resolve_error("memcpy");
    return err;
  }
  if((err=DYNLIB_DLSYM(0x2, "strcmp", &strcmp))) {
    klog_resolve_error("strcmp");
    return err;
  }
  if((err=DYNLIB_DLSYM(0x2, "sprintf", &sprintf))) {
    klog_resolve_error("sprintf");
    return err;
  }

  if(!(prison=kernel_get_ucred_prison(-1))) {
    klog_puts("kernel_get_ucred_prison failed");
    return -1;
  }
  if(!(rootdir=kernel_get_proc_rootdir(-1))) {
    klog_puts("kernel_get_proc_rootdir failed");
    return -1;
  }
  if(!(jaildir=kernel_get_proc_jaildir(-1))) {
    klog_puts("kernel_get_proc_jaildir failed");
    return -1;
  }

  if((err=kernel_set_proc_rootdir(-1, KERNEL_ADDRESS_ROOTVNODE))) {
    klog_puts("kernel_set_proc_rootdir failed");

  } else if((err=kernel_set_proc_jaildir(-1, KERNEL_ADDRESS_ROOTVNODE))) {
    klog_puts("kernel_set_proc_jaildir failed");

  } else if((err=kernel_set_ucred_prison(-1, KERNEL_ADDRESS_PRISON0))) {
    klog_puts("kernel_set_proc_rootdir failed");
  }

  if(DYNLIB_LOAD("/system/common/lib/libSceSysmodule.sprx", &libSceSysmodule)) {
    klog_libload_error("/system/common/lib/libSceSysmodule.sprx");
    return -1;
  }
  if(DYNLIB_DLSYM(libSceSysmodule, "sceSysmoduleLoadModuleInternal",
		   &sceSysmoduleLoadModuleInternal)) {
    klog_resolve_error("sceSysmoduleLoadModuleInternal");
    return -1;
  }

  if(!err) {
    err = rtld_load(__image_start, _DYNAMIC);
  }

  if(kernel_set_proc_rootdir(-1, rootdir)) {
    klog_puts("kernel_set_proc_rootdir failed");
    err = -1;
  }
  if(kernel_set_proc_jaildir(-1, jaildir)) {
    klog_puts("kernel_set_proc_jaildir failed");
    err = -1;
  }
  if(kernel_set_ucred_prison(-1, prison)) {
    klog_puts("kernel_set_proc_rootdir failed");
    err = -1;
  }

  return err;
}
