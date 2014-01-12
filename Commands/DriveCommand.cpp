// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "OI.h"
#include "DriveBase.h"
#include "DriveCommand.h"

// Identify resources required by this command.
// Other commands that are using these resources will be Canceled
// when this command is Started.
DriveCommand::DriveCommand() :
    Command("DriveCommand"),
    tankMode(true)
{
    Requires(Robot::driveBase());
}

// Called just before this Command runs the first time
void DriveCommand::Initialize()
{
    printf("DriveCommand::Initialize\n");
    tankMode = true;
}

// Called repeatedly when this Command is scheduled to run
void DriveCommand::Execute()
{
    if (Robot::oi()->MechanumMode()) {
	if (tankMode) printf("DriveCommand: switch to mechanum mode\n");
	tankMode = false;
    } else if (Robot::oi()->TankMode()) {
	if (!tankMode) printf("DriveCommand: switch to tank mode\n");
	tankMode = true;
    }

    (void) Robot::driveBase()->SetDriveMode(tankMode);

    float throttle = (1.0 - Robot::oi()->GetThrottle())/2.0;
    float x = Robot::oi()->GetX();
    float y = Robot::oi()->GetY();
    float t = Robot::oi()->GetTwist();
    Robot::driveBase()->Drive(x * throttle, y * throttle, t, true);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveCommand::IsFinished()
{
    return false;
}

// Called once after isFinished returns true
void DriveCommand::End()
{
    printf("DriveCommand::End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveCommand::Interrupted()
{
    printf("DriveCommand::Interrupted\n");
    Robot::driveBase()->Stop();
}
