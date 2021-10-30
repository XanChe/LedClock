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
public:
    ClockMenu(byte lHour, byte lMinute, CHSV lClockColor, CHSV lSubZeroColor, CHSV lPlusZeroColor);
    void decreaseHour();
    void increaseHour();
    void decreaseMinute();
    void increaseMinute();
    void decreaseColor(CHSV &color);
    void increaseColor(CHSV &color);
    void decreaseBrightness();
    void increaseBrightness();
};





#endif