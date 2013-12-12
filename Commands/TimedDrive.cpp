// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#include <WPILib.h>
#include "Robot.h"
#include "DriveBase.h"
#include "TimedDrive.h"
#include <math.h>

TimedDrive::TimedDrive( double x, double y, double t, double seconds, bool pushy ) :
    Command("TimedDrive")
{
    Requires(Robot::driveBase());
    m_x = x;
    m_y = y;
    m_t = t;
    m_time = seconds;
    m_startTime = 0.0;
    m_pushy = pushy;
}

// Called just before this Command runs the first time
void TimedDrive::Initialize()
{
    printf("TimedDrive::Initialize\n");
    m_startTime = Timer::GetFPGATimestamp();
}

// Call this while running to update the speed, direction or time to run.
void TimedDrive::Set( double x, double y, double t, double seconds, bool pushy )
{
    m_x = x;
    m_y = y;
    m_t = t;
    m_time = seconds;
    m_startTime = Timer::GetFPGATimestamp();
    m_pushy = pushy;
}

// Called repeatedly when this Command is scheduled to run
void TimedDrive::Execute()
{
    Robot::driveBase()->Drive(m_x, m_y, m_t, m_pushy);
}

// Make this return true when this Command no longer needs to run execute()
bool TimedDrive::IsFinished()
{
    bool finished = ((Timer::GetFPGATimestamp() - m_startTime) >= m_time);
    if (finished) printf("TimedDrive::IsFinished\n");
    return finished;
}

// Called once after isFinished returns true
void TimedDrive::End()
{
    printf("TimedDrive::End\n");
    Robot::driveBase()->Drive(0.0, 0.0, 0.0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TimedDrive::Interrupted()
{
    printf("TimedDrive::Interrupted\n");
}

