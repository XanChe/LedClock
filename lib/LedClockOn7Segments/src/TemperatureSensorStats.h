#ifndef TemperatureSensorStats_h
#define TemperatureSensorStats_h
#include <Arduino.h>
#include <LibEnums.h>
#include <EEPROM.h>

class TemperatureSensorStats
{
private:
    
    unsigned long lastDataCommit = 0;
    bool isValide = true;
    icons icon;
    byte stratIndOnFlesh = 0;
    char currentHour = 0;    
    int currentTmeperature = -120;
    int currentMaxT = -120;
    int currentMinT = 120;
   /* char maxTemperatureStatsArray[24]; 
    char minTemperatureStatsArray[24]; */
    void saveCurrentHourStats();
    int readFromEEPROM(byte index);
public:
    TemperatureSensorStats(icons ic, byte startInd);
    
    void putCurrentTemperature(unsigned long dateTimeInMinutes, byte hour, byte minute, int temperature);
    int getMaxtemperature();
    int getMintemperature();
    int getCurentTemperature();
    bool canBeShowed(unsigned long dateTimeInMinutes);
    icons getIcon();
};





#endif
