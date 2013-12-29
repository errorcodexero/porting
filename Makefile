CC := gcc
CXX := g++
CINCLUDES := -I./ -I./WPILib/ -I./stubs/ 
CDEBUG :=
OI := -DPRACTICE_OI

PROGRAMS := BuiltinDefaultCode bunny CommandBasedRobotTemplate DriverStationLCDText holo3 SimpleTemplate

##### the rest is generic #####

LIBS := WPILib/WPILib.a stubs/stubs.a
LDLIBS := -lpthread -lrt -lz

.PHONY: default all WPILib stubs clean allclean

all: WPILib stubs $(PROGRAMS)

CFLAGS = -Wall -Werror -g -O0 $(CINCLUDES) $(CDEBUG) $(OI)
CXXFLAGS = -Wall -Wno-deprecated -Wno-ctor-dtor-privacy -Wno-delete-non-virtual-dtor -Werror -g $(CINCLUDES) $(CDEBUG) $(OI)

define PROGRAM_template =
$(1)_SOURCES := robot_main.cpp $(shell find $(1) '(' -name '*.c' -o -name '*.cpp' ')' -print)
$(1)_OBJECTS := $$(patsubst %.c,%.o,$$(patsubst %.cpp,%.o,$$($(1)_SOURCES)))
$(1)_CINCLUDES := $$(patsubst %,-I%,$$(sort $$(dir $$($(1)_SOURCES))))
.PHONY: $(1)
$(1) : $(1).exe
$(1).exe : CINCLUDES += $$($(1)_CINCLUDES)
$(1).exe : WPILib stubs $$($(1)_OBJECTS) $(LIBS)
	$(CXX) -o $(1).exe $$(CXXFLAGS) $$($(1)_OBJECTS) $(LIBS) $(LDLIBS)
CLEAN_TARGETS += $(1).exe $$($(1)_OBJECTS)
endef

$(foreach prog,$(PROGRAMS),$(eval $(call PROGRAM_template,$(prog))))

WPILib WPILib/WPILib.a:
	$(MAKE) -C WPILib

stubs stubs/stubs.a:
	$(MAKE) -C stubs

clean:
	$(RM) $(CLEAN_TARGETS) errorlog undefined

allclean: clean
	$(MAKE) -C WPILib clean
	$(MAKE) -C stubs clean

