// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "DriveBase.h"
#include "SetDriveMode.h"
#include <math.h>

SetDriveMode::SetDriveMode( bool tankMode ) :
    Command("SetDriveMode")
{
    Requires(Robot::driveBase());
    m_tankMode = tankMode;
    m_finished = false;
}

// Called just before this Command runs the first time
void SetDriveMode::Initialize()
{
    // printf("SetDriveMode::Initialize\n");
    m_finished = false;
}

// Call this while running to update the speed, direction or time to run.
void SetDriveMode::Set( bool tankMode )
{
    m_tankMode = tankMode;
    m_finished = false;
}

// Called repeatedly when this Command is scheduled to run
void SetDriveMode::Execute()
{
    m_finished = Robot::driveBase()->SetDriveMode(m_tankMode);
}

// Make this return true when this Command no longer needs to run execute()
bool SetDriveMode::IsFinished()
{
    return m_finished;
}

// Called once after isFinished returns true
void SetDriveMode::End()
{
    // printf("SetDriveMode::End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SetDriveMode::Interrupted()
{
    // printf("SetDriveMode::Interrupted\n");
}

