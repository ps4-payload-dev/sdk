#!/usr/bin/env bash
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

PS4_PAYLOAD_SDK="${BASH_SOURCE[0]}"
PS4_PAYLOAD_SDK="$(dirname "${PS4_PAYLOAD_SDK}")"
PS4_PAYLOAD_SDK="${PS4_PAYLOAD_SDK}/.."
PS4_PAYLOAD_SDK="$(realpath "${PS4_PAYLOAD_SDK}")"

export PS4_PAYLOAD_SDK=${PS4_PAYLOAD_SDK}
export PS4_SYSROOT=${PS4_PAYLOAD_SDK}/target
export PS4_HBROOT=/user/homebrew

export DESTDIR="${PS4_SYSROOT}"
export PREFIX="${PS4_HBROOT}"

export PS4_DEPLOY=${PS4_PAYLOAD_SDK}/bin/orbis-deploy
export PS4_CROSS_FIX_ROOT=${PS4_PAYLOAD_SDK}/bin/orbis-cross-fix-root

export AS=${PS4_PAYLOAD_SDK}/bin/orbis-as
export CC=${PS4_PAYLOAD_SDK}/bin/orbis-clang
export CXX=${PS4_PAYLOAD_SDK}/bin/orbis-clang++
export LD=${PS4_PAYLOAD_SDK}/bin/orbis-lld

export AR=${PS4_PAYLOAD_SDK}/bin/orbis-ar
export NM=${PS4_PAYLOAD_SDK}/bin/orbis-nm
export OBJCOPY=${PS4_PAYLOAD_SDK}/bin/orbis-objcopy
export RANLIB=${PS4_PAYLOAD_SDK}/bin/orbis-ranlib
export STRIP=${PS4_PAYLOAD_SDK}/bin/orbis-strip
export STRINGS=strings

export MAKE=make
export CMAKE=${PS4_PAYLOAD_SDK}/bin/orbis-cmake
export MESON=${PS4_PAYLOAD_SDK}/bin/orbis-meson
export PKG_CONFIG=${PS4_PAYLOAD_SDK}/bin/orbis-pkg-config

