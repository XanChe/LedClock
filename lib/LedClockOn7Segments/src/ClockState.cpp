#include <ClockState.h>



ClockState::ClockState(clockStates st)
{
    changeStateTo(st);
}

void ClockState::changeNextAvailable(){
    
    Serial.println(state);
    switch (state)
    {
    case CUR_TIME:
        changeStateTo(CUR_T_OUTDOOR);
        break;
    case CUR_T_OUTDOOR:
        changeStateTo(CUR_T_INDOOR);
        break;
    case CUR_T_INDOOR:
        changeStateTo(CUR_TIME);
        break;
    
    default:
        break;
    }
}

void ClockState::changeStateTo(clockStates st, byte worckDuration){
    if(st == MENU_HOUR) {
        //startMenu();
    }
    stateStartMilles = millis();
    workPeriodInSeconds = worckDuration;
    state = st;
}

clockStates ClockState::getClockState(){
    if(millis() < stateStartMilles + workPeriodInSeconds*1000){
        return state;
    }else{
        changeStateTo(CUR_TIME);
        return state;
    }
}

bool ClockState::equals(clockStates st){
    return st == state;
}