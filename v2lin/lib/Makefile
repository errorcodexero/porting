#  Copyright (C) 2004, 2005, 2006 v2lin Team <http://v2lin.sf.net>
#
#  This file is part of the v2lin Library.
#  VxWorks is a registered trademark of Wind River Systems, Inc.
#
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

LIB_SHORT = v2lin
LIB_STATIC = lib$(LIB_SHORT).a
LIB_SHARED = lib$(LIB_SHORT).dll
LIB_MAIN = lib$(LIB_SHORT)main.a

OBJECTS = lkernelLib.o ltaskLib.o lmsgQLib.o lsemLib.o lwdLib.o v2ldebug.o v2ltime.o

TARGETS = $(LIB_STATIC) $(LIB_SHARED) $(LIB_MAIN)

.PHONY: all show

all: .deps.mk $(TARGETS)

$(LIB_STATIC): $(OBJECTS)
	ar rcus $@ $^

$(LIB_SHARED): $(OBJECTS)
	$(CC) --shared -Wl,--out-implib,$@.a -o $@ $^

$(LIB_MAIN): main_impl.o
	ar rcus $@ $^

show:
	@echo LIB_STATIC $(LIB_STATIC)
	@echo LIB_SHARED $(LIB_SHARED)
	@echo LIB_MAIN $(LIB_MAIN)

include ../rules.mk

