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
#include "syscall.h"


/**
 * Private constants.
 **/
#define X86_CR0_WP (1 << 16)
#define MSR_LSTAR  0xC0000082
#define EFAULT     14
#define EINVAL     22
#define ENOSYS     78


/**
 * Forward declaration of the data structure used by FreeBSD to capture the state
 * of kernel threads.
 **/
struct thread;


/**
 * Arguments passed to the kexec syscall.
 **/
typedef struct kexec_args {
  int (*fn)(struct thread*, struct kexec_args*);
  long arg1;
  long arg2;
  long arg3;
  long arg4;
  long arg5;
} kexec_args_t;


/**
 * Context used by kexec_find_pattern().
 **/
typedef struct {
  unsigned long kaddr;
  unsigned int kaddr_len;
  char pattern[0x100];
  unsigned long *res;
} kexec_find_pattern_ctx_t;


/**
 * Data structure for the ELF header.
 **/
typedef struct {
  unsigned char e_ident[16];
  unsigned short e_type;
  unsigned short e_machine;
  unsigned int e_version;
  unsigned long e_entry;
  unsigned long e_phoff;
  unsigned long e_shoff;
  unsigned int e_flags;
  unsigned short e_ehsize;
  unsigned short e_phentsize;
  unsigned short e_phnum;
  unsigned short e_shentsize;
  unsigned short e_shnum;
  unsigned short e_shstrndx;
} Elf64_Ehdr;


/**
 * Data structure for the ELF program header.
 **/
typedef struct {
  unsigned int p_type;
  unsigned int p_flags;
  unsigned long p_offset;
  unsigned long p_vaddr;
  unsigned long p_paddr;
  unsigned long p_filesz;
  unsigned long p_memsz;
  unsigned long p_align;
} Elf64_Phdr;


/**
 * Public constants.
 **/
unsigned long KERNEL_IMAGE_SIZE = 0;

unsigned long KERNEL_ADDRESS_IMAGE_BASE = 0;
unsigned long KERNEL_ADDRESS_ALLPROC    = 0;
unsigned long KERNEL_ADDRESS_ROOTVNODE  = 0;
unsigned long KERNEL_ADDRESS_PRISON0    = 0;
unsigned long KERNEL_ADDRESS_TARGETID   = 0;
unsigned long KERNEL_ADDRESS_QA_FLAGS   = 0;
unsigned long KERNEL_ADDRESS_COPYIN     = 0;
unsigned long KERNEL_ADDRESS_COPYOUT    = 0;

const unsigned long KERNEL_OFFSET_PROC_P_UCRED   = 0x40;
const unsigned long KERNEL_OFFSET_PROC_P_FD      = 0x48;
const unsigned long KERNEL_OFFSET_PROC_P_PID     = 0xB0;

const unsigned long KERNEL_OFFSET_UCRED_CR_UID   = 0x04;
const unsigned long KERNEL_OFFSET_UCRED_CR_RUID  = 0x08;
const unsigned long KERNEL_OFFSET_UCRED_CR_SVUID = 0x0C;
const unsigned long KERNEL_OFFSET_UCRED_CR_RGID  = 0x14;
const unsigned long KERNEL_OFFSET_UCRED_CR_SVGID = 0x18;

const unsigned long KERNEL_OFFSET_UCRED_CR_PRISON = 0x30;

const unsigned long KERNEL_OFFSET_UCRED_CR_SCEAUTHID = 0x58;
const unsigned long KERNEL_OFFSET_UCRED_CR_SCECAPS   = 0x60;
const unsigned long KERNEL_OFFSET_UCRED_CR_SCEATTRS  = 0x83;

const unsigned long KERNEL_OFFSET_FILEDESC_FD_CDIR = 0x10;
const unsigned long KERNEL_OFFSET_FILEDESC_FD_RDIR = 0x18;
const unsigned long KERNEL_OFFSET_FILEDESC_FD_JDIR = 0x20;


/**
 * Compare memory areas.
 **/
static int
memcmp(const unsigned char* dst, const unsigned char* src, unsigned long len) {
  for(unsigned long i=0; i<len; i++) {
    if(dst[i] != src[i]) {
      return src[i] - dst[i];
    }
  }
  return 0;
}


/**
 * Convert a hexchar to a nibble (half byte).
 **/
