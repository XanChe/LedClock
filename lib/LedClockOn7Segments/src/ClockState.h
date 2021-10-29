#ifndef ClockState_h
#define ClockState_h
#include <Arduino.h>
#include <LibEnums.h>

class ClockState
{
private:
    clockStates state;
    unsigned long stateStartMilles = 0;
    byte workPeriodInSeconds = 15;
public:
    ClockState(clockStates st);
    void changeStateTo(clockStates st, byte worckDuration = 15);
    bool equals(clockStates st);
    void changeNextAvailable();
    clockStates getClockState();
};




#endif