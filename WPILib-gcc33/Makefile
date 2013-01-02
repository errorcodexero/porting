CC := gcc-3
CXX := g++-3
CINCLUDES := -I./ -I../WPILib/ -I../stubs/ -I../v2lin/lib/
CFLAGS := -Wall -Werror -g $(CINCLUDES)
CXXFLAGS := -Wall -Wno-deprecated -Werror -g $(CINCLUDES)
LDLIBS = -lpthread -ldl -lrt -lm

SOURCES := \
	Accelerometer.cpp \
	ADXL345_I2C.cpp \
	ADXL345_SPI.cpp \
	AnalogChannel.cpp \
	AnalogModule.cpp \
	AnalogTrigger.cpp \
	AnalogTriggerOutput.cpp \
	CANJaguar.cpp \
	Compressor.cpp \
	Counter.cpp \
	Dashboard.cpp \
	DigitalInput.cpp \
	DigitalModule.cpp \
	DigitalOutput.cpp \
	DigitalSource.cpp \
	DoubleSolenoid.cpp \
	DriverStation.cpp \
	DriverStationEnhancedIO.cpp \
	DriverStationLCD.cpp \
	Encoder.cpp \
	Error.cpp \
	ErrorBase.cpp \
	GearTooth.cpp \
	Gyro.cpp \
	HiTechnicCompass.cpp \
	I2C.cpp \
	InterruptableSensorBase.cpp \
	IterativeRobot.cpp \
	Jaguar.cpp \
	Joystick.cpp \
	Kinect.cpp \
	KinectStick.cpp \
	Module.cpp \
	MotorSafetyHelper.cpp \
	Notifier.cpp \
	PIDController.cpp \
	Preferences.cpp \
	PWM.cpp \
	Relay.cpp \
	Resource.cpp \
	RobotBase.cpp \
	RobotDrive.cpp \
	SafePWM.cpp \
	SensorBase.cpp \
	SerialPort.cpp \
	Servo.cpp \
	SimpleRobot.cpp \
	Solenoid.cpp \
	SolenoidBase.cpp \
	SPI.cpp \
	Synchronized.cpp \
	Task.cpp \
	Timer.cpp \
	Ultrasonic.cpp \
	Utility.cpp \
	Victor.cpp \
	Watchdog.cpp \
	Buttons/AnalogIOButton.cpp \
	Buttons/Button.cpp \
	Buttons/ButtonScheduler.cpp \
	Buttons/DigitalIOButton.cpp \
	Buttons/HeldButtonScheduler.cpp \
	Buttons/InternalButton.cpp \
	Buttons/JoystickButton.cpp \
	Buttons/NetworkButton.cpp \
	Buttons/PressedButtonScheduler.cpp \
	Buttons/ReleasedButtonScheduler.cpp \
	CInterfaces/CAccelerometer.cpp \
	CInterfaces/CAnalogChannel.cpp \
	CInterfaces/CCompressor.cpp \
	CInterfaces/CCounter.cpp \
	CInterfaces/CDigitalInput.cpp \
	CInterfaces/CDigitalOutput.cpp \
	CInterfaces/CDriverStation.cpp \
	CInterfaces/CEncoder.cpp \
	CInterfaces/CGearTooth.cpp \
	CInterfaces/CGyro.cpp \
	CInterfaces/CJaguar.cpp \
	CInterfaces/CJoystick.cpp \
	CInterfaces/CPWM.cpp \
	CInterfaces/CRelay.cpp \
	CInterfaces/CRobotDrive.cpp \
	CInterfaces/CSerialPort.cpp \
	CInterfaces/CServo.cpp \
	CInterfaces/CSolenoid.cpp \
	CInterfaces/CTimer.cpp \
	CInterfaces/CUltrasonic.cpp \
	CInterfaces/CVictor.cpp \
	CInterfaces/CWatchdog.cpp \
	CInterfaces/SimpleCRobot.cpp \
	Commands/Command.cpp \
	Commands/CommandGroup.cpp \
	Commands/CommandGroupEntry.cpp \
	Commands/PIDCommand.cpp \
	Commands/PIDSubsystem.cpp \
	Commands/PrintCommand.cpp \
	Commands/Scheduler.cpp \
	Commands/StartCommand.cpp \
	Commands/Subsystem.cpp \
	Commands/WaitCommand.cpp \
	Commands/WaitForChildren.cpp \
	Commands/WaitUntilCommand.cpp \
	NetworkTables/BooleanEntry.cpp \
	NetworkTables/Buffer.cpp \
	NetworkTables/Confirmation.cpp \
	NetworkTables/Connection.cpp \
	NetworkTables/ConnectionManager.cpp \
	NetworkTables/Data.cpp \
	NetworkTables/Denial.cpp \
	NetworkTables/DoubleEntry.cpp \
	NetworkTables/Entry.cpp \
	NetworkTables/IntegerEntry.cpp \
	NetworkTables/Key.cpp \
	NetworkTables/NetworkQueue.cpp \
	NetworkTables/NetworkTable.cpp \
	NetworkTables/OldData.cpp \
	NetworkTables/Reader.cpp \
	NetworkTables/StringEntry.cpp \
	NetworkTables/TableAssignment.cpp \
	NetworkTables/TableEntry.cpp \
	NetworkTables/TransactionEnd.cpp \
	NetworkTables/TransactionStart.cpp \
	SmartDashboard/SendableChooser.cpp \
	SmartDashboard/SendableGyro.cpp \
	SmartDashboard/SendablePIDController.cpp \
	SmartDashboard/SmartDashboard.cpp \
	Vision/AxisCamera.cpp \
	Vision/AxisCameraParams.cpp \
	Vision/BinaryImage.cpp \
	Vision/ColorImage.cpp \
	Vision/EnumCameraParameter.cpp \
	Vision/HSLImage.cpp \
	Vision/ImageBase.cpp \
	Vision/IntCameraParameter.cpp \
	Vision/MonoImage.cpp \
	Vision/PCVideoServer.cpp \
	Vision/RGBImage.cpp \
	Vision/Threshold.cpp \
	Vision2009/AxisCamera.cpp \
	Vision2009/BaeUtilities.cpp \
	Vision2009/FrcError.cpp \
	Vision2009/TrackAPI.cpp \
	Vision2009/VisionAPI.cpp

OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

.PHONY: all clean 

all: WPILib.a

WPILib.a: $(OBJECTS)
	$(AR) crus WPILib.a $(OBJECTS)

clean:
	$(RM) $(OBJECTS) WPILib.a