static inline __attribute__((always_inline)) short
hex2nib(char hexchar) {
  if(hexchar >= '0' && hexchar <= '9') {
    return hexchar - '0';
  }

  if(hexchar >= 'A' && hexchar <= 'F') {
    return hexchar - 'A' + 10;
  }

  if(hexchar >= 'a' && hexchar <= 'f') {
    return hexchar - 'a' + 10;
  }

  return -1;
}


/**
 * Check if a byte value matches a hexstring.
 **/
static inline __attribute__((always_inline)) short
byte_match(unsigned char bval, const char* hexstr) {
  short nib;

  if(!hexstr[0] || hexstr[0] == '?') {
    return 1;
  }

  if((nib=hex2nib(hexstr[0])) < 0) {
    return -1;
  }

  if(nib != (bval >> 4)) {
    return -1;
  }

  if(!hexstr[1] || hexstr[1] == '?') {
    return 1;
  }

  if((nib=hex2nib(hexstr[1])) < 0) {
    return -1;
  }

  if(nib != (bval & 0xf)) {
    return -1;
  }

  return 1;
}


/**
 * Check if a memory region matches a hexstring.
 **/
static inline __attribute__((always_inline)) int
pattern_match(const unsigned char* addr, unsigned int len, const char* hexstr) {
  while(len && *hexstr) {
    if(byte_match(*addr, hexstr) < 0) {
      return -1;
    }

    hexstr++;
    addr++;
    len--;

    if(*hexstr) {
      hexstr++;
    }
  }

  return 1;
}


/**
 * Scan a memory region for a pattern encoded by the given hexstring.
 **/
static inline __attribute__((always_inline)) const unsigned char*
pattern_scan(const unsigned char* addr, unsigned int len, const char* hexstr) {
  for(unsigned int i=0; i<len; i++) {
    if(pattern_match(addr+i, len-i, hexstr) < 0) {
      continue;
    }

    return addr+i;
  }

  return 0;
}


/**
 * Invoke the given function from kernel space.
 **/
int
kexec(int (*fn)(struct thread*, kexec_args_t*), ...) {
  long a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0;
  __builtin_va_list ap;
  int err;

  __builtin_va_start(ap, fn);
  a1 = __builtin_va_arg(ap, long);
  a2 = __builtin_va_arg(ap, long);
  a3 = __builtin_va_arg(ap, long);
  a4 = __builtin_va_arg(ap, long);
  a5 = __builtin_va_arg(ap, long);
  __builtin_va_end(ap);

  if((err=__syscall(SYS_kexec, (long)fn, a1, a2, a3, a4, a5)) < 0) {
    return err;
  }

  return 0;
}


/**
 * Get a model specific register value (must be invoked from kernel space).
 **/
static int
kexec_get_msr(struct thread *td, kexec_args_t* args) {
  unsigned int reg = (unsigned int)args->arg1;
  unsigned long* val = (unsigned long*)args->arg2;
  unsigned int edx;
  unsigned int eax;

  if(!val) {
    return EINVAL;
  }

  asm volatile(".att_syntax\n"
	       "rdmsr" : "=d"(edx), "=a"(eax) : "c"(reg));

  *val = (((unsigned long)edx) << 32) | (unsigned long)eax;

  return 0;
}


/**
 * Scan for the kernel image base address. This approach works for firmwares
 * 5.00-12.52. Earlier versions store the image at the start of a different page
 * table.
 *
 * TODO: search for ELF magic in other page tables.
 **/
static int
kexec_find_image_base(struct thread *td, kexec_args_t* args) {
  unsigned long* res = (unsigned long*)args->arg1;
  unsigned long base;
  unsigned int edx;
  unsigned int eax;

  if(!res) {
    return EFAULT;
  }

  asm volatile(".att_syntax\n"
	       "rdmsr" : "=d"(edx), "=a"(eax) : "c"(MSR_LSTAR));

  base = (((unsigned long)edx) << 32) | (unsigned long)eax;
  base &= 0xFFFFFFFFFFFFC000;

  *res = base;

  return 0;
}


/**
 * Get the size of the kernel image.
 **/
