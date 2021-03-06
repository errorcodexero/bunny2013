// FIRST Team 1425 "Error Code Xero"
// for FRC 2013 game "Ultimate Ascent"

class OI;

#ifndef OI_H
#define OI_H

#include <WPILib.h>
#include "Robot.h"

class DSAnalogInput {
private:
    DriverStationEnhancedIO *m_pEIO;
    UINT32 m_channel;

public:
    DSAnalogInput( DriverStationEnhancedIO *pEIO, UINT32 channel ) :
	m_pEIO(pEIO),
	m_channel(channel)
    {
	;
    }

    ~DSAnalogInput() {}

    double GetAnalog() {
	return m_pEIO->GetAnalogInRatio(m_channel); 
    }

    int GetDigital( int numPositions ) {
	return (int)(GetAnalog() * numPositions);
    }
};

class DSDigitalInput : public Button {
private:
    DriverStationEnhancedIO *m_pEIO;
    UINT32 m_channel;
    bool m_active;

public:
    DSDigitalInput( DriverStationEnhancedIO *pEIO, UINT32 channel,
    		    DriverStationEnhancedIO::tDigitalConfig config,
		    bool active ) :
	m_pEIO(pEIO),
	m_channel(channel),
	m_active(active)
    {
	m_pEIO->SetDigitalConfig(channel, config);
    }

    ~DSDigitalInput() {}

    // return true when the button is in the active state, whether high or low
    bool Get() {
	return (m_pEIO->GetDigital(m_channel) == m_active);
    }
};

class DSDigitalOutput {
private:
    DriverStationEnhancedIO *m_pEIO;
    UINT32 m_channel;

public:
    DSDigitalOutput( DriverStationEnhancedIO *pEIO, UINT32 channel ) :
	m_pEIO(pEIO),
	m_channel(channel)
    {
	m_pEIO->SetDigitalConfig(channel,
	    DriverStationEnhancedIO::kOutput);
    }

    ~DSDigitalOutput() {}

    void Set( bool value ) {
	return m_pEIO->SetDigitalOutput(m_channel, value);
    }
};

class OI {
private:
    DriverStation *m_pDS;
    DriverStationEnhancedIO *m_pEIO;
    DriverStationLCD *m_pLCD;

    Joystick* m_pStick;
    JoystickButton* m_pGamepadButtonA;
    JoystickButton* m_pGamepadButtonB;
    JoystickButton* m_pGamepadButtonX;
    JoystickButton* m_pGamepadButtonY;
    JoystickButton* m_pGamepadLeftBumper;
    JoystickButton* m_pGamepadRightBumper;
    JoystickButton* m_pGamepadBack;
    JoystickButton* m_pGamepadStart;

    DSAnalogInput* m_pAutoSelect;

    // aiming
    class TimedDrive* m_pNudgeLeft;
    class TimedDrive* m_pNudgeRight;

    class Rotate* m_pRotateFwd;
    class Rotate* m_pRotateRev;

    // reset (for pit use!)
    class ResetRobot* m_pResetRobot;

public:
    OI();
    ~OI();
    void Initialize();
    DriverStation *GetDS() { return m_pDS; }
    DriverStationEnhancedIO* GetEIO() { return m_pEIO; }
    DriverStationLCD* GetLCD() { return m_pLCD; }

    float GetX() { return m_pStick->GetRawAxis( 1 ); } 
    // Windows joysticks return y = -1.0 when pushed forward.
    // We prefer to think of forward motion as y = +1.0
    float GetY() { return -m_pStick->GetRawAxis( 2 ); } 
    float GetThrottle() { return m_pStick->GetRawAxis( 3 ); } 
    float GetTwist() { return m_pStick->GetRawAxis( 4 ); } 
    float GetX2() { return m_pStick->GetRawAxis( 4 ); }
    float GetY2() { return -m_pStick->GetRawAxis( 5 ); }
    int GetAuto() { return m_pAutoSelect->GetDigital(10); }

    bool MechanumMode() { return m_pGamepadStart->Get(); }
    bool TankMode()     { return m_pGamepadBack->Get(); }

    bool GetButtonA() { return m_pGamepadButtonA->Get(); }
    bool GetButtonB() { return m_pGamepadButtonB->Get(); }
    bool GetButtonX() { return m_pGamepadButtonX->Get(); }
    bool GetButtonY() { return m_pGamepadButtonY->Get(); }
}; 
#endif
