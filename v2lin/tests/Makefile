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

LDLIBS += -L$(top)/lib -lv2linmain -lv2lin -lpthread -ldl -lrt
TARGETS = test test_sem test_time

all: $(TARGETS)

test:	test_tasks.o test_semaphores.o test_msgq.o test_watchdog.o test_mutexes.o

gdb:
	export PATH+=:$(top)/lib/; gdb -q --command run.gdb test
	
run:
	$(MAKE) -C $(top)
	@echo Running test, takes less than 1 min
	export PATH+=:$(top)/lib/; time nice -n 15 ./test 2| tee test.log; echo $$?
	@grep ERROR -w test.log && echo -n "Errors: " && grep ERROR -wc test.log && exit 1 \
		|| echo No errors, Test passed
	@echo Running test_sem, takes about 1 min
	export PATH+=:$(top)/lib/; time ./test_sem | tee test_sem.log
	@grep ERROR -w test_sem.log && echo -n "Errors: " && grep ERROR -wc test_sem.log && exit 1 \
		|| echo No errors, Test passed

include $(top)/rules.mk