static int
kexec_get_image_size(struct thread *td, kexec_args_t* args) {
  const unsigned char* img = (const unsigned char*)args->arg1;
  unsigned long* res = (unsigned long*)args->arg2;
  const Elf64_Ehdr* ehdr = (const Elf64_Ehdr*)img;
  const Elf64_Phdr* phdr = (const Elf64_Phdr*)(img + ehdr->e_phoff);
  unsigned long min_vaddr = -1;
  unsigned long max_vaddr = 0;

  if(!img || !res) {
    return EFAULT;
  }

  if(ehdr->e_ident[0] != 0x7f || ehdr->e_ident[1] != 'E' ||
     ehdr->e_ident[2] != 'L'  || ehdr->e_ident[3] != 'F') {
    return EINVAL;
  }

  for(int i=0; i<ehdr->e_phnum; i++) {
    if(phdr[i].p_vaddr < min_vaddr) {
      min_vaddr = phdr[i].p_vaddr;
    }

    if(max_vaddr < phdr[i].p_vaddr + phdr[i].p_memsz) {
      max_vaddr = phdr[i].p_vaddr + phdr[i].p_memsz;
    }
  }

  *res = sizeof(ehdr) + ehdr->e_phnum*sizeof(phdr) + max_vaddr - min_vaddr;

  return 0;
}


/**
 * Search for the entry to the kernel copyin function.
 **/
static int
kexec_find_copyin(struct thread *td, kexec_args_t* args) {
  char pattern[] = "554889e54989fa4901d24939e2??174989ea4983c2104c39d77d0b4889f"
                   "f4889d6e8????????65488b0425200000004c8b05????????4c8980d000"
                   "0000";
  const unsigned char* img = (const unsigned char*)args->arg1;
  unsigned long img_size = (unsigned long)args->arg2;
  unsigned long* res = (unsigned long*)args->arg3;

  if(!img || !res) {
    return EFAULT;
  }

  if(!img_size) {
    return EINVAL;
  }

  *res = (long)pattern_scan(img, img_size, pattern);

  return 0;
}


/**
 * Search for the entry to the kernel copyout function.
 **/
static int
kexec_find_copyout(struct thread *td, kexec_args_t* args) {
  char pattern[] = "554889e565488b0425200000004c8b05????????4c8980d00000004885d"
                   "274??65488b0c2500000000488b4108488b88????????48f7c100000040";
  const unsigned char* img = (const unsigned char*)args->arg1;
  unsigned long img_size = (unsigned long)args->arg2;
  unsigned long* res = (unsigned long*)args->arg3;

  if(!img || !res) {
    return EFAULT;
  }

  if(!img_size) {
    return EINVAL;
  }

  *res = (long)pattern_scan(img, img_size, pattern);

  return 0;
}


/**
 * Search for the global kernel variable targetid.
 *
 * TODO: add pattern scanning for targetid.
 **/
static int
kexec_find_targetid(struct thread *td, kexec_args_t* args) {
  char pattern[] = "488d0d????????31c0807934000f95c0";
  const unsigned char* img = (const unsigned char*)args->arg1;
  unsigned long img_size = (unsigned long)args->arg2;
  unsigned long* res = (unsigned long*)args->arg3;

  if(!img || !res) {
    return EFAULT;
  }

  if(!img_size) {
    return EINVAL;
  }

  if(0) {
    *res = (long)pattern_scan(img, img_size, pattern);
  } else {
    *res = 0;
  }

  return 0;
}


/**
 * Get the address of the invoking kernel thread (must be invoked from kernel space).
 **/
static int
kexec_get_thread(struct thread *td, kexec_args_t* args) {
  unsigned long* addr = (unsigned long*)args->arg1;

  if(!addr) {
    return EINVAL;
  }

  *addr = (long)td;

  return 0;
}


/**
 * Copy data from user space into kernel space (must be invoked from kernel space).
 **/
static int
kexec_copyin(struct thread *td, kexec_args_t* args) {
  int (*copyin)(const void*, void*, unsigned long) = (void*)args->arg1;
  const void* uaddr = (void*)args->arg2;
  void *kaddr = (void*)args->arg3;
  unsigned long len = args->arg4;
  unsigned long cr0;
  int ret;

  // read cr0
  asm volatile(".att_syntax\n"
	       "mov %%cr0, %0" : "=r" (cr0));

  // clear cr0 write protection bits
  asm volatile(".att_syntax\n"
	       "mov %0, %%cr0" : : "r" (cr0 & ~X86_CR0_WP));

  ret = copyin(uaddr, kaddr, len);

  // restore cr0
  asm volatile(".att_syntax\n"
	       "mov %0, %%cr0" : : "r" (cr0));

  return ret;
}


