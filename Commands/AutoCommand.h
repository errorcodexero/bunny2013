// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef AUTOCOMMAND_H
#define AUTOCOMMAND_H

#include <Commands/Command.h>
#include <Commands/CommandGroup.h>

// This command group is started whenever the robot enters autonomous mode
// and canceled when the robot enters any other mode.

class TimedDrive;
class SetDriveMode;

#define	MAX_AUTO_SEQ 10		// autonomous mode sequences
#define MAX_AUTO_STEPS 6	// max steps in each sequence

struct DriveStep {
    double x;
    double y;
    double twist;
    double secs;
};

struct DrivePattern {
    DriveStep step[MAX_AUTO_STEPS];
};

class AutoCommand: public CommandGroup {
public:	
    AutoCommand();
    virtual ~AutoCommand();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
    void SetDrivePattern( int pattern );
    void GetDashboardSettings();
    void PutDashboardSettings();
    void GetAutoPreferences();
    void SaveAutoPreferences();

private:
    SetDriveMode *m_setDriveMode;
    TimedDrive *m_step[6];

    int m_pattern;  // which autonomous mode pattern to run
    struct DrivePattern m_drivePattern[MAX_AUTO_SEQ];

    class AutoSelect *m_autoSelectCmd;
    class AutoSave *m_autoSaveCmd;
};

// These two commands run in the background
// to monitor and act on SmartDashboard updates to the drive pattern

class AutoSelect : public Command {
private:
    AutoCommand *m_auto;
public:
    AutoSelect( AutoCommand *cmd );
    virtual ~AutoSelect();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

class AutoSave : public Command {
private:
    AutoCommand *m_auto;
public:
    AutoSave( AutoCommand *cmd );
    virtual ~AutoSave();
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
