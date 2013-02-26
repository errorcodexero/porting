
CC := gcc
CXX := g++
CINCLUDES := -I./ -I./WPILib/ -I./stubs/
CDEBUG :=
CFLAGS := -Wall -Werror -g -Oo $(CINCLUDES) $(CDEBUG)
CXXFLAGS := -Wall -Wno-deprecated -Wno-ctor-dtor-privacy -Werror -g $(CINCLUDES) $(CDEBUG)

SOURCES := \
	MyRobot.cpp \
	robot_main.c

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

LIBS := WPILib/WPILib.a stubs/stubs.a
LDLIBS := $(LIBS) -pthread -lrt -lz

.PHONY: default all WPILib v2lin stubs clean allclean

default: MyRobot.exe

all: WPILib stubs MyRobot.exe

WPILib WPILib/WPILib.a:
	$(MAKE) -C WPILib

stubs stubs/stubs.a:
	$(MAKE) -C stubs

MyRobot.exe: $(OBJECTS) $(LIBS)
	$(CXX) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS) $(LDLIBS)

clean:
	$(RM) $(OBJECTS) MyRobot.exe errorlog undefined

allclean: clean
	$(MAKE) -C WPILib clean
	$(MAKE) -C stubs clean

