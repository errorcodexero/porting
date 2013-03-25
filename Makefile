
CC := gcc
CXX := g++
CINCLUDES := -I./ -I./WPILib/ -I./stubs/ -I./Commands -I./Subsystems -I./Extensions
CDEBUG :=
CFLAGS := -Wall -Werror -g -Oo $(CINCLUDES) $(CDEBUG)
CXXFLAGS := -Wall -Wno-deprecated -Wno-ctor-dtor-privacy -Werror -g $(CINCLUDES) $(CDEBUG)

SOURCES := \
	robot_main.c \
	Robot.cpp \
	OI.cpp \
	Commands/AimTrim.cpp \
	Commands/AutoCommand.cpp \
	Commands/BlinkyBreathe.cpp \
	Commands/BlinkyOff.cpp \
	Commands/BlinkyOn.cpp \
	Commands/ClimbManual.cpp \
	Commands/DriveCommand.cpp \
	Commands/ResetRobot.cpp \
	Commands/Rotate.cpp \
	Commands/SelectTarget.cpp \
	Commands/ShootCommand.cpp \
	Commands/ShootManual.cpp \
	Commands/TargetCommand.cpp \
	Commands/TiltCommand.cpp \
	Commands/TimedDrive.cpp \
	Subsystems/BlinkyLight.cpp \
	Subsystems/Climber.cpp \
	Subsystems/DriveBase.cpp \
	Subsystems/Shooter.cpp \
	Extensions/RateGyro.cpp \
	Extensions/RobotDrive3.cpp \
	Extensions/TripleSolenoid.cpp

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

LIBS := WPILib/WPILib.a stubs/stubs.a
LDLIBS := $(LIBS) -lpthread -lrt -lz

.PHONY: default all WPILib v2lin stubs clean allclean

default: robot.exe

all: WPILib stubs robot.exe

WPILib WPILib/WPILib.a:
	$(MAKE) -C WPILib

stubs stubs/stubs.a:
	$(MAKE) -C stubs

robot.exe: $(OBJECTS) $(LIBS)
	$(CXX) -o $@ $(CFLAGS) $(OBJECTS) $(LIBS) $(LDLIBS)

clean:
	$(RM) $(OBJECTS) robot.exe errorlog undefined

allclean: clean
	$(MAKE) -C WPILib clean
	$(MAKE) -C stubs clean

