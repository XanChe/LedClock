#include <ClockMenu.h>

ClockMenu::ClockMenu(DisplaySettings settings)
{
    this->settings = settings;
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
        increaseColor(settings.clockColor);
        break;
    case MENU_PLUS_COLOR:
        increaseColor(settings.plusZeroColor);
        break;
    case MENU_SUB_COLOR:
        increaseColor(settings.subZeroColor);
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
        decreaseColor(settings.clockColor);
        break;
    case MENU_PLUS_COLOR:
        decreaseColor(settings.plusZeroColor);
        break;
    case MENU_SUB_COLOR:
        decreaseColor(settings.subZeroColor);
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
    int hue = color.hue;
    if(--hue < 0) hue = 192;
    color = CHSV(hue, 255, 255);   
}
void ClockMenu::increaseColor(CHSV &color){
    int hue = color.hue;
    if(++hue > 192) hue = 0;
    Serial.println(hue);
    color = CHSV(hue, 255, 255);
}
void ClockMenu::decreaseBrightness(){
    if(settings.brightness > 15) settings.brightness--;
}
void ClockMenu::increaseBrightness(){
    if(settings.brightness < 255) settings.brightness++;
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

