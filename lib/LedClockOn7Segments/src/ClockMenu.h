#ifndef ClockMenu_h
#define ClockMenu_h
#include <Arduino.h>
#include <LibEnums.h>
#include <Tytedefs.h>

class ClockMenu
{
private:
    byte hour;
    byte minute;      
    byte dColor = 1;

    DisplaySettings *settings;
    menuStates menuState = MENU_HOUR;
    /* data */
    void decreaseHour();
    void increaseHour();
    void decreaseMinute();
    void increaseMinute();
    void decreaseColor(byte &color);
    void increaseColor(byte &color);
    void decreaseBrightness();
    void increaseBrightness();
    void decreaseBrightCorrector();
    void increaseBrightCorrectors();
    void decreasePeriodTemp();
    void increasePeriodTemp();
    void decreaseDurationTemp();
    void increaseDurationTemp();

public:
    
    ClockMenu(DisplaySettings* settings, byte hour, byte minute);
    
    void increseValue();
    void decreaseValue();
    menuStates nextMenu();
    menuStates currentMenu();    
   
    byte getHour(){
        return hour;
    }
    byte getMinute(){
        return minute;
    }    
    void setdColor(byte delta = 1){
        dColor = delta;
    }  
};





#endif