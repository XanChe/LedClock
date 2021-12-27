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
        menuState = MENU_ICONS_COLOR;
        break;
    case MENU_ICONS_COLOR:
        menuState = MENU_BRIGHTNESS;
        break;
    case MENU_BRIGHTNESS:
        menuState = MENU_PERIOD;
        break;
    case MENU_PERIOD:
        menuState = MENU_DURATION;
        break;
    case MENU_DURATION:
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
    case MENU_ICONS_COLOR:
        increaseColor(settings->iconsColor);
        break;
    case MENU_BRIGHTNESS:
        increaseBrightness();
        break;
    case MENU_PERIOD:
        increasePeriodTemp();
        break;
    case MENU_DURATION:
        increaseDurationTemp();
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
    case MENU_ICONS_COLOR:
        decreaseColor(settings->iconsColor);
        break;
    case MENU_BRIGHTNESS:
        decreaseBrightness();
        break;
    case MENU_PERIOD:
        decreasePeriodTemp();
        break;
    case MENU_DURATION:
        decreaseDurationTemp();
        break;  
    default:
        break;
    }
}

void ClockMenu::increaseHour(){
    if(++hour > 23) hour = 0;
}
void ClockMenu::decreaseHour(){
    if(hour - 1 >= 0){
        --hour;
    }else{
        hour = 23;
    }
}
void ClockMenu::decreaseMinute(){
    if(minute -1 >= 0){
        --minute;
    }else{
        minute = 59;
    }    
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
    Serial.print("Br-=");
    if(--settings->brightness < 60) settings->brightness = 60;
    Serial.println(settings->brightness);
}
void ClockMenu::increaseBrightness(){
    Serial.print("Br+=");
    
    if(settings->brightness + 1 <= 255) ++(settings->brightness);
    Serial.println(settings->brightness);
}

void ClockMenu::decreasePeriodTemp(){
    if(--settings->periodTemp < 20) settings->periodTemp = 20;
}
void ClockMenu::increasePeriodTemp(){
    if(++settings->periodTemp > 99) settings->periodTemp = 99;
}

void ClockMenu::decreaseDurationTemp(){
    if(--settings->durationTemp < 15) settings->durationTemp = 15;
}
void ClockMenu::increaseDurationTemp(){
    if(++settings->durationTemp > 60) settings->durationTemp = 60;
}


