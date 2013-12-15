// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include "DriveBase.h"

#if 0
static const char *driveModeName[] = {
    "INITIAL",
    "MECH",
    "TANK",
    "MECHTOTANK_1",
    "MECHTOTANK_2",
    "TANKTOMECH_1",
    "TANKTOMECH_2",
};
#endif

DriveBase::DriveBase( int frontLeftMotorChannel,
		      int backLeftMotorChannel,
		      int frontRightMotorChannel,
		      int backRightMotorChannel,
		      int gyroAnalogChannel,
		      int solenoidChannel,
		      int proximityFrontLeft,
		      int proximityFrontRight,
		      int proximityRightFront,
		      int proximityRightRear,
		      int proximityLeftFront,
		      int proximityLeftRear )
    : Subsystem("DriveBase"),
    m_front_left(NULL),
    m_back_left(NULL),
    m_front_right(NULL),
    m_back_right(NULL),
    m_drive(NULL),
    m_gyro(NULL),
    m_solenoid(NULL),
    m_proximityFrontLeft(NULL),
    m_proximityFrontRight(NULL),
    m_proximityRightFront(NULL),
    m_proximityRightRear(NULL),
    m_proximityLeftFront(NULL),
    m_proximityLeftRear(NULL),
    m_defaultCommand(NULL),
    m_started(false),
    m_driveMode(INITIAL),
    m_driveModeTimer(0)
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

    m_solenoid = new DoubleSolenoid(solenoidChannel, solenoidChannel+1);
    lw->AddActuator("DriveBase", "Solenoid", m_solenoid);
    
    m_proximityFrontLeft = new AnalogChannel(proximityFrontLeft);
    lw->AddSensor("DriveBase", "ProximityFrontLeft", m_proximityFrontLeft);
    m_proximityFrontRight = new AnalogChannel(proximityFrontRight);
    lw->AddSensor("DriveBase", "ProximityFrontRight", m_proximityFrontRight);
    m_proximityRightFront = new AnalogChannel(proximityRightFront);
    lw->AddSensor("DriveBase", "ProximityRightFront", m_proximityRightFront);
    m_proximityRightRear = new AnalogChannel(proximityRightRear);
    lw->AddSensor("DriveBase", "ProximityRightRear", m_proximityRightRear);
    m_proximityLeftFront = new AnalogChannel(proximityLeftFront);
    lw->AddSensor("DriveBase", "ProximityLeftFront", m_proximityLeftFront);
    m_proximityLeftRear = new AnalogChannel(proximityLeftRear); 
    lw->AddSensor("DriveBase", "ProximityLeftRear", m_proximityLeftRear);

    Stop();
}


