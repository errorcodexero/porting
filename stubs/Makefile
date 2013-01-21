CC := gcc
CXX := g++
CINCLUDES := -I./ -I../WPILib/ -I../stubs/ -I../v2lin/lib/
CDEBUG :=
CFLAGS := -Wall -Werror -g -O0 $(CINCLUDES) $(CDEBUG)
CXXFLAGS := -Wall -Wno-deprecated -Werror -g -O0 $(CINCLUDES) $(CDEBUG)
LDLIBS = -lpthread -ldl -lrt -lm

SOURCES := chipStubs.cpp netStubs.cpp vxStubs.cpp visionStubs.cpp vxSem.c

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

.PHONY: all clean 

all: stubs.a

stubs.a: $(OBJECTS)
	$(AR) crus stubs.a $(OBJECTS)

clean:
	$(RM) $(OBJECTS) stubs.a
