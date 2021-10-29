#ifndef TemperatureSensorStats_h
#define TemperatureSensorStats_h
#include <Arduino.h>
#include <LibEnums.h>

class TemperatureSensorStats
{
private:
    
    unsigned long lastDataCommit = 0;
    bool isValide = true;

    float currentTmeperature = -200.0;
    float maxTemperatureStatsArray[24]; 
    float minTemperatureStatsArray[24]; 

public:
    TemperatureSensorStats(/* args */);
    
    void putCurrentTemperature(byte day, byte hour, byte minute, float temperature);
    int getMaxtemperature();
    int getMintemperature();
    int getCurentTemperature();
    bool canBeShowed();
};





#endif
