#   Copyright (C) 2025 John Törnblom
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING. If not see
# <http://www.gnu.org/licenses/>.

PS4_PAYLOAD_SDK := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)
PS4_SYSROOT     := $(PS4_PAYLOAD_SDK)/target
PS4_HBROOT      := /user/homebrew

DESTDIR ?= $(PS4_SYSROOT)
PREFIX  ?= $(PS4_HBROOT)

PS4_DEPLOY := $(PS4_PAYLOAD_SDK)/bin/orbis-deploy

AS         := $(PS4_PAYLOAD_SDK)/bin/orbis-as
CC         := $(PS4_PAYLOAD_SDK)/bin/orbis-clang
CXX        := $(PS4_PAYLOAD_SDK)/bin/orbis-clang++
LD         := $(PS4_PAYLOAD_SDK)/bin/orbis-lld
PKG_CONFIG := $(PS4_PAYLOAD_SDK)/bin/orbis-pkg-config
AR         := $(PS4_PAYLOAD_SDK)/bin/orbis-ar
NM         := $(PS4_PAYLOAD_SDK)/bin/orbis-nm
OBJCOPY    := $(PS4_PAYLOAD_SDK)/bin/orbis-objcopy
RANLIB     := $(PS4_PAYLOAD_SDK)/bin/orbis-ranlib
STRIP      := $(PS4_PAYLOAD_SDK)/bin/orbis-strip

STRINGS := strings
