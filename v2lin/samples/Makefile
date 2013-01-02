#  Copyright (C) 2004, 2005, 2006 v2lin Team <http://v2lin.sf.net>
#
#  This file is part of the v2lin Library.
#  VxWorks is a registered trademark of Wind River Systems, Inc.
#
#  Initial implementation Gary S. Robertson, 2000, 2001.
#  Contributed by Andrew Skiba, skibochka@sourceforge.net, 2004.
#  Contributed by Mike Kemelmakher, mike@ubxess.com, 2005.
#  Contributed by Constantine Shulyupin, conan.sh@gmail.com, 2006.
#
#  The v2lin library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  The v2lin Library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.

top=..
include $(top)/defs.mk
include defs.mk

CFLAGS += -I$(top)/lib
LDLIBS += -L$(top)/lib -lv2linmain -lv2lin -lpthread -ldl -lrt
TARGETS = with_sysinit

all: $(TARGETS)
	$(MAKE) -C with_main
	$(MAKE) -C shared_library

include $(top)/rules.mk

clean:
	$(MAKE) common_clean
	$(MAKE) -C with_main clean
	$(MAKE) -C shared_library clean