/**
 * Copy data from kernel space into user space (must be invoked from kernel space).
 **/
static int
kexec_copyout(struct thread *td, kexec_args_t* args) {
  int (*copyout)(const void*, void*, unsigned long) = (void*)args->arg1;
  const void *kaddr = (void*)args->arg2;
  void* uaddr = (void*)args->arg3;
  unsigned long len = args->arg4;

  return copyout(kaddr, uaddr, len);
}


/**
 * Find a pattern in kernel memory (must be invoked from kernel space).
 **/
static int
kexec_find_pattern(struct thread *td, kexec_args_t* args) {
  int (*copyin)(const void*, void*, unsigned long) = (void*)args->arg1;
  int (*copyout)(const void*, void*, unsigned long) = (void*)args->arg2;
  void* uctx = (void*)args->arg3;
  kexec_find_pattern_ctx_t ctx;
  unsigned long res;
  int err;

  if((err=copyin(uctx, &ctx, sizeof(ctx)))) {
    return err;
  }

  ctx.pattern[sizeof(ctx.pattern)-1] = 0;
  res = (long)pattern_scan((void*)ctx.kaddr, ctx.kaddr_len, ctx.pattern);

  return copyout(&res, ctx.res, sizeof(res));
}


unsigned int
kernel_get_fw_version(void) {
  int mib[2] = {1, 38};
  unsigned long size = sizeof(mib);
  unsigned int version = 0;

  if(__syscall(SYS_sysctl, mib, 2, &version, &size, 0, 0)) {
    return 0;
  }

  return version;
}


