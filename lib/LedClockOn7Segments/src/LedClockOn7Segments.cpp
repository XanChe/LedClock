#include <LedClockOn7Segments.h>
#include <stdlib.h>

/*
LedClockOn7Segments::LedClockOn7Segments(){

       // displaySettings = loadSettings();

       // display = ClockDisplay(displaySettings);

}*/
bool LedClockOn7Segments::isMenuMode(){
    return (state == MENU_HOUR || state == MENU_MINUTES || state == MENU_PLUS_COLOR || state == MENU_SUB_COLOR || state == MENU_COLOR || state == MENU_BRIGHTNESS);
}
void LedClockOn7Segments::drowCurentState(){
    
    if(!isMenuMode()){
        drowClockState();
    }else{
        drowMenuState();        
    }
    
   
    
    
}

void LedClockOn7Segments::drowClockState(){
    switch (getClockState()){
        case CUR_TIME:
            if(menu != NULL) delete menu;
            display.drowTimeOnDispley(curentHour, curentMinute);
            break;
        case CUR_T_OUTDOOR:
            if(drowTemperatureIfCan(ledClock.outdoorStats)){
                break;
            }        
            
        case CUR_T_INDOOR:
            if(drowTemperatureIfCan(ledClock.indoorStats)){
                break;
            }        
            
        
        default:
            display.drowTimeOnDispley(curentHour, curentMinute);
            break;
        }
}

void LedClockOn7Segments::drowMenuState(){
    if(menu == NULL) exit;
    display.setSettings(menu->getSettings());
    switch (menu->currentMenu())
    {
    case MENU_HOUR:                
        display.drowMenuTimeOnDispley(curentHour, curentMinute, state);
        
        break;
    case MENU_MINUTES:
        display.drowMenuTimeOnDispley(curentHour, curentMinute, state);
        break;
    case MENU_COLOR:
        display.drowTimeOnDispley(curentHour, curentMinute);
        display.drowColorPallete(menu->getSettings().clockColor.hue);        
        break;
    case MENU_PLUS_COLOR:        
        display.drowTemperatureOnDispley(35, icons::FADE_ALL);
        //drowColorPallete();
        break;
    case MENU_SUB_COLOR:        
        display.drowTemperatureOnDispley(-35, icons::FADE_ALL);
       // drowColorPallete();
        break;    
    }
}

void LedClockOn7Segments::drowCurentStateOnValueChanging(){
    
   /* 
    clearDispley();
    clearIcons();
   
    switch (getClockState())
    {
    case MENU_HOUR:        
        drowMenuTimeOnDispley();
        break;
    case MENU_MINUTES:        
        drowMenuTimeOnDispley();
        break;
    case MENU_COLOR:
       // drowMenuTimeOnDispley();
        drowLedSegment(ledMain, 0, NUM_LEDS - SIGN_LED_COUNT, CRGB::Black, PLUS_ZERO);
        drowColorPallete();
        //drowColorPallete();        
        break;
    case MENU_PLUS_COLOR:        
        drowLedSegment(ledMain, 0, NUM_LEDS - SIGN_LED_COUNT, CRGB::Black, PLUS_ZERO);
        drowColorPallete();
        break;
    case MENU_SUB_COLOR:        
        drowLedSegment(ledMain, 0, NUM_LEDS - SIGN_LED_COUNT, CRGB::Black, PLUS_ZERO);
        drowColorPallete();
        break;
    default:
        drowTimeOnDispley();
        break;
    }
    */
}

void LedClockOn7Segments::tick(){
    if((millis() - mil) >=50){
        cronCounter++;
        display.tick();
        mil = millis();        
    }    
    
    switch (cronCounter % 20){
    case 1: //Зпрос датчика температуры
        if(*requestTempCallbackFunction!=NULL && cronCounter>=1200){
            requestTempCallbackFunction();
            cronCounter++;
        }
        break;    
    case 18: //Через чуть-менее секунды - получить ответ
        if(*getAnswerTempCallbackFunction!=NULL && cronCounter>=1200){
            getAnswerTempCallbackFunction();
           cronCounter++;
        }
        break;
    case 0:
        drowCurentState();
        cronCounter++;
        break; 
   
    case 5:
        if(*timeUpdateCallbackFunction!=NULL)timeUpdateCallbackFunction(); 
        cronCounter++;
        break;
    
    default:
        
        break;   
    }
    
    display.render();
}


void LedClockOn7Segments::saveSettings(DisplaySettings diaplaySettings){
    Settings settings = Settings(diaplaySettings.clockColor.hue, diaplaySettings.subZeroColor.hue, diaplaySettings.plusZeroColor.hue, diaplaySettings.brightness);
    settings.save();
}

DisplaySettings LedClockOn7Segments::loadSettings(){
    Settings settings = Settings(HUE_GREEN, HUE_BLUE, HUE_RED, 20);
    settings.load();
    DisplaySettings loadedSettings;
    loadedSettings.clockColor = CHSV(settings.getClockColorHue(), 255, 255);
    loadedSettings.subZeroColor = CHSV(settings.getSuZeroColorHue(), 255, 255);
    loadedSettings.plusZeroColor = CHSV(settings.getPlusZeroColorHue(), 255, 255);
    loadedSettings.brightness = settings.getBrightness();
}





