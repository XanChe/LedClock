#include <ClockMenu.h>
ClockMenu::ClockMenu(byte lHour, byte lMinute, CHSV lClockColor, CHSV lSubZeroColor, CHSV lPlusZeroColor)
{
    hour = lHour;
    minute = lMinute;
    clockColor = lClockColor;
    subZeroColor = lSubZeroColor;
    plusZeroColor = lPlusZeroColor;
}

void ClockMenu::decreaseHour(){
    if(--hour <0) hour = 23;
}
void ClockMenu::increaseHour(){
    if(++hour > 23) hour = 0;
}
void ClockMenu::decreaseMinute(){
    if(--minute < 0) minute = 59;    
}
void ClockMenu::increaseMinute(){
    if(++minute > 59) minute = 0;
}
void ClockMenu::decreaseColor(CHSV &color){
    int8_t hue = color.hue;
    if(--hue < 0) hue = 360;
    color.hue = hue;    
}
void ClockMenu::increaseColor(CHSV &color){
    int8_t hue = color.hue;
    if(++hue > 360) hue = 0;
    color.hue = hue;
}
void ClockMenu::decreaseBrightness(){
    if(brightness > 15) brightness--;
}
void ClockMenu::increaseBrightness(){
    if(brightness < 255) brightness++;
}