int
__kernel_init(void) {
  unsigned int fw = kernel_get_fw_version();
  unsigned long lstar = 0;
  unsigned long addr = 0;
  unsigned long prev = 0;
  int pid;
  int err;

  if(fw < 0x3500000) {
    return -ENOSYS;
  }

  if((err=kexec(kexec_get_msr, MSR_LSTAR, &lstar))) {
    return err;
  }

  switch(fw & 0xffff0000) {
  case 0x3500000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x003A1AD0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x019A121D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x003B92A0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x003B9220;
    break;

  case 0x3550000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x003A1F10;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x019A121D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x003B96E0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x003B9660;
    break;

  case 0x3700000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x003A2000;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x019AD21D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x003B97D0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x003B9750;
    break;

  case 0x4000000:
  case 0x4010000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x0030EA00;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0200151D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00286CC0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00286C40;
    break;

  case 0x4050000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x0030EB30;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0200151D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00286DF0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00286D70;
    break;

  case 0x4060000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x0030EB40;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0200151D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00286DF0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00286D70;
    break;

  case 0x4070000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x0030EB40;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0200551D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00286DF0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00286D70;
    break;

  case 0x4500000:
  case 0x4550000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x003095D0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01B6D08D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x0014A890;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x0014A7B0;
    break;

  case 0x4700000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x0030B840;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01B7D08D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00149F20;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00149E40;
    break;

  case 0x4710000:
  case 0x4720000:
  case 0x4730000:
  case 0x4740000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x0030B7D0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01B7D08D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00149F20;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00149E40;
    break;

  case 0x5000000:
  case 0x5010000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01CD068D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x001EA600;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x001EA520;
    break;

  case 0x5030000:
  case 0x5050000:
  case 0x5070000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01CD068D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x001EA710;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x001EA630;
    break;

  case 0x5500000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x022BBF8D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00405BB0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00405AC0;
    break;

  case 0x5530000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x022BBF8D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00405AB0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x004059C0;
    break;

  case 0x5550000:
  case 0x5560000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x022BBF8D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00405E70;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00405D80;
    break;

  case 0x6000000:
  case 0x6020000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x02159B8D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x001148F0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00114800;
    break;

  case 0x6200000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0215DB8D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x001148F0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00114800;
    break;

  case 0x6500000:
  case 0x6510000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01BD800D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x003C13F0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x003C1300;
    break;

  case 0x6700000:
  case 0x6710000:
  case 0x6720000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01BD800D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x003C17A0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x003C16B0;
    break;

  case 0x7000000:
  case 0x7010000:
  case 0x7020000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x022FED8D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x0002F230;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x0002F140;
    break;

  case 0x7500000:
  case 0x7510000:
  case 0x7550000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x022287CD;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x0028F9F0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x0028F900;
    break;

  case 0x8000000:
  case 0x8010000:
  case 0x8030000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01B5158D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x0025E3B0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x0025E2C0;
    break;

  case 0x8500000:
  case 0x8520000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01C8338D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x003A42E0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x003A41F0;
    break;

  case 0x9000000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0221688D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002716A0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002715B0;
    break;

  case 0x9030000:
  case 0x9040000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0221688D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00271320;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00271230;
    break;

  case 0x9500000:
  case 0x9510000:
  case 0x9600000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x0221A40D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00201EB0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00201DC0;
    break;

  case 0x10000000:
  case 0x10010000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01B9E08D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x00472F10;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x00472E20;
    break;

  case 0x10500000:
  case 0x10700000:
  case 0x10710000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x01BE460D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x000D7560;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x000D7470;
    break;

  case 0x11000000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x021CC60D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002DDFE0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002DDEF0;
    break;

  case 0x11020000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x021CC60D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002DE000;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002DDF10;
    break;

  case 0x11500000:
  case 0x11520000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x021CC60D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD590;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD4A0;
    break;

  case 0x12000000:
  case 0x12020000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x021CC60D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD670;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD580;
    break;

  case 0x12500000:
  case 0x12520000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x021CC60D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD6B0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD5C0;
    break;

  case 0x13000000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x021CC60D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD6D0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD5E0;
    break;

  case 0x13020000:
  case 0x13040000:
    KERNEL_ADDRESS_IMAGE_BASE = lstar - 0x000001C0;
    KERNEL_ADDRESS_TARGETID   = KERNEL_ADDRESS_IMAGE_BASE + 0x021CC60D;
    KERNEL_ADDRESS_COPYIN     = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD6E0;
    KERNEL_ADDRESS_COPYOUT    = KERNEL_ADDRESS_IMAGE_BASE + 0x002BD5F0;
    break;

  default:
    if(fw < 0x13040000) {
      return -ENOSYS;
    }

    if((err=kexec(kexec_find_image_base, &KERNEL_ADDRESS_IMAGE_BASE))) {
      return err;
    }
    if((err=kexec(kexec_get_image_size, KERNEL_ADDRESS_IMAGE_BASE,
		  &KERNEL_IMAGE_SIZE))) {
      return err;
    }
    if((err=kexec(kexec_find_copyin, KERNEL_ADDRESS_IMAGE_BASE,
		  KERNEL_IMAGE_SIZE, &KERNEL_ADDRESS_COPYIN))) {
      return err;
    }
    if((err=kexec(kexec_find_copyout, KERNEL_ADDRESS_IMAGE_BASE,
		  KERNEL_IMAGE_SIZE, &KERNEL_ADDRESS_COPYOUT))) {
      return err;
    }
    if((err=kexec(kexec_find_targetid, KERNEL_ADDRESS_IMAGE_BASE,
		  KERNEL_IMAGE_SIZE, &KERNEL_ADDRESS_TARGETID))) {
      return err;
    }

    if(!KERNEL_ADDRESS_IMAGE_BASE || !KERNEL_IMAGE_SIZE ||
       !KERNEL_ADDRESS_COPYIN || !KERNEL_ADDRESS_COPYOUT ||
       !KERNEL_ADDRESS_TARGETID) {
      return -ENOSYS;
    }
    break;
  }

  // get the size of the kernel image
  if((err=kexec(kexec_get_image_size, KERNEL_ADDRESS_IMAGE_BASE,
		&KERNEL_IMAGE_SIZE))) {
    return err;
  }

  // get the kernel address pointing at the current thread
  if((err=kexec(kexec_get_thread, &addr))) {
    return err;
  }

  // get the kernel address pointing at the current proc
  if((err=kernel_copyout(addr+8, &addr, sizeof(addr)))) {
    return err;
  }

  // find 'allproc'
  while(addr) {
    if(kernel_copyout(addr + KERNEL_OFFSET_PROC_P_PID, &pid,
		      sizeof(pid))) {
      return 0;
    }

    if(pid == 0) {
      break;
    }

    if(kernel_copyout(addr+8, &prev, sizeof(prev))) {
      return 0;
    }

    addr = prev;
  }

  // ensure we found 'allproc'
  if(!(KERNEL_ADDRESS_ALLPROC=addr)) {
    return -1;
  }

  // get the address pointing at the global kernel variable 'prison0'
  if(!(KERNEL_ADDRESS_PRISON0=kernel_get_ucred_prison(1))) {
    return -1;
  }

  // get the address pointing at the global kernel variable 'rootvnode'
  if(!(KERNEL_ADDRESS_ROOTVNODE=kernel_get_proc_rootdir(1))) {
    return -1;
  }

  KERNEL_ADDRESS_QA_FLAGS = KERNEL_ADDRESS_TARGETID + 0x1b;

  return 0;
}


