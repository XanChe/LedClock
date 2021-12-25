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

    DisplaySettings *settings;
    menuStates menuState = MENU_HOUR;
    /* data */
    void decreaseHour();
    void increaseHour();
    void decreaseMinute();
    void increaseMinute();
    void decreaseColor(int8_t &color);
    void increaseColor(int8_t &color);
    void decreaseBrightness();
    void increaseBrightness();

public:
    
    ClockMenu(DisplaySettings *settings);
    
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
    
  //  CHSV getCurrentColor();
    DisplaySettings getSettings(){
        return *settings;
    }
};





#endif