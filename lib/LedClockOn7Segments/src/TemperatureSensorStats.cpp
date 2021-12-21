#include <TemperatureSensorStats.h>
#include <stdlib.h>
#include <LibConfig.h>


TemperatureSensorStats::TemperatureSensorStats(icons ic, byte startInd)
{
    icon = ic;
    this->stratIndOnFlesh = startInd;
    /*for(byte i; i <24; i++){
        maxTemperatureStatsArray[i] = -120;
        minTemperatureStatsArray[i] = 120;
    }*/
}

void TemperatureSensorStats::saveCurrentHourStats(){
     EEPROM.write(stratIndOnFlesh + currentHour * 2, currentMaxT);
     EEPROM.write(stratIndOnFlesh + currentHour * 2 +1, currentMinT);
}

void TemperatureSensorStats::putCurrentTemperature(unsigned long dateTimeInMinutes, byte hour, byte minute, char temperature){

    
    if(temperature<120 && temperature >-120){
        lastDataCommit = dateTimeInMinutes;
        currentTmeperature = temperature;
    }
    
    isValide = abs(dateTimeInMinutes - lastDataCommit ) <= SENSOR_TIMEOUT_IN_MINUTE;
    if(isValide){
        if(hour != currentHour){
            saveCurrentHourStats();
            currentHour = hour;
            currentMaxT = temperature;
            currentMinT = temperature;
        }else{
            currentMaxT = max(temperature, currentMaxT);
            currentMinT = min(temperature, currentMinT);
        }
       /* if(minute == 0) {
            maxTemperatureStatsArray[hour] = temperature;
            minTemperatureStatsArray[hour] = temperature;
        }else{
            maxTemperatureStatsArray[hour] = max(temperature, maxTemperatureStatsArray[hour]);
            minTemperatureStatsArray[hour] = min(temperature, minTemperatureStatsArray[hour]);
        }*/
    }
    
}

int TemperatureSensorStats::getMaxtemperature(){
    char maxT = -120;
    for(byte i = 0; i<24; i++){
        maxT = max(maxT, EEPROM.read(stratIndOnFlesh + i * 2));
    }
    return maxT;
}

int TemperatureSensorStats::getMintemperature(){
    char minT = 120;
    for(byte i = 0; i<24; i++){
        minT = min(minT, EEPROM.read(stratIndOnFlesh + i * 2 + 1));
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