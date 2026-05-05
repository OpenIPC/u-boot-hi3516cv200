#
# (C) Copyright 2002
# Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#
PLATFORM_RELFLAGS += -fno-strict-aliasing -fno-common -ffixed-r8
#		     -msoft-float

# Hi3518EV200 / Hi3516CV200 are Cortex-A7 (ARMv7-A). The historical
# armv5te downgrade was for old toolchains that didn't ship an armv7
# multilib; the OpenIPC arm-hisiv510-linux toolchain does, so match the
# silicon and let the linker find libgcc.
PLATFORM_RELFLAGS += -fno-aggressive-loop-optimizations
PLATFORM_CPPFLAGS += -march=armv7-a
PLATFORM_CPPFLAGS += -mno-unaligned-access
# =========================================================================
#
# Supply options according to compiler version
#
# =========================================================================
PLATFORM_RELFLAGS +=$(call cc-option,-mshort-load-bytes,\
		    $(call cc-option,-malignment-traps,))
