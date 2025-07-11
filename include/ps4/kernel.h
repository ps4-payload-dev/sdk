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

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

extern const size_t KERNEL_IMAGE_SIZE;

extern const intptr_t KERNEL_ADDRESS_IMAGE_BASE;
extern const intptr_t KERNEL_ADDRESS_ALLPROC;
extern const intptr_t KERNEL_ADDRESS_PRISON0;
extern const intptr_t KERNEL_ADDRESS_ROOTVNODE;
extern const intptr_t KERNEL_ADDRESS_TARGETID;
extern const intptr_t KERNEL_ADDRESS_QA_FLAGS;

uint32_t kernel_get_fw_version(void);
ssize_t  kernel_get_image_size(void);

int32_t  kernel_copyin(const void *udaddr, intptr_t kaddr, size_t len);
int32_t  kernel_copyout(intptr_t kaddr, void *udaddr, size_t  len);

int32_t kernel_setlong(intptr_t addr, uint64_t val);
int32_t kernel_setint(intptr_t addr, uint32_t val);
int32_t kernel_setshort(intptr_t addr, uint16_t val);
int32_t kernel_setchar(intptr_t addr, uint8_t val);

uint64_t kernel_getlong(intptr_t addr);
uint32_t kernel_getint(intptr_t addr);
uint16_t kernel_getshort(intptr_t addr);
uint8_t  kernel_getchar(intptr_t addr);

intptr_t kernel_find_pattern(intptr_t addr, uint32_t len, const char* pattern);
int kernel_patch(intptr_t addr, const void* before, const void* after,
		 uint16_t size);

intptr_t kernel_get_proc(pid_t pid);
intptr_t kernel_get_proc_ucred(pid_t pid);
intptr_t kernel_get_proc_filedesc(pid_t pid);

intptr_t kernel_dynlib_mapbase_addr(pid_t pid, uint32_t handle);
intptr_t kernel_dynlib_entry_addr(pid_t pid, uint32_t handle);

int32_t kernel_set_ucred_uid(pid_t pid, uid_t uid);
int32_t kernel_set_ucred_ruid(pid_t pid, uid_t ruid);
int32_t kernel_set_ucred_svuid(pid_t pid, uid_t svuid);

int32_t kernel_set_ucred_rgid(pid_t pid, gid_t rgid);
int32_t kernel_set_ucred_svgid(pid_t pid, gid_t svgid);

intptr_t kernel_get_ucred_prison(int pid);
int32_t  kernel_set_ucred_prison(int pid, intptr_t prison);

uint64_t kernel_get_ucred_authid(pid_t pid);
int32_t  kernel_set_ucred_authid(pid_t pid, uint64_t authid);

int32_t kernel_get_ucred_caps(pid_t pid, uint8_t caps[16]);
int32_t kernel_set_ucred_caps(pid_t pid, const uint8_t caps[16]);

uint64_t kernel_get_ucred_attrs(pid_t pid);
int32_t  kernel_set_ucred_attrs(pid_t pid, uint64_t attr);

intptr_t kernel_get_proc_rootdir(pid_t pid);
int32_t  kernel_set_proc_rootdir(pid_t pid, intptr_t vnode);

intptr_t kernel_get_proc_jaildir(pid_t pid);
int32_t  kernel_set_proc_jaildir(pid_t pid, intptr_t vnode);

intptr_t kernel_get_proc_currdir(pid_t pid);
int32_t  kernel_set_proc_currdir(pid_t pid, intptr_t vnode);

int32_t kernel_get_qaflags(uint8_t qaflags[16]);
int32_t kernel_set_qaflags(uint8_t const qaflags[16]);

__END_DECLS

