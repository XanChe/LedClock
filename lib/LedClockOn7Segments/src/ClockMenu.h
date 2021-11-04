#ifndef ClockMenu_h
#define ClockMenu_h
#include <Arduino.h>
#include <LibEnums.h>
#include <FastLED.h>

class ClockMenu
{
private:
    byte hour;
    byte minute;
    byte brightness = 50;

    CHSV subZeroColor = CHSV(HUE_BLUE, 255, 255);
    CHSV plusZeroColor = CHSV(HUE_ORANGE, 255, 255);
    CHSV clockColor = CHSV(HUE_GREEN, 255, 255);

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
    ClockMenu(byte lHour, byte lMinute, CHSV lClockColor, CHSV lSubZeroColor, CHSV lPlusZeroColor);
    
    void increseValue();
    void decreaseValue();
    clockStates nextMenu();
    clockStates currentMenu();

    CHSV getClockColor(){
        return clockColor;
    }
    CHSV getSubZeroColor(){
        return subZeroColor;
    }
    CHSV getPlusZeroColor(){
        return plusZeroColor;
    }
    byte getHour(){
        return hour;
    }
    byte getMinute(){
        return minute;
    }
    byte getBtightness(){
        return brightness;
    }
    
    CHSV getCurrentColor();
};





#endif