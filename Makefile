CC := gcc
CXX := g++
CINCLUDES := -I./ -I./WPILib/ -I./stubs/ 
CDEBUG :=
OI := -DPRACTICE_OI
CFLAGS = -Wall -Werror -g -O0 $(CINCLUDES) $(CDEBUG) $(OI)
CXXFLAGS = -Wall -Wno-deprecated -Wno-ctor-dtor-privacy -Wno-delete-non-virtual-dtor -Werror -g $(CINCLUDES) $(CDEBUG) $(OI)

holo3_SOURCES := \
	robot_main.c \
	holo3/Robot.cpp \
	holo3/OI.cpp \
	holo3/Commands/AimTrim.cpp \
	holo3/Commands/AutoCommand.cpp \
	holo3/Commands/BlinkyBreathe.cpp \
	holo3/Commands/BlinkyOff.cpp \
	holo3/Commands/BlinkyOn.cpp \
	holo3/Commands/ClimbManual.cpp \
	holo3/Commands/DriveCommand.cpp \
	holo3/Commands/ResetRobot.cpp \
	holo3/Commands/Rotate.cpp \
	holo3/Commands/SelectTarget.cpp \
	holo3/Commands/ShootCommand.cpp \
	holo3/Commands/ShootManual.cpp \
	holo3/Commands/TargetCommand.cpp \
	holo3/Commands/TiltCommand.cpp \
	holo3/Commands/TimedDrive.cpp \
	holo3/Subsystems/BlinkyLight.cpp \
	holo3/Subsystems/Climber.cpp \
	holo3/Subsystems/DriveBase.cpp \
	holo3/Subsystems/Shooter.cpp \
	holo3/Extensions/RateGyro.cpp \
	holo3/Extensions/RobotDrive3.cpp \
	holo3/Extensions/TripleSolenoid.cpp

holo3_OBJECTS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(holo3_SOURCES)))

bunny_SOURCES := \
	robot_main.c \
	bunny/OI.cpp \
	bunny/OI.h \
	bunny/Robot.cpp \
	bunny/Robot.h \
	bunny/Commands/AutoCommand.cpp \
	bunny/Commands/AutoCommand.h \
	bunny/Commands/Deploy.cpp \
	bunny/Commands/Deploy.h \
	bunny/Commands/DriveCommand.cpp \
	bunny/Commands/DriveCommand.h \
	bunny/Commands/ResetRobot.cpp \
	bunny/Commands/ResetRobot.h \
	bunny/Commands/Rotate.cpp \
	bunny/Commands/Rotate.h \
	bunny/Commands/SetDriveMode.cpp \
	bunny/Commands/SetDriveMode.h \
	bunny/Commands/TimedDrive.cpp \
	bunny/Commands/TimedDrive.h \
	bunny/Extensions/RateGyro.cpp \
	bunny/Extensions/RateGyro.h \
	bunny/Subsystems/DriveBase.cpp \
	bunny/Subsystems/DriveBase.h

bunny_OBJECTS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(bunny_SOURCES)))

LIBS := WPILib/WPILib.a stubs/stubs.a
LDLIBS := -lpthread -lrt -lz

.PHONY: default all WPILib v2lin stubs clean allclean holo3 bunny

all: WPILib stubs holo3.exe bunny.exe

holo3.exe : TARGET := holo3
holo3.exe : CINCLUDES += -I./holo3 -I./holo3/Commands -I./holo3/Subsystems -I./holo3/Extensions 
holo3.exe : $(holo3_OBJECTS) $(LIBS)
	$(CXX) -o holo3.exe $(CFLAGS) $(holo3_OBJECTS) $(LIBS) $(LDLIBS)

bunny.exe : TARGET=bunny
bunny.exe : CINCLUDES += -I./bunny -I./bunny/Commands -I./bunny/Subsystems -I./bunny/Extensions 
bunny.exe : $(bunny_OBJECTS) $(LIBS)
	$(CXX) -o bunny.exe $(CFLAGS) $(bunny_OBJECTS) $(LIBS) $(LDLIBS)

WPILib WPILib/WPILib.a:
	$(MAKE) -C WPILib

stubs stubs/stubs.a:
	$(MAKE) -C stubs

clean:
	$(RM) $(holo3_OBJECTS) holo3.exe $(bunny_OBJECTS) bunny.exe errorlog undefined

allclean: clean
	$(MAKE) -C WPILib clean
	$(MAKE) -C stubs clean