int
kernel_copyin(const void *uaddr, unsigned long kaddr, unsigned long len) {
  if(!kaddr || !uaddr) {
    return -EINVAL;
  }

  return kexec(kexec_copyin, KERNEL_ADDRESS_COPYIN, uaddr, kaddr, len);
}


int
kernel_copyout(unsigned long kaddr, void *uaddr, unsigned long len) {
  if(!kaddr || !uaddr) {
    return -EINVAL;
  }

  return kexec(kexec_copyout, KERNEL_ADDRESS_COPYOUT, kaddr, uaddr, len);
}


int
kernel_setlong(unsigned long addr, unsigned long val) {
  return kernel_copyin(&val, addr, sizeof(val));
}


int
kernel_setint(unsigned long addr, unsigned int val) {
  return kernel_copyin(&val, addr, sizeof(val));
}


int
kernel_setshort(unsigned long addr, unsigned short val) {
  return kernel_copyin(&val, addr, sizeof(val));
}


int
kernel_setchar(unsigned long addr, unsigned char val) {
  return kernel_copyin(&val, addr, sizeof(val));
}


unsigned long
kernel_getlong(unsigned long addr) {
  unsigned long val = 0;

  kernel_copyout(addr, &val, sizeof(val));

  return val;
}


unsigned int
kernel_getint(unsigned long addr) {
  unsigned int val = 0;

  kernel_copyout(addr, &val, sizeof(val));

  return val;
}


unsigned short
kernel_getshort(unsigned long addr) {
  unsigned short val = 0;

  kernel_copyout(addr, &val, sizeof(val));

  return val;
}


unsigned char
kernel_getchar(unsigned long addr) {
  unsigned char val = 0;

  kernel_copyout(addr, &val, sizeof(val));

  return val;
}


unsigned long
kernel_find_pattern(unsigned long addr, unsigned int len, const char* pattern) {
  kexec_find_pattern_ctx_t ctx;
  unsigned long res = 0;
  int pattern_len = -1;

  if(!addr || !len || !pattern) {
    return 0;
  }

  ctx.kaddr = addr;
  ctx.kaddr_len = len;
  ctx.res = &res;

  for(int i=0; i<sizeof(ctx.pattern); i++) {
    ctx.pattern[i] = pattern[i];
    if(!pattern[i]) {
      pattern_len = i;
      break;
    }
  }

  if(pattern_len < 0 || pattern_len >= sizeof(ctx.pattern)-1) {
    return 0;
  }

  if(kexec(kexec_find_pattern, KERNEL_ADDRESS_COPYIN, KERNEL_ADDRESS_COPYOUT,
	   &ctx)) {
    return 0;
  }

  return res;
}


int
kernel_patch(unsigned long addr, const void* before, const void* after,
	     unsigned short size) {
  unsigned char current[size];

  // obtain current pattern
  if((kernel_copyout(addr, current, size))) {
    return -1;
  }

  // already patched
  if(!memcmp(current, after, size)) {
    return 0;
  }

  // before does not match current, wrong offset?
  if(before && memcmp(before, current, size)) {
    return -1;
  }

  // apply patch
  if((kernel_copyin(after, addr, size))) {
    return -1;
  }

  return 0;
}


unsigned long
kernel_get_proc(int pid) {
  unsigned int other_pid = 0;
  unsigned long addr = 0;
  unsigned long next = 0;

  if(pid <= 0) {
    pid = __syscall(SYS_getpid);
  }

  if(kernel_copyout(KERNEL_ADDRESS_ALLPROC, &addr, sizeof(addr))) {
    return 0;
  }

  while(addr) {
    if(kernel_copyout(addr + KERNEL_OFFSET_PROC_P_PID, &other_pid,
		      sizeof(other_pid))) {
      return 0;
    }

    if(pid == other_pid) {
      break;
    }

    if(kernel_copyout(addr, &next, sizeof(next))) {
      return 0;
    }

    addr = next;
  }

  return addr;
}


