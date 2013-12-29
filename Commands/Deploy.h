// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 BunnyBots

#ifndef DEPLOY_BUNNY_H
#define DEPLOY_BUNNY_H

#include <Commands/Command.h>

class Solenoid;

class DeployBunny: public Command {
private:
    Solenoid *m_actuator;
public:
    DeployBunny( Solenoid *actuator );
    virtual void Initialize();
    virtual void Execute();
    virtual bool IsFinished();
    virtual void End();
    virtual void Interrupted();
};

#endif
