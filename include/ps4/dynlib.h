#pragma once

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

#include <stdint.h>
#include <stddef.h>

/**
 * an ELF loaded in kernel memory.
 **/
typedef struct dynlib_obj {
  uintptr_t next;
  uintptr_t path;
  uint64_t unknown0[2];
  uint32_t refcount;
  uint64_t handle;

  uintptr_t mapbase;
  uintptr_t mapsize;
  uintptr_t textsize;

  uintptr_t database;
  size_t datasize;

  uint64_t unknown1;
  uint64_t unknown1size;

  uintptr_t vaddrbase;
  uintptr_t relocbase;
  uintptr_t entry;

  uint32_t tlsindex;
  uintptr_t tlsinit;
  uintptr_t tlsinitsize;
  size_t tlssize;
  uint64_t tlsoffset;
  uint64_t tlsalign;

  uintptr_t pltgot;

  uint64_t unknown2[7];

  uintptr_t init;
  uintptr_t fini;

  uintptr_t eh_frame_hdr;
  size_t eh_frame_hdr_size;

  uintptr_t eh_frame;
  size_t eh_frame_size;

  int32_t status;
  int32_t flags;

  uint64_t unknown5[5];

  uintptr_t dynsec;
  uint64_t unknown6[6]; //fingerprint?
} dynlib_obj_t;
