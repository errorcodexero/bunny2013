// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 BunnyBots

#include <WPILib.h>
#include "Robot.h"
#include "DriveBase.h"
#include "Deploy.h"

DeployBunny::DeployBunny( Solenoid *actuator ) :
    Command("DeployBunny")
{
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(chassis);
    m_actuator = actuator;
}

// Called just before this Command runs the first time
void DeployBunny::Initialize()
{
    printf("DeployBunny::Initialize\n");
}

// Called repeatedly when this Command is scheduled to run
void DeployBunny::Execute()
{
    m_actuator->Set(true);
}

// Make this return true when this Command no longer needs to run execute()
bool DeployBunny::IsFinished()
{
    bool finished = true;	// don't wait for actuator
    if (finished) printf("RobotReset::IsFinished\n");
    return finished;
}

// Called once after isFinished returns true
void DeployBunny::End()
{
    printf("DeployBunny::End\n");
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DeployBunny::Interrupted()
{
    printf("DeployBunny::Interrupted\n");
}
