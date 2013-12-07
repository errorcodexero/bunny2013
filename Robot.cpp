// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"

// Subsystems
#include "DriveBase.h"
#include "RateGyro.h"

// Commands
#include "AutoCommand.h"

// Operator Interface
#include "OI.h"


Robot::Robot()
{
    // printf("Robot::Robot() has been called!\n");
}

Robot::~Robot()
{
    // printf("BWA HA HA HA HA!  The Robot cannot be destroyed!\n");
}

void Robot::RobotInit()
{
    // printf("Robot::RobotInit()\n");

    m_oi = new OI();

    // subsystems

    m_compressor = new Compressor( DIGITAL_PRESSURE_SWITCH, RELAY_COMPRESSOR );

    m_driveBase = new DriveBase( PWM_DRIVE_FRONT_LEFT, PWM_DRIVE_BACK_LEFT,
				 PWM_DRIVE_FRONT_RIGHT, PWM_DRIVE_BACK_RIGHT,
				 ANALOG_GYRO, DRIVE_MODE );

    // commands

    m_autonomousCommand = new AutoCommand();

    // link operator controls to commands

    m_oi->Initialize();

    // Now that everything else is set up, start the compressor

    m_compressor->Start();
    SmartDashboard::PutData("scheduler", Scheduler::GetInstance());
}

void Robot::Cancel()
{
    printf("Robot::Cancel\n");
    if (m_autonomousCommand->IsRunning()) {
	m_autonomousCommand->Cancel();
    }
    m_driveBase->Stop();
}
	
void Robot::DisabledInit()
{
    printf("Robot::DisabledInit\n");
    Cancel();
}

void Robot::DisabledPeriodic()
{
    Scheduler::GetInstance()->Run();
}

void Robot::AutonomousInit()
{
    printf("Robot::AutonomousInit\n");
    Cancel();
    m_autonomousCommand->Start();
}
    
void Robot::AutonomousPeriodic()
{
    Scheduler::GetInstance()->Run();
}
    
void Robot::TeleopInit()
{
    printf("Robot::TeleopInit\n");
    Cancel();
}
    
void Robot::TeleopPeriodic()
{
    Scheduler::GetInstance()->Run();
}

void Robot::TestInit()
{
    printf("Robot::TestInit\n");
    Cancel();
}

void Robot::TestPeriodic()
{
    LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot);