static int
kernel_dynlib_obj(int pid, unsigned int handle, dynlib_obj_t* obj) {
  unsigned long kproc;
  unsigned long kaddr;

  if(!(kproc=kernel_get_proc(pid))) {
    return -1;
  }

  if(kernel_copyout(kproc + 0x340, &kaddr, sizeof(kaddr)) < 0) {
    return -1;
  }

  do {
    if(kernel_copyout(kaddr, &kaddr, sizeof(kaddr)) < 0) {
      return -1;
    }
    if(!kaddr) {
      return -1;
    }

    if(kernel_copyout(kaddr + __builtin_offsetof(dynlib_obj_t, handle),
		      &obj->handle, sizeof(obj->handle)) < 0) {
      return -1;
    }
  } while(obj->handle != handle);

  if(kernel_copyout(kaddr, obj, sizeof(dynlib_obj_t)) < 0) {
    return -1;
  }

  return 0;
}

unsigned long
kernel_dynlib_mapbase_addr(int pid, unsigned int handle) {
  dynlib_obj_t obj;

  if(kernel_dynlib_obj(pid, handle, &obj)) {
    return 0;
  }

  return obj.mapbase;
}


unsigned long
kernel_dynlib_entry_addr(int pid, unsigned int handle) {
  dynlib_obj_t obj;

  if(kernel_dynlib_obj(pid, handle, &obj)) {
    return 0;
  }

  return obj.entry;
}


unsigned long
kernel_get_proc_filedesc(int pid) {
  unsigned long filedesc = 0;
  unsigned long proc = 0;

  if(!(proc=kernel_get_proc(pid))) {
    return 0;
  }

  if(kernel_copyout(proc + KERNEL_OFFSET_PROC_P_FD, &filedesc,
		    sizeof(filedesc))) {
    return 0;
  }

  return filedesc;
}


unsigned long
kernel_get_proc_ucred(int pid) {
  unsigned long proc = 0;
  unsigned long ucred = 0;

  if(!(proc=kernel_get_proc(pid))) {
    return 0;
  }

  if(kernel_copyout(proc + KERNEL_OFFSET_PROC_P_UCRED, &ucred,
		    sizeof(ucred))) {
    return 0;
  }

  return ucred;
}


unsigned long
kernel_get_ucred_prison(int pid) {
  unsigned long prison = 0;
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return 0;
  }

  if(kernel_copyout(ucred + KERNEL_OFFSET_UCRED_CR_PRISON, &prison,
		    sizeof(prison))) {
    return 0;
  }

  return prison;
}


int
kernel_set_ucred_prison(int pid, unsigned long prison) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&prison, ucred + KERNEL_OFFSET_UCRED_CR_PRISON,
		   sizeof(prison))) {
    return -1;
  }

  return 0;
}


unsigned long
kernel_get_ucred_authid(int pid) {
  unsigned long authid = 0;
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return 0;
  }

  if(kernel_copyout(ucred + KERNEL_OFFSET_UCRED_CR_SCEAUTHID, &authid,
		    sizeof(authid))) {
    return 0;
  }

  return authid;
}


int
kernel_set_ucred_authid(int pid, unsigned long authid) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&authid, ucred + KERNEL_OFFSET_UCRED_CR_SCEAUTHID,
		   sizeof(authid))) {
    return -1;
  }

  return 0;
}


int
kernel_get_ucred_caps(int pid, unsigned char caps[16]) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyout(ucred + KERNEL_OFFSET_UCRED_CR_SCECAPS, caps, 16)) {
    return -1;
  }

  return 0;
}


int
kernel_set_ucred_caps(int pid, const unsigned char caps[16]) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(caps, ucred + KERNEL_OFFSET_UCRED_CR_SCECAPS, 16)) {
    return -1;
  }

  return 0;
}


unsigned long
kernel_get_ucred_attrs(int pid) {
  unsigned long ucred = 0;
  unsigned long attrs = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return 0;
  }

  if(kernel_copyout(ucred + KERNEL_OFFSET_UCRED_CR_SCEATTRS, &attrs,
		    sizeof(attrs))) {
    return 0;
  }

  return attrs;
}


int
kernel_set_ucred_attrs(int pid, unsigned long attrs) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&attrs, ucred + KERNEL_OFFSET_UCRED_CR_SCEATTRS,
		   sizeof(attrs))) {
    return -1;
  }

  return 0;
}


