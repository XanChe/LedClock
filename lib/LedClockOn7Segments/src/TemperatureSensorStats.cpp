#include <TemperatureSensorStats.h>
#include <stdlib.h>
#include <LibConfig.h>


TemperatureSensorStats::TemperatureSensorStats(icons ic)
{
    icon = ic;
    for(byte i; i <24; i++){
        maxTemperatureStatsArray[i] = -200.0;
        minTemperatureStatsArray[i] = 1500;
    }
}

void TemperatureSensorStats::putCurrentTemperature(byte day, byte hour, byte minute, float temperature){

    if(temperature<1000 && temperature >-150)lastDataCommit = (unsigned long)day * 1440 + (unsigned long)hour * 60 + (unsigned long)minute;
    isValide = ((unsigned long)day * 1440 + (unsigned long)hour * 60 + (unsigned long)minute) - lastDataCommit <= 10;
    if(minute == 0) {
        maxTemperatureStatsArray[hour] = temperature;
        minTemperatureStatsArray[hour] = temperature;
    }else{
        maxTemperatureStatsArray[hour] = max(temperature, maxTemperatureStatsArray[hour]);
        minTemperatureStatsArray[hour] = min(temperature, minTemperatureStatsArray[hour]);
    }
}

int TemperatureSensorStats::getMaxtemperature(){
    float maxT = -200.0;
    for(byte i = 0; i<24; i++){
        maxT = max(maxT, maxTemperatureStatsArray[i]);
    }
    return maxT;
}

int TemperatureSensorStats::getMintemperature(){
    float minT = 1500;
    for(byte i = 0; i<24; i++){
        minT = min(minT, minTemperatureStatsArray[i]);
    }
    return minT;
}

int TemperatureSensorStats::getCurentTemperature(){
    
    return currentTmeperature;
}
bool TemperatureSensorStats::canBeShowed(){
    return isValide;
}

icons TemperatureSensorStats::getIcon(){
    return icon;
}