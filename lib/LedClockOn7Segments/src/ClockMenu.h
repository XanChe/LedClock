#ifndef ClockMenu_h
#define ClockMenu_h
#include <Arduino.h>
#include <LibEnums.h>
#include <FastLED.h>
#include <Tytedefs.h>

class ClockMenu
{
private:
    byte hour;
    byte minute;      

    DisplaySettings settings;
    clockStates menuState = MENU_HOUR;
    /* data */
    void decreaseHour();
    void increaseHour();
    void decreaseMinute();
    void increaseMinute();
    void decreaseColor(CHSV &color);
    void increaseColor(CHSV &color);
    void decreaseBrightness();
    void increaseBrightness();

public:
    
    ClockMenu(DisplaySettings settings);
    
    void increseValue();
    void decreaseValue();
    clockStates nextMenu();
    clockStates currentMenu();
    
    /*CHSV getClockColor(){
        return clockColor;
    }
    CHSV getSubZeroColor(){
        return subZeroColor;
    }
    CHSV getPlusZeroColor(){
        return plusZeroColor;
    }*/
    byte getHour(){
        return hour;
    }
    byte getMinute(){
        return minute;
    }    
    
  //  CHSV getCurrentColor();
    DisplaySettings getSettings(){
        return settings;
    }
};





#endif