
CC := gcc-3
CXX := g++-3
CINCLUDES := -I./ -I./WPILib/ -I./stubs/ -I./v2lin/lib/
CFLAGS := -Wall -Werror -g $(CINCLUDES)
CXXFLAGS := -Wall -Wno-deprecated -Werror -g $(CINCLUDES)

SOURCES := \
	MyRobot.cpp \
	robot_main.c

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

LIBS := WPILib/WPILib.a v2lin/lib/libv2lin.a stubs/stubs.a
LDLIBS := $(LIBS) -pthread -lrt -lz

.PHONY: all force WPILib v2lin stubs clean clobber

all: MyRobot.exe
force: WPILib libv2lin stubs MyRobot.exe

MyRobot.exe: $(OBJECTS) $(LIBS)
	@echo $(CXX) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS) $(LDLIBS)
	@if ! $(CXX) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS) $(LDLIBS) >& errorlog; then \
	    echo `wc -l < errorlog` errors reported, see \"errorlog\"; \
	    sed -n -e '/undefined/s/.*`\(.*\)'\''.*/\1/p' < errorlog | sort -u > undefined; \
	    echo `wc -l < undefined` undefined symbols, see \"undefined\"; \
	    exit 1; \
	fi

WPILib WPILib/WPILib.a:
	$(MAKE) -C WPILib

libv2lin v2lin/lib/libv2lin.a:
	$(MAKE) -C v2lin

stubs stubs/stubs.a:
	$(MAKE) -C stubs

clean:
	$(RM) $(OBJECTS) MyRobot.exe errorlog undefined

clobber: clean
	$(MAKE) -C WPILib clean
	$(MAKE) -C v2lin clean
	$(MAKE) -C stubs clean

