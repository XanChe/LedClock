#include <ClockMenu.h>
ClockMenu::ClockMenu(byte lHour, byte lMinute, CHSV lClockColor, CHSV lSubZeroColor, CHSV lPlusZeroColor)
{
    hour = lHour;
    minute = lMinute;
    clockColor = lClockColor;
    subZeroColor = lSubZeroColor;
    plusZeroColor = lPlusZeroColor;
}
clockStates ClockMenu::nextMenu(){
    switch (menuState)
    {
    case MENU_HOUR:
        menuState = MENU_MINUTES;
        break;
    case MENU_MINUTES:
        menuState = MENU_COLOR;
        break;
    case MENU_COLOR:
        menuState = MENU_PLUS_COLOR;
        break;
    case MENU_PLUS_COLOR:
        menuState = MENU_SUB_COLOR;
        break;
    case MENU_SUB_COLOR:
        menuState = MENU_BRIGHTNESS;
        break;
    case MENU_BRIGHTNESS:
        menuState = MENU_SAVE;
        break;
    
    default:
         menuState = MENU_SAVE;
        break;
    }
    return menuState;
}

clockStates ClockMenu::currentMenu(){
    return menuState;
}

void ClockMenu::decreaseHour(){
    if(--hour <0) hour = 23;
}

void ClockMenu::increseValue(){
    switch (menuState)
    {
    case MENU_HOUR:
        increaseHour();
        break;
    case MENU_MINUTES:
        increaseMinute();
        break;
    case MENU_COLOR:
        increaseColor(clockColor);
        break;
    case MENU_PLUS_COLOR:
        increaseColor(plusZeroColor);
        break;
    case MENU_SUB_COLOR:
        increaseColor(subZeroColor);
        break;
    case MENU_BRIGHTNESS:
        increaseBrightness();
        break;
    
    default:
        break;
    }    
}

void ClockMenu::decreaseValue(){
    switch (menuState)
    {
    case MENU_HOUR:
        decreaseHour();
        break;
    case MENU_MINUTES:
        decreaseMinute();
        break;
    case MENU_COLOR:
        decreaseColor(clockColor);
        break;
    case MENU_PLUS_COLOR:
        decreaseColor(plusZeroColor);
        break;
    case MENU_SUB_COLOR:
        decreaseColor(subZeroColor);
        break;
    case MENU_BRIGHTNESS:
        decreaseBrightness();
        break;
    
    default:
        break;
    }
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