int
kernel_set_ucred_uid(int pid, unsigned int uid) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&uid, ucred + KERNEL_OFFSET_UCRED_CR_UID,
		   sizeof(uid))) {
    return -1;
  }

  return 0;
}


int
kernel_set_ucred_ruid(int pid, unsigned int ruid) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&ruid, ucred + KERNEL_OFFSET_UCRED_CR_RUID,
		   sizeof(ruid))) {
    return -1;
  }

  return 0;
}


int
kernel_set_ucred_svuid(int pid, unsigned int svuid) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&svuid, ucred + KERNEL_OFFSET_UCRED_CR_SVUID,
		   sizeof(svuid))) {
    return -1;
  }

  return 0;
}


int
kernel_set_ucred_rgid(int pid, unsigned int rgid) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&rgid, ucred + KERNEL_OFFSET_UCRED_CR_RGID,
		   sizeof(rgid))) {
    return -1;
  }

  return 0;
}


int
kernel_set_ucred_svgid(int pid, unsigned int svgid) {
  unsigned long ucred = 0;

  if(!(ucred=kernel_get_proc_ucred(pid))) {
    return -1;
  }

  if(kernel_copyin(&svgid, ucred + KERNEL_OFFSET_UCRED_CR_SVGID,
		   sizeof(svgid))) {
    return -1;
  }

  return 0;
}

unsigned long
kernel_get_proc_rootdir(int pid) {
  unsigned long filedesc = 0;
  unsigned long vnode = 0;

  if(!(filedesc=kernel_get_proc_filedesc(pid))) {
    return 0;
  }

  if(kernel_copyout(filedesc + KERNEL_OFFSET_FILEDESC_FD_RDIR, &vnode,
		    sizeof(vnode))) {
    return 0;
  }

  return vnode;
}


unsigned long
kernel_get_proc_jaildir(int pid) {
  unsigned long filedesc = 0;
  unsigned long vnode = 0;

  if(!(filedesc=kernel_get_proc_filedesc(pid))) {
    return 0;
  }

  if(kernel_copyout(filedesc + KERNEL_OFFSET_FILEDESC_FD_JDIR, &vnode,
		    sizeof(vnode))) {
    return 0;
  }

  return vnode;
}

unsigned long
kernel_get_proc_currdir(int pid) {
  unsigned long filedesc = 0;
  unsigned long vnode = 0;

  if(!(filedesc=kernel_get_proc_filedesc(pid))) {
    return 0;
  }

  if(kernel_copyout(filedesc + KERNEL_OFFSET_FILEDESC_FD_CDIR, &vnode,
		    sizeof(vnode))) {
    return 0;
  }

  return vnode;
}


int
kernel_set_proc_rootdir(int pid, unsigned long vnode) {
  unsigned long filedesc = 0;

  if(!(filedesc=kernel_get_proc_filedesc(pid))) {
    return 0;
  }

  if(kernel_copyin(&vnode, filedesc + KERNEL_OFFSET_FILEDESC_FD_RDIR,
		   sizeof(vnode))) {
    return -1;
  }

  return 0;
}


int
kernel_set_proc_jaildir(int pid, unsigned long vnode) {
  unsigned long filedesc = 0;

  if(!(filedesc=kernel_get_proc_filedesc(pid))) {
    return 0;
  }

  if(kernel_copyin(&vnode, filedesc + KERNEL_OFFSET_FILEDESC_FD_JDIR,
		   sizeof(vnode))) {
    return -1;
  }

  return 0;
}


int
kernel_set_proc_currdir(int pid, unsigned long vnode) {
  unsigned long filedesc = 0;

  if(!(filedesc=kernel_get_proc_filedesc(pid))) {
    return 0;
  }

  if(kernel_copyin(&vnode, filedesc + KERNEL_OFFSET_FILEDESC_FD_CDIR,
		   sizeof(vnode))) {
    return -1;
  }

  return 0;
}


int
kernel_get_qaflags(unsigned char qaflags[16]) {
  return kernel_copyout(KERNEL_ADDRESS_QA_FLAGS, qaflags, 16);
}


int
kernel_set_qaflags(const unsigned char qaflags[16]) {
  return kernel_copyin(qaflags, KERNEL_ADDRESS_QA_FLAGS, 16);
}
