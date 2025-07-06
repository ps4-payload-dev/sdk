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

#include <sys/types.h>
#include <sys/user.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>


static char *state_abbrev[] = {
  "", "START", "RUN\0\0\0", "SLEEP", "STOP", "ZOMB", "WAIT", "LOCK"
};


int
main() {
  int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PROC, 0};
  size_t buf_size;
  void *buf;

  // determine size of query response
  if(sysctl(mib, 4, NULL, &buf_size, NULL, 0)) {
    perror("sysctl");
    return -1;
  }

  // allocate memory for query response
  if(!(buf=malloc(buf_size))) {
    perror("malloc");
    return -1;
  }

  // query the kernel for proc info
  if(sysctl(mib, 4, buf, &buf_size, NULL, 0)) {
    perror("sysctl");
    free(buf);
    return -1;
  }

  printf("\n     PID      PPID     PGID      SID      UID    "
        "  State    Command\n");
  for(void *ptr=buf; ptr<(buf+buf_size);) {
    struct kinfo_proc *ki = (struct kinfo_proc*)ptr;
    ptr += ki->ki_structsize;

    printf("%8u  %8u %8u %8u %8u %10s  %s\n",
	   ki->ki_pid, ki->ki_ppid, ki->ki_pgid, ki->ki_sid,
	   ki->ki_uid,
           state_abbrev[(int)ki->ki_stat], ki->ki_comm);
  }

  free(buf);

  return 0;
}

