#include <TemperatureSensorStats.h>
#include <stdlib.h>
#include <LibConfig.h>


TemperatureSensorStats::TemperatureSensorStats(icons ic)
{
    icon = ic;
    for(byte i; i <24; i++){
        maxTemperatureStatsArray[i] = -120;
        minTemperatureStatsArray[i] = 120;
    }
}

void TemperatureSensorStats::putCurrentTemperature(unsigned long dateTimeInMinutes, byte hour, byte minute, char temperature){

    
    if(temperature<120 && temperature >-120){
        lastDataCommit = dateTimeInMinutes;
        currentTmeperature = temperature;
    }
    
    isValide = abs(dateTimeInMinutes - lastDataCommit ) <= SENSOR_TIMEOUT_IN_MINUTE;
    if(minute == 0) {
        maxTemperatureStatsArray[hour] = temperature;
        minTemperatureStatsArray[hour] = temperature;
    }else{
        maxTemperatureStatsArray[hour] = max(temperature, maxTemperatureStatsArray[hour]);
        minTemperatureStatsArray[hour] = min(temperature, minTemperatureStatsArray[hour]);
    }
}

int TemperatureSensorStats::getMaxtemperature(){
    char maxT = -120;
    for(byte i = 0; i<24; i++){
        maxT = max(maxT, maxTemperatureStatsArray[i]);
    }
    return maxT;
}

int TemperatureSensorStats::getMintemperature(){
    char minT = 120;
    for(byte i = 0; i<24; i++){
        minT = min(minT, minTemperatureStatsArray[i]);
    }
    return minT;
}

int TemperatureSensorStats::getCurentTemperature(){
    
    return currentTmeperature;
}
bool TemperatureSensorStats::canBeShowed(unsigned long dateTimeInMinutes){
   
    isValide = abs(dateTimeInMinutes - lastDataCommit ) <= SENSOR_TIMEOUT_IN_MINUTE;
    return isValide;
}

icons TemperatureSensorStats::getIcon(){
    return icon;
}