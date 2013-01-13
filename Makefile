
CC := gcc
CXX := g++
CINCLUDES := -I./ -I./WPILib/ -I./stubs/
CFLAGS := -Wall -Werror -g -Oo $(CINCLUDES)
CXXFLAGS := -Wall -Wno-deprecated -Wno-ctor-dtor-privacy -Werror -g $(CINCLUDES)

SOURCES := \
	MyRobot.cpp \
	robot_main.c

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

LIBS := WPILib/WPILib.a stubs/stubs.a
LDLIBS := $(LIBS) -pthread -lrt -lz

.PHONY: default all WPILib v2lin stubs clean clobber

default: MyRobot.exe

all: WPILib stubs MyRobot.exe

WPILib WPILib/WPILib.a:
	$(MAKE) -C WPILib

stubs stubs/stubs.a:
	$(MAKE) -C stubs

MyRobot.exe: $(OBJECTS) $(LIBS)
	@echo $(CXX) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS) $(LDLIBS)
	@if ! $(CXX) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS) $(LDLIBS) >& errorlog; then \
	    echo `wc -l < errorlog` errors reported, see \"errorlog\"; \
	    sed -n -e '/undefined/s/.*`\(.*\)'\''.*/\1/p' < errorlog | sort -u > undefined; \
	    echo `wc -l < undefined` undefined symbols, see \"undefined\"; \
	    exit 1; \
	fi

clean:
	$(RM) $(OBJECTS) MyRobot.exe errorlog undefined

clobber: clean
	$(MAKE) -C WPILib clean
	$(MAKE) -C stubs clean

