// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "DriveBase.h"

DriveBase::DriveBase( int frontLeftMotorChannel,
		      int backLeftMotorChannel,
		      int frontRightMotorChannel,
		      int backRightMotorChannel,
		      int gyroAnalogChannel )
    : Subsystem("DriveBase"),
    m_front_left(NULL),
    m_back_left(NULL),
    m_front_right(NULL),
    m_back_right(NULL),
    m_drive(NULL),
    m_gyro(NULL),
    m_defaultCommand(NULL),
    m_started(false)
{
    LiveWindow *lw = LiveWindow::GetInstance();

    m_front_left  = new Victor(frontLeftMotorChannel);
    lw->AddActuator("DriveBase", "FrontLeft", dynamic_cast<Victor*>(m_front_left));
    m_back_left  = new Victor(backLeftMotorChannel);
    lw->AddActuator("DriveBase", "BackLeft", dynamic_cast<Victor*>(m_back_left));
    m_front_right  = new Victor(frontRightMotorChannel);
    lw->AddActuator("DriveBase", "FrontRight", dynamic_cast<Victor*>(m_front_right));
    m_back_right  = new Victor(backRightMotorChannel);
    lw->AddActuator("DriveBase", "BackRight", dynamic_cast<Victor*>(m_back_right));

    m_drive = new RobotDrive(m_front_left, m_back_left, m_front_right, m_back_right);
    m_drive->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
    m_drive->SetInvertedMotor(RobotDrive::kRearRightMotor, true);

    m_gyro = new RateGyro(1, gyroAnalogChannel);
    lw->AddSensor("DriveBase", "Gyro", m_gyro);

    Stop();
}

DriveBase::~DriveBase()
{
    SetDefaultCommand(NULL);
    Scheduler::GetInstance()->Remove(m_defaultCommand);
    delete m_defaultCommand;
    m_defaultCommand = NULL;

    Stop();

    delete m_gyro;
    delete m_drive;
    delete m_back_right;
    delete m_front_right;
    delete m_back_left;
    delete m_front_left;
}
    
void DriveBase::InitDefaultCommand()
{
    if (!m_defaultCommand) {
	m_defaultCommand = new DriveCommand();
	SetDefaultCommand(m_defaultCommand);
    }
}

void DriveBase::Stop()
{
printf("DriveBase::Stop\n");

    // stop and disable all motors
#if 0
    // Disabling the PWM outputs is confusing because it looks like
    // the controllers are disconnected (blinking).  Just set the
    // outputs to zero instead.
    m_drive->SetLeftRightMotorOutputs(0.0, 0.0);
#else
    m_drive->StopMotor();
#endif
    // watchdogs not needed while stopped
    m_drive->SetSafetyEnabled(false);
    dynamic_cast<MotorSafety*>(m_front_left)->SetSafetyEnabled(false);
    dynamic_cast<MotorSafety*>(m_back_left)->SetSafetyEnabled(false);
    dynamic_cast<MotorSafety*>(m_front_right)->SetSafetyEnabled(false);
    dynamic_cast<MotorSafety*>(m_back_right)->SetSafetyEnabled(false);
    // remember that we're stopped
    m_started = false;
}

void DriveBase::Start()
{
    if (!m_started) {
printf("DriveBase::Start\n");
	// set the watchdog timers to something long enough to
	// avoid panic in the presence of short-term network dropouts
	m_drive->SetExpiration(2.0);
	dynamic_cast<MotorSafety*>(m_front_left)->SetExpiration(2.0);
	dynamic_cast<MotorSafety*>(m_back_left)->SetExpiration(2.0);
	dynamic_cast<MotorSafety*>(m_front_right)->SetExpiration(2.0);
	dynamic_cast<MotorSafety*>(m_back_right)->SetExpiration(2.0);

	// set all motors to 0.0 in order to feed their watchdogs
	m_drive->SetLeftRightMotorOutputs(0.0, 0.0);

	// now enable the watchdogs
	m_drive->SetSafetyEnabled(true);
#if 0 // we don't really need watchdogs on the individual motors
	dynamic_cast<MotorSafety*>(m_front_left)->SetSafetyEnabled(true);
	dynamic_cast<MotorSafety*>(m_back_left)->SetSafetyEnabled(true);
	dynamic_cast<MotorSafety*>(m_front_right)->SetSafetyEnabled(true);
	dynamic_cast<MotorSafety*>(m_back_right)->SetSafetyEnabled(true);
#endif

	// remember that we're started
	m_started = true;
    }
}

void DriveBase::Drive( float x, float y, float twist )
{
    if (!m_started) Start();

    // Reduce the sensitivity to the "twist" control.
    // Add gyro compensation (adjust the "200" for best PID response).
    twist -= m_gyro->GetRate() / 200.;

    // limit the twist range to avoid normalization problems
    if (twist < -1.0) twist = -1.0;
    if (twist > 1.0) twist = 1.0;

    m_drive->MecanumDrive_Cartesian( x, y, twist, 0.0 );
}

