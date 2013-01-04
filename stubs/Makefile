CC := gcc-3
CXX := g++-3
CINCLUDES := -I./ -I../WPILib/ -I../stubs/ -I../v2lin/lib/
CFLAGS := -Wall -Werror -g $(CINCLUDES)
CXXFLAGS := -Wall -Wno-deprecated -Werror -g $(CINCLUDES)
LDLIBS = -lpthread -ldl -lrt -lm

SOURCES := chipStubs.cpp netStubs.cpp vxStubs.cpp visionStubs.cpp vxSem.c

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

.PHONY: all clean 

all: stubs.a

stubs.a: $(OBJECTS)
	$(AR) crus stubs.a $(OBJECTS)

clean:
	$(RM) $(OBJECTS) stubs.a
