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

#include "kernel.h"
#include "klog.h"
#include "patch.h"


/**
 * Patch kernel so payloads can mmap SELF files for decryption.
 **/
static int
patch_mmap(void) {
  unsigned int fw = kernel_get_fw_version() >> 16;
  unsigned int patch1 = 0;
  unsigned int patch2 = 0;
  unsigned int patch3 = 0;
  int err = 0;

  switch(fw) {
  case 0x350:
    patch1 = 0x00394BF0;
    patch2 = 0x00394C00;
    patch3 = 0x0034AF55;
    break;
  case 0x355:
    patch1 = 0x00395020;
    patch2 = 0x00395030;
    patch3 = 0x0034B385;
    break;
  case 0x370:
    patch1 = 0x003950F0;
    patch2 = 0x00395100;
    patch3 = 0x0034B455;
    break;
  case 0x401:
    patch1 = 0x003604C0;
    patch2 = 0x003604D0;
    patch3 = 0x0031ED07;
    break;
  case 0x405:
    patch1 = 0x003605F0;
    patch2 = 0x00360600;
    patch3 = 0x0031EE37;
    break;
  case 0x406:
    patch1 = 0x00360600;
    patch2 = 0x00360610;
    patch3 = 0x0031EE47;
    break;
  case 0x407:
    patch1 = 0x00360600;
    patch2 = 0x00360610;
    patch3 = 0x0031EE47;
    break;
  case 0x450:
    patch1 = 0x0016A5B0;
    patch2 = 0x0016A5C0;
    patch3 = 0x00143BE7;
    break;
  case 0x455:
    patch1 = 0x0016A5B0;
    patch2 = 0x0016A5C0;
    patch3 = 0x00143BE7;
    break;
  case 0x470:
    patch1 = 0x00169810;
    patch2 = 0x00169820;
    patch3 = 0x00143277;
    break;
  case 0x471:
    patch1 = 0x00169810;
    patch2 = 0x00169820;
    patch3 = 0x00143277;
    break;
  case 0x472:
    patch1 = 0x00169810;
    patch2 = 0x00169820;
    patch3 = 0x00143277;
    break;
  case 0x473:
    patch1 = 0x00169810;
    patch2 = 0x00169820;
    patch3 = 0x00143277;
    break;
  case 0x474:
    patch1 = 0x00169810;
    patch2 = 0x00169820;
    patch3 = 0x00143277;
    break;
  case 0x500:
    patch1 = 0x000117B0;
    patch2 = 0x000117C0;
    patch3 = 0x0013EF2F;
    break;
  case 0x501:
    patch1 = 0x000117B0;
    patch2 = 0x000117C0;
    patch3 = 0x0013EF2F;
    break;
  case 0x503:
    patch1 = 0x000117B0;
    patch2 = 0x000117C0;
    patch3 = 0x0013F03F;
    break;
  case 0x505:
    patch1 = 0x000117B0;
    patch2 = 0x000117C0;
    patch3 = 0x0013F03F;
    break;
  case 0x507:
    patch1 = 0x000117B0;
    patch2 = 0x000117C0;
    patch3 = 0x0013F03F;
    break;
  case 0x550:
    patch1 = 0x001B4990;
    patch2 = 0x001B49A0;
    patch3 = 0x003C3FBD;
    break;
  case 0x553:
    patch1 = 0x001B4890;
    patch2 = 0x001B48A0;
    patch3 = 0x003C3EBD;
    break;
  case 0x555:
    patch1 = 0x001B4C00;
    patch2 = 0x001B4C10;
    patch3 = 0x003C427D;
    break;
  case 0x556:
    patch1 = 0x001B4C00;
    patch2 = 0x001B4C10;
    patch3 = 0x003C427D;
    break;
  case 0x600:
    patch1 = 0x00459490;
    patch2 = 0x004594A0;
    patch3 = 0x002420A7;
    break;
  case 0x602:
    patch1 = 0x00459490;
    patch2 = 0x004594A0;
    patch3 = 0x002420A7;
    break;
  case 0x620:
    patch1 = 0x004594B0;
    patch2 = 0x004594C0;
    patch3 = 0x002420A7;
    break;
  case 0x650:
    patch1 = 0x00233890;
    patch2 = 0x002338A0;
    patch3 = 0x000AD2E4;
    break;
  case 0x651:
    patch1 = 0x00233890;
    patch2 = 0x002338A0;
    patch3 = 0x000AD2E4;
    break;
  case 0x670:
    patch1 = 0x00233C40;
    patch2 = 0x00233C50;
    patch3 = 0x000AD2E4;
    break;
  case 0x671:
    patch1 = 0x00233C40;
    patch2 = 0x00233C50;
    patch3 = 0x000AD2E4;
    break;
  case 0x672:
    patch1 = 0x00233C40;
    patch2 = 0x00233C50;
    patch3 = 0x000AD2E4;
    break;
  case 0x700:
    patch1 = 0x001CB8F0;
    patch2 = 0x001CB910;
    patch3 = 0x001D40BB;
    break;
  case 0x701:
    patch1 = 0x001CB8F0;
    patch2 = 0x001CB910;
    patch3 = 0x001D40BB;
    break;
  case 0x702:
    patch1 = 0x001CB8F0;
    patch2 = 0x001CB910;
    patch3 = 0x001D40BB;
    break;
  case 0x750:
    patch1 = 0x00364D40;
    patch2 = 0x00364D60;
    patch3 = 0x000DCED1;
    break;
  case 0x751:
    patch1 = 0x00364D40;
    patch2 = 0x00364D60;
    patch3 = 0x000DCED1;
    break;
  case 0x755:
    patch1 = 0x00364D40;
    patch2 = 0x00364D60;
    patch3 = 0x000DCED1;
    break;
  case 0x800:
    patch1 = 0x001D5780;
    patch2 = 0x001D57A0;
    patch3 = 0x000FED61;
    break;
  case 0x801:
    patch1 = 0x001D5780;
    patch2 = 0x001D57A0;
    patch3 = 0x000FED61;
    break;
  case 0x803:
    patch1 = 0x001D5780;
    patch2 = 0x001D57A0;
    patch3 = 0x000FED61;
    break;
  case 0x850:
    patch1 = 0x00293650;
    patch2 = 0x00293670;
    patch3 = 0x00084411;
    break;
  case 0x852:
    patch1 = 0x00293650;
    patch2 = 0x00293670;
    patch3 = 0x00084411;
    break;
  case 0x900:
    patch1 = 0x0008BC90;
    patch2 = 0x0008BCB0;
    patch3 = 0x00168051;
    break;
  case 0x903:
    patch1 = 0x0008BC90;
    patch2 = 0x0008BCB0;
    patch3 = 0x00168001;
    break;
  case 0x904:
    patch1 = 0x0008BC90;
    patch2 = 0x0008BCB0;
    patch3 = 0x00168001;
    break;
  case 0x950:
    patch1 = 0x00032600;
    patch2 = 0x00032620;
    patch3 = 0x00124AA1;
    break;
  case 0x951:
    patch1 = 0x00032600;
    patch2 = 0x00032620;
    patch3 = 0x00124AA1;
    break;
  case 0x960:
    patch1 = 0x00032600;
    patch2 = 0x00032620;
    patch3 = 0x00124AA1;
    break;
  case 0x1000:
    patch1 = 0x000A5CD0;
    patch2 = 0x000A5CF0;
    patch3 = 0x000EF2C1;
    break;
  case 0x1001:
    patch1 = 0x000A5CD0;
    patch2 = 0x000A5CF0;
    patch3 = 0x000EF2C1;
    break;
  case 0x1050:
    patch1 = 0x001F44E0;
    patch2 = 0x001F4500;
    patch3 = 0x0019E151;
    break;
  case 0x1070:
    patch1 = 0x001F44E0;
    patch2 = 0x001F4500;
    patch3 = 0x0019E151;
    break;
  case 0x1071:
    patch1 = 0x001F44E0;
    patch2 = 0x001F4500;
    patch3 = 0x0019E151;
    break;
  case 0x1100:
    patch1 = 0x003D0E50;
    patch2 = 0x003D0E70;
    patch3 = 0x00157F91;
    break;
  case 0x1102:
    patch1 = 0x003D0E70;
    patch2 = 0x003D0E90;
    patch3 = 0x00157FB1;
    break;
  case 0x1150:
    patch1 = 0x003B2B00;
    patch2 = 0x003B2B20;
    patch3 = 0x001FC361;
    break;
  case 0x1152:
    patch1 = 0x003B2B00;
    patch2 = 0x003B2B20;
    patch3 = 0x001FC361;
    break;
  case 0x1200:
    patch1 = 0x003B2D40;
    patch2 = 0x003B2D60;
    patch3 = 0x001FC441;
    break;
  case 0x1202:
    patch1 = 0x003B2D40;
    patch2 = 0x003B2D60;
    patch3 = 0x001FC441;
    break;
  case 0x1250:
    patch1 = 0x003B2D80;
    patch2 = 0x003B2DA0;
    patch3 = 0x001FC481;
    break;
  case 0x1252:
    patch1 = 0x003B2D80;
    patch2 = 0x003B2DA0;
    patch3 = 0x001FC481;
    break;
  case 0x1300:
    patch1 = 0x003B2DA0;
    patch2 = 0x003B2DC0;
    patch3 = 0x001FC4A1;
    break;
  case 0x1302:
    patch1 = 0x003B2DB0;
    patch2 = 0x003B2DD0;
    patch3 = 0x001FC4B1;
    break;
  case 0x1304:
    patch1 = 0x003B2DB0;
    patch2 = 0x003B2DD0;
    patch3 = 0x001FC4B1;
    break;
  case 0x1350:
    patch1 = 0x003B31F0;
    patch2 = 0x003B3210;
    patch3 = 0x001FC4C1;
    break;
  default:
    klog_printf("Unsupported firmware %x\n", fw);
  }

  if(patch1) {
    if(kernel_patch(KERNEL_ADDRESS_IMAGE_BASE + patch1,
		    0, "\xb8\x01\x00\x00\x00\xc3", 6)) {
      err = -1;
    }
  }
  if(patch2) {
    if(kernel_patch(KERNEL_ADDRESS_IMAGE_BASE + patch2,
		    0, "\xb8\x01\x00\x00\x00\xc3", 6)) {
      err = -1;
    }
  }
  if(patch3) {
    if(kernel_patch(KERNEL_ADDRESS_IMAGE_BASE + patch3,
		    0, "\x31\xc0\xeb\x01", 4)) {
      err = -1;
    }
  }

  return err;
}


int
__patch_init(void) {
  return patch_mmap();
}

