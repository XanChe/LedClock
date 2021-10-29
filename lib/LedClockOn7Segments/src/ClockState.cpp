#include <ClockState.h>



ClockState::ClockState(clockStates st)
{
    changeStateTo(st);
}

void ClockState::changeStateTo(clockStates st, byte worckDuration){
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