CC := gcc
CXX := g++
CINCLUDES := -I./ -I../WPILib/ -I../stubs/ -I../v2lin/lib/
CFLAGS := -Wall -Werror -g -O0 $(CINCLUDES)
CXXFLAGS := -Wall -Wno-deprecated -Wno-ctor-dtor-privacy -Werror -g -O0 $(CINCLUDES)
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
    HiTechnicColorSensor.cpp \
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
    Talon.cpp \
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
    Buttons/Trigger.cpp \
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
    LiveWindow/LiveWindow.cpp \
    LiveWindow/LiveWindowStatusListener.cpp \
    NetworkTables/NetworkTable.cpp \
    NetworkTables/NetworkTableConnectionListenerAdapter.cpp \
    NetworkTables/NetworkTableKeyListenerAdapter.cpp \
    NetworkTables/NetworkTableListenerAdapter.cpp \
    NetworkTables/NetworkTableMode.cpp \
    NetworkTables/NetworkTableProvider.cpp \
    NetworkTables/NetworkTableSubListenerAdapter.cpp \
    networktables2/AbstractNetworkTableEntryStore.cpp \
    networktables2/client/ClientConnectionAdapter.cpp \
    networktables2/client/ClientConnectionState.cpp \
    networktables2/client/ClientNetworkTableEntryStore.cpp \
    networktables2/client/NetworkTableClient.cpp \
    networktables2/connection/BadMessageException.cpp \
    networktables2/connection/ConnectionMonitorThread.cpp \
    networktables2/connection/DataIOStream.cpp \
    networktables2/connection/NetworkTableConnection.cpp \
    networktables2/NetworkTableEntry.cpp \
    networktables2/NetworkTableNode.cpp \
    networktables2/OutgoingEntryReciever.cpp \
    networktables2/server/NetworkTableServer.cpp \
    networktables2/server/ServerConnectionAdapter.cpp \
    networktables2/server/ServerConnectionList.cpp \
    networktables2/server/ServerConnectionState.cpp \
    networktables2/server/ServerIncomingStreamMonitor.cpp \
    networktables2/server/ServerNetworkTableEntryStore.cpp \
    networktables2/stream/FDIOStream.cpp \
    networktables2/stream/SocketServerStreamProvider.cpp \
    networktables2/stream/SocketStreamFactory.cpp \
    networktables2/stream/SocketStreams.cpp \
    networktables2/TableKeyExistsWithDifferentTypeException.cpp \
    networktables2/thread/DefaultThreadManager.cpp \
    networktables2/TransactionDirtier.cpp \
    networktables2/type/ArrayData.cpp \
    networktables2/type/ArrayEntryType.cpp \
    networktables2/type/BooleanArray.cpp \
    networktables2/type/ComplexData.cpp \
    networktables2/type/ComplexEntryType.cpp \
    networktables2/type/DefaultEntryTypes.cpp \
    networktables2/type/NetworkTableEntryType.cpp \
    networktables2/type/NetworkTableEntryTypeManager.cpp \
    networktables2/type/NumberArray.cpp \
    networktables2/type/StringArray.cpp \
    networktables2/util/EOFException.cpp \
    networktables2/util/IllegalStateException.cpp \
    networktables2/util/IOException.cpp \
    networktables2/util/StringCache.cpp \
    networktables2/util/System.cpp \
    networktables2/WriteManager.cpp \
    SmartDashboard/SendableChooser.cpp \
    SmartDashboard/SmartDashboard.cpp \
    tables/TableKeyNotDefinedException.cpp \
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

