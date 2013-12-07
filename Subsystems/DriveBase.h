// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include "WPILib.h"
#include "RateGyro.h"
#include "DriveCommand.h"

// robot drive base with four-wheel mecanum drive

class DriveBase : public Subsystem {
private:
	SpeedController * m_front_left;
	SpeedController * m_back_left;
	SpeedController * m_front_right;
	SpeedController * m_back_right;
	RobotDrive * m_drive;
	RateGyro * m_gyro;
	Solenoid * m_solenoid;
	INT32 m_gyroZero;
	Command * m_defaultCommand;
	bool m_started;
	enum DriveMode {INITIAL, MECH, TANK, MECHTOTANK_1, MECHTOTANK_2, TANKTOMECH_1, TANKTOMECH_2} m_driveMode;
	unsigned long m_driveModeTimer;
	
	void setNewMode(enum DriveMode newMode);
	void driveModeStateMachine(void);
	
public:
	DriveBase( int frontLeftMotorChannel,
		   int backLeftMotorChannel,
		   int frontRightMotorChannel,
		   int backRightMotorChannel,
		   int gyroAnalogChannel, 
		   int solenoidChannel );

	~DriveBase();

	void InitDefaultCommand();

	void Stop();
	void Start();
	void Drive( float x, float y, float twist );
	bool SetDriveMode(bool tankMode);
};

#endif