void LedClockOn7Segments::attachGetTimeFunction(void (*func)()){
    timeUpdateCallbackFunction = func;
}
void LedClockOn7Segments::attachSetTimeToHarwareFunction(void (*func)(byte, byte, byte)){
    setHardwareClockTo = func;
}
void LedClockOn7Segments::attachRequestTempFunction(void (*func)()){
    requestTempCallbackFunction = func;
}
void LedClockOn7Segments::attachGetAnswerTempFunction(void (*func)()){
    getAnswerTempCallbackFunction = func;
}
void LedClockOn7Segments::attachMainLedsArray(CRGB ledArray[]){
     display.attachMainLedsArray(ledArray);
}
void LedClockOn7Segments::attachIconLedsArray(CRGB ledArray[]){
    display.attachIconLedsArray(ledArray);
}

void LedClockOn7Segments::attachFastLED(CFastLED &fLED){
    display.attachFastLED(fLED);
}


bool LedClockOn7Segments::drowTemperatureIfCan(TemperatureSensorStats tStats){
    if(tStats.canBeShowed(curentDateTimeInMinutes)){
        display.drowTemperatureOnDispley(tStats.getCurentTemperature(), tStats.getIcon());
        
        return true;
    }else{
        changeStateTo(CUR_TIME);
        return false;
    }
}
void LedClockOn7Segments::setCurTime( byte day, byte hour, byte minutes, byte seconds){
    curentHour = hour;
    curentMinute = minutes;
    curentSecond = seconds;
    //curentTime = ((unsigned long)hour * 3600) + ((unsigned long)minutes * 60) + (unsigned long)seconds;
    curentDateTimeInMinutes = (unsigned long)day * 1440 + (unsigned long)hour * 60 + (unsigned long)minutes;
        
}


void LedClockOn7Segments::setCurentIndoorTemperature(char t){
    setCurentTemperature(indoorStats, t);
}

void LedClockOn7Segments::setCurentOutdoorTemperature(char t){
    setCurentTemperature(outdoorStats, t);
}

void LedClockOn7Segments::setCurentTemperature(TemperatureSensorStats &tStats, char t){
    tStats.putCurrentTemperature(curentDateTimeInMinutes, curentHour, curentMinute, t);
}

bool LedClockOn7Segments::checkStateAvailable(clockStates st){
    switch (st)
    {
    case CUR_TIME:
        return true;
        break;
    case CUR_T_INDOOR:
        return indoorStats.canBeShowed(curentDateTimeInMinutes);
        break;
    case CUR_T_OUTDOOR:
        return outdoorStats.canBeShowed(curentDateTimeInMinutes);
    
    default:
        return false;
        break;
    }
}

void LedClockOn7Segments::switchModeButtonClick(){
    while(!checkStateAvailable(changeNextAvailable()));
}
void LedClockOn7Segments::statsButtonClick(){
    //TODO:
}
void LedClockOn7Segments::menuButtonClick(){
    Serial.println("MenuCLick");
    if(menu == NULL){
       /* menu = new ClockMenu(displaySettings);
        changeStateTo(MENU_HOUR, 30);*/
    }else{
        //saveMenu(menu);
    }
    
}
void LedClockOn7Segments::menuNextButtonClick(){
    Serial.println("Next Menu");
    if(menu != NULL){
        clockStates nextMenu = menu->nextMenu();
        if(nextMenu != MENU_SAVE){
             changeStateTo(nextMenu, 30);
        }else{
            //setSettingsFromMenu(); //TODO: набор сохраняемых параметров выделить в одельную сущность
            
            curentHour = menu->getHour();
            curentMinute = menu->getMinute();
            curentSecond = 0;
            setHardwareClockTo(curentHour, curentMinute, curentSecond);
            displaySettings = menu->getSettings();
            delete menu;
            saveSettings(displaySettings);
            
        }  
    }
}
void LedClockOn7Segments::menuPlusButtonClick(){
    Serial.println("PLus");
    if(menu != NULL){
       changeStateTo(menu->currentMenu(), 30);
       menu->increseValue();  
       cronCounter = 0;  
       display.setSettings(menu->getSettings());
       //custormColor =  menu->getCurrentColor();
       drowCurentStateOnValueChanging(); 
       display.render(); 
    }
}
void LedClockOn7Segments::menuMinusButtonClick(){
    Serial.println("Minus");
    if(menu != NULL){
       changeStateTo(menu->currentMenu(), 30);
       menu->decreaseValue();  
       cronCounter = 0;  
       display.setSettings(menu->getSettings());
       //custormColor =  menu->getCurrentColor();//TODO дублируемый код
       drowCurentStateOnValueChanging();
       display.render();     
    }
}


clockStates LedClockOn7Segments::changeNextAvailable(){
    
    Serial.println(state);
    switch (state)
    {
    case CUR_TIME:
        changeStateTo(CUR_T_OUTDOOR);        
        break;
    case CUR_T_OUTDOOR:
        changeStateTo(CUR_T_INDOOR);
        break;
    case CUR_T_INDOOR:
        changeStateTo(CUR_TIME);
        break;
    
    default:
        break;
    }
    return state;
   

}

void LedClockOn7Segments::changeStateTo(clockStates st, byte worckDuration){
    if(st == MENU_HOUR) {
        //startMenu();
    }
    stateStartMilles = millis();
    workPeriodInSeconds = worckDuration;
    state = st;
}

clockStates LedClockOn7Segments::getClockState(){
    if(millis() < stateStartMilles + workPeriodInSeconds*1000){
        return state;
    }else{
        changeStateTo(CUR_TIME);
        return state;
    }
}


LedClockOn7Segments ledClock = LedClockOn7Segments();
