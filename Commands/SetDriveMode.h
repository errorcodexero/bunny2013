// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

#ifndef SET_DRIVE_MODE_H
#define SET_DRIVE_MODE_H

#include <Commands/Command.h>

class SetDriveMode: public Command {
private:
    bool m_tankMode;
    bool m_finished;
public:
    SetDriveMode( bool tankMode );
    void Set( bool tankMode );
    virtual void Initialize();
    virtual void Execute();
    void Stop();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