DriveBase::~DriveBase()
{
    SetDefaultCommand(NULL);
    Scheduler::GetInstance()->Remove(m_defaultCommand);
    delete m_defaultCommand;
    m_defaultCommand = NULL;

    Stop();
    
    delete m_solenoid;
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


void DriveBase::setNewMode(enum DriveMode newMode) 
{
    if (newMode == m_driveMode) {
    	//Shortcut if we're already in the right mode
	return;
    }

#if 0
    printf("DriveBase::setNewMode(%s)\n", driveModeName[newMode]);
#endif

    switch (newMode) {
	case MECH: 
	case TANK:
	    break; 
	case TANKTOMECH_1:
	case MECHTOTANK_1:
	    Stop();
	    break;
	case INITIAL:
	case MECHTOTANK_2:
	    m_solenoid->Set(DoubleSolenoid::kReverse);
	    break;
	case TANKTOMECH_2:
	    m_solenoid->Set(DoubleSolenoid::kForward);
	    break;
    }
    m_driveMode = newMode;
    m_driveModeTimer = GetFPGATime();
}


void DriveBase::driveModeStateMachine()
{
    unsigned long elapsedTime = GetFPGATime() - m_driveModeTimer;
    switch (m_driveMode) {
	case INITIAL:
	    setNewMode(TANKTOMECH_2);
	    break;
	case MECH: 
	case TANK:
	    break; 
	case TANKTOMECH_1:
	    if (elapsedTime > BRAKINGTIME) 
		setNewMode(TANKTOMECH_2);
	    break;
	case MECHTOTANK_1:
	    if (elapsedTime > BRAKINGTIME) 
		setNewMode(MECHTOTANK_2);
	    break;
	case MECHTOTANK_2:
	    if (elapsedTime > TRANSITIONTIME) 
		setNewMode(TANK);
	    break;
	case TANKTOMECH_2:
	    if (elapsedTime > TRANSITIONTIME) 
		setNewMode(MECH);
	    break;
    }
}

void DriveBase::Drive( float x, float y, float twist, bool pushy )
{

#if 0
    static unsigned long noise;
    if (noise == 0) noise = GetFPGATime();
    unsigned long now = GetFPGATime();
    if ((now - noise) >= 3000000UL) {
	printf("DriveBase: %s x %4.2f y %4.2f t %4.2f\n",
		driveModeName[m_driveMode], x, y, twist);
	noise = now;
    }
#endif

    driveModeStateMachine();
	
    // Adjust the sensitivity to the "twist" control.
    // Add gyro compensation for skew.
    twist -= m_gyro->GetRate() / 120.;

    // limit the twist range to avoid normalization problems
    if (twist < -1.0) twist = -1.0;
    if (twist > 1.0) twist = 1.0;

    // can't drive sideways with the tank wheels
    if (m_driveMode == TANK) {
	x = 0.0;
    }

    if (!pushy) {
	// collision avoidance
	float v;
	
	v = m_proximityFrontLeft->GetVoltage();
	if (v > PRETTYCLOSE) {
	    float backoff;
	    if (v > TOOCLOSE) {
		backoff = BACKOFF;
	    } else {
		backoff = BACKOFF * (v - PRETTYCLOSE) / (TOOCLOSE - PRETTYCLOSE);
	    }
	    if (y > -backoff) y = -backoff;
	}

	v = m_proximityFrontRight->GetVoltage();
	if (v > PRETTYCLOSE) {
	    float backoff;
	    if (v > TOOCLOSE) {
		backoff = BACKOFF;
	    } else {
		backoff = BACKOFF * (v - PRETTYCLOSE) / (TOOCLOSE - PRETTYCLOSE);
	    }
	    if (y > -backoff) y = -backoff;
	}

	v = m_proximityLeftFront->GetVoltage();
	if (v > PRETTYCLOSE) {
	    float backoff;
	    if (v > TOOCLOSE) {
		backoff = BACKOFF;
	    } else {
		backoff = BACKOFF * (v - PRETTYCLOSE) / (TOOCLOSE - PRETTYCLOSE);
	    }
	    if (x < backoff) x = backoff;
	}

	v = m_proximityLeftRear->GetVoltage();
	if (v > PRETTYCLOSE) {
	    float backoff;
	    if (v > TOOCLOSE) {
		backoff = BACKOFF;
	    } else {
		backoff = BACKOFF * (v - PRETTYCLOSE) / (TOOCLOSE - PRETTYCLOSE);
	    }
	    if (x < backoff) x = backoff;
	}

	v = m_proximityRightFront->GetVoltage();
	if (v > PRETTYCLOSE) {
	    float backoff;
	    if (v > TOOCLOSE) {
		backoff = BACKOFF;
	    } else {
		backoff = BACKOFF * (v - PRETTYCLOSE) / (TOOCLOSE - PRETTYCLOSE);
	    }
	    if (x > -backoff) x = -backoff;
	}

	v = m_proximityRightRear->GetVoltage();
	if (v > PRETTYCLOSE) {
	    float backoff;
	    if (v > TOOCLOSE) {
		backoff = BACKOFF;
	    } else {
		backoff = BACKOFF * (v - PRETTYCLOSE) / (TOOCLOSE - PRETTYCLOSE);
	    }
	    if (x > -backoff) x = -backoff;
	}

	// TBD: consider adding twist correction for tank-drive mode
    }

    // RobotDrive was designed to work with joysticks that generate
    // y = -1.0 for forward motion.  We prefer to think of forward
    // motion as y = +1.0.  Change the sign here (and also at the
    // point in the OI where we read the joystick!)

    // TBD: check the motor speed calculations; we may have to use ArcadeDrive
    // to get the correct results for tank mode.

    if (!m_started) Start();
    m_drive->MecanumDrive_Cartesian( x, -y, twist, 0.0 );
}


bool DriveBase::SetDriveMode(bool tankMode)
{
    driveModeStateMachine();

    switch (m_driveMode) {
	case INITIAL:
	    break;
	case MECH:
	    if (tankMode) setNewMode(MECHTOTANK_1);
	    break;
	case TANK:
	    if (!tankMode) setNewMode(TANKTOMECH_1);
	    break;
	case MECHTOTANK_1:
	    if (!tankMode) setNewMode(MECH);
	    break;
	case MECHTOTANK_2:
	    if (!tankMode) setNewMode(TANKTOMECH_2);
	    break;
	case TANKTOMECH_1:
	    if (tankMode) setNewMode(TANK);
	    break;
	case TANKTOMECH_2:
	    if (tankMode) setNewMode(MECHTOTANK_2);
	    break;
	default:
	    break;
    }

    return ((tankMode && m_driveMode == TANK) ||
    	    (!tankMode && m_driveMode == MECH));
}


