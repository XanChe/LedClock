#include <ClockMenu.h>

ClockMenu::ClockMenu(DisplaySettings* settings, byte hour, byte minute)
{
    this->settings = settings;
    this->hour = hour;
    this->minute = minute;
}
menuStates ClockMenu::nextMenu(){
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

menuStates ClockMenu::currentMenu(){
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
        increaseColor(settings->clockColor);
        break;
    case MENU_PLUS_COLOR:
        increaseColor(settings->plusZeroColor);
        break;
    case MENU_SUB_COLOR:
        increaseColor(settings->subZeroColor);
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
        decreaseColor(settings->clockColor);
        break;
    case MENU_PLUS_COLOR:
        decreaseColor(settings->plusZeroColor);
        break;
    case MENU_SUB_COLOR:
        decreaseColor(settings->subZeroColor);
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
void ClockMenu::decreaseColor(int8_t &color){    
    int tColor = color - dColor;
    if(tColor < 0) tColor += 255; 
    color = tColor % 255;    
}
void ClockMenu::increaseColor(int8_t &color){    
    int tColor = color + dColor;
    color = tColor % 255;
}
void ClockMenu::decreaseBrightness(){
    if(settings->brightness > 60) settings->brightness--;
}
void ClockMenu::increaseBrightness(){
    if(settings->brightness < 255) settings->brightness++;
}
/*
CHSV ClockMenu::getCurrentColor(){
        switch (menuState)
        {
        case MENU_HOUR:
            return clockColor;
            break;
        case MENU_MINUTES:
            return clockColor;
            break;
        case MENU_COLOR:
            return clockColor;
            break;
        case MENU_PLUS_COLOR:
            return plusZeroColor;
            break;
        case MENU_SUB_COLOR:
            return subZeroColor;
            break;
        
        default:
            return clockColor;
            break;
        }
}*/

