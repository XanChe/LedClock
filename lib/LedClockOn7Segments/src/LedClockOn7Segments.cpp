#include <LedClockOn7Segments.h>
#include <stdlib.h>

LedClockOn7Segments::LedClockOn7Segments(){
    displaySettings = loadSettings();
    display.setSettings(displaySettings);
    createSensorStats();   
}
LedClockOn7Segments::~LedClockOn7Segments(){
    LedClockOn7Segments::freeSensorStats();
    freeMenu();
}
void LedClockOn7Segments::freeMenu(){
    delete menu;
    menu = NULL;
}
void LedClockOn7Segments::createSensorStats(){    
    outdoorStats = new TemperatureSensorStats(OUTDOOR_T, 12);    
    indoorStats = new TemperatureSensorStats(INDOOR_T, 60);
}
void LedClockOn7Segments::freeSensorStats(){    
    delete outdoorStats;  
    outdoorStats = NULL;  
    delete indoorStats ;
    indoorStats = NULL;
}

bool LedClockOn7Segments::isMenuMode(){
    return (state == MENU);
}
void LedClockOn7Segments::drowCurentState(){  
    if(!isMenuMode()){
        drowClockState();
    }else{
        drowMenuState();        
    }
}

void LedClockOn7Segments::drowClockState(){
    display.setSettings(displaySettings);
    switch (getClockState()){
        case CUR_TIME:
            if(menu != NULL) freeMenu();
            display.drowTimeOnDispley(curentHour, curentMinute);
            break;
        case CUR_T_OUTDOOR:
            if(drowTemperatureIfCan(ledClock.outdoorStats, temperatureMode)){
                break;
            }                   
        case CUR_T_INDOOR:
            if(drowTemperatureIfCan(ledClock.indoorStats, temperatureMode)){
                break;
            } 
        default:
            display.drowTimeOnDispley(curentHour, curentMinute);
            break;
        }
    if(isButtunLightOn()){
        display.drowIcon(STAT_BUTTON);
        display.drowIcon(SWITCH_BUTTON);
    }
}

void LedClockOn7Segments::drowMenuState(){
    if(menu == NULL) return;
    //display.setSettings(menu->getSettings());
    display.setSettings(displaySettings);
    menuStates menustate = menu->currentMenu();
    switch(menustate)
    {
    case MENU_HOUR:                
        display.drowMenuTimeOnDispley(menu->getHour(), menu->getMinute(), menustate);        
        break;
    case MENU_MINUTES:
        display.drowMenuTimeOnDispley(menu->getHour(), menu->getMinute(), menustate);
        break;
    case MENU_COLOR:
        display.drowTimeOnDispley(menu->getHour(), menu->getMinute());
        //display.drowColorPallete(menu->getSettings().clockColor);        
        break;
    case MENU_PLUS_COLOR:        
        display.drowTemperatureOnDispley(35, icons::FADE_ALL);
        //display.drowColorPallete(menu->getSettings().plusZeroColor);  
        break;
    case MENU_SUB_COLOR:        
        display.drowTemperatureOnDispley(-35, icons::FADE_ALL);
        //display.drowColorPallete(menu->getSettings().subZeroColor);
        break; 
    case MENU_ICONS_COLOR:        
        display.drowIcon(icons::ALL);
        display.drowColorPallete(displaySettings.iconsColor);
        break;
    case MENU_BRIGHTNESS:        
        display.drowSettingsBtigthOnDispley();
        break;
    case MENU_PERIOD:        
        display.drowVauleOnDispley(CHAR_DOWN_P, CHAR_DOWN_R, displaySettings.periodTemp);
        break;
    case MENU_DURATION:        
        display.drowVauleOnDispley(CHAR_DOWN_ZERO, CHAR_DOWN_ZERO, displaySettings.durationTemp);
        break;
    case MENU_SAVE:        
        //TODO "&"
        break;   
    }
}

void LedClockOn7Segments::drowCurentStateOnValueChanging(){
    if(menu == NULL) return;
    //display.setSettings(menu->getSettings());
    menuStates menustate = menu->currentMenu();
    switch(menustate)
    {
    case MENU_HOUR:                
        display.drowMenuTimeOnDispley(menu->getHour(), menu->getMinute(), menustate);        
        break;
    case MENU_MINUTES:
        display.drowMenuTimeOnDispley(menu->getHour(), menu->getMinute(), menustate);
        break;
    case MENU_COLOR:
        //display.drowTimeOnDispley(menu->getHour(), menu->getMinute());
        display.drowColorPallete(displaySettings.clockColor);        
        break;
    case MENU_PLUS_COLOR:        
       // display.drowTemperatureOnDispley(35, icons::FADE_ALL);
        display.drowColorPallete(displaySettings.plusZeroColor);  
        break;
    case MENU_SUB_COLOR:        
        //display.drowTemperatureOnDispley(-35, icons::FADE_ALL);
        display.drowColorPallete(displaySettings.subZeroColor);
        break; 
    case MENU_ICONS_COLOR:        
        display.drowIcon(icons::ALL);
        display.drowColorPallete(displaySettings.iconsColor);
        break;
    case MENU_BRIGHTNESS:   
        display.drowSettingsBtigthOnDispley();
        break;
    case MENU_PERIOD:        
        display.drowVauleOnDispley(CHAR_DOWN_P, CHAR_DOWN_R, displaySettings.periodTemp);
    case MENU_DURATION:        
        display.drowVauleOnDispley(CHAR_DOWN_ZERO, CHAR_DOWN_ZERO, displaySettings.durationTemp);
        break;
    case MENU_SAVE:        
        //TODO "&"
        break;   
    }
}

void LedClockOn7Segments::tick(){
    if((millis() - mil) >=50){
        cronCounter++;
        display.tick();
        mil = millis(); 
        if(cronCounter >= 5000) cronCounter = 0;      
    }        
    switch (cronCounter % 20){
    case 1: //Зпрос датчика температуры
        if(*requestTempCallbackFunction!=NULL && cronCounter>=1200 && cronCounter % 100 < 20){
            requestTempCallbackFunction();
            cronCounter++;
        }
        break;    
    case 18: //Через ~4 секунды - получить ответ
        if(*getAnswerTempCallbackFunction!=NULL && cronCounter % 100 > 78){
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
    display.periodicalRender();
   
}
void LedClockOn7Segments::attachSetLedColor(void (*func)(byte, byte, byte)){
    display.attachSetLedColor(func);
}
void LedClockOn7Segments::attachSetLedIconColor(void (*func)(byte, byte, byte)){
    display.attachSetLedIconColor(func);
}
void LedClockOn7Segments::attachShowMainLed(void (*func)(byte)){
    display.attachShowMainLed(func);
}
void LedClockOn7Segments::attachShowIconLed(void (*func)(byte)){
    display.attachShowIconLed(func);
}
void LedClockOn7Segments::saveSettings(DisplaySettings diaplaySettings){
    diaplaySettings.save();    
}

DisplaySettings LedClockOn7Segments::loadSettings(){
    DisplaySettings loadedSettings;
    loadedSettings.load();
    return loadedSettings;
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

bool LedClockOn7Segments::drowTemperatureIfCan(TemperatureSensorStats *tStats, statMode mode){
    if(tStats != NULL && tStats->canBeShowed(curentDateTimeInMinutes)){
        switch (mode)
        {
        case CURRENT_T_MODE:
            display.drowTemperatureOnDispley(tStats->getCurentTemperature(), tStats->getIcon());
            break;
        case MAX_T_MODE:
            display.drowTemperatureOnDispley(tStats->getMaxtemperature(), tStats->getIcon());
            display.drowIcon(MAX_T);
            
            break;
        case MIN_T_MODE:
            display.drowTemperatureOnDispley(tStats->getMintemperature(), tStats->getIcon());
            display.drowIcon(MIN_T);
            break;
        default:
            break;
        }
        
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
    curentDateTimeInMinutes = (unsigned long)day * 1440 + (unsigned long)hour * 60 + (unsigned long)minutes;
        
}


void LedClockOn7Segments::setCurentIndoorTemperature(char t){
    setCurentTemperature(indoorStats, t);
}

void LedClockOn7Segments::setCurentOutdoorTemperature(char t){
    setCurentTemperature(outdoorStats, t);
}

void LedClockOn7Segments::setCurentTemperature(TemperatureSensorStats *tStats, char t){
    if(tStats != NULL) tStats->putCurrentTemperature(curentDateTimeInMinutes, curentHour, curentMinute, t);
}

bool LedClockOn7Segments::checkStateAvailable(clockStates st){
    switch (st)
    {
    case CUR_TIME:
        return true;
        break;
    case CUR_T_INDOOR:
        return (indoorStats != NULL && indoorStats->canBeShowed(curentDateTimeInMinutes));
        break;
    case CUR_T_OUTDOOR:        
        return (outdoorStats != NULL && outdoorStats->canBeShowed(curentDateTimeInMinutes));
    
    default:
        return false;
        break;
    }
}

void LedClockOn7Segments::switchModeButtonClick(){
    starBtnLigthMilles = millis();
    if(menu != NULL){
        menu->setdColor(12);
        return;
    }
    display.drowIcon(icons::STAT_BUTTON);
    display.drowIcon(icons::SWITCH_BUTTON);
    while(!checkStateAvailable(changeNextAvailable()));
}

void LedClockOn7Segments::statsButtonClick(){
    /*Serial.print("pT=");
    Serial.println(displaySettings.periodTemp);
    Serial.print("D=");
    Serial.println(displaySettings.durationTemp);
    Serial.print("sMil=");
    Serial.println(stateStartMilles);
    Serial.print("wPer=");
    Serial.println(workPeriodInSeconds);*/
    starBtnLigthMilles = millis();
    if(menu != NULL){
        menu->setdColor(1);
        return;
    }else{
        display.drowIcon(icons::STAT_BUTTON);
        display.drowIcon(icons::SWITCH_BUTTON);
        if(getClockState() == CUR_T_OUTDOOR){
            changeNextStstMode();
        }
        if(getClockState() == CUR_T_INDOOR){
            changeNextStstMode();
        }
    }
}

void LedClockOn7Segments::menuButtonClick(){    
    Serial.println("Menu");
    if(menu == NULL){
        freeSensorStats();
        menu = new ClockMenu(&displaySettings, curentHour, curentMinute); 
        menu->setdColor(12);       
        changeStateTo(MENU, 30);
    }else{
        exitFromMenu();
        createSensorStats();
    }    
}

void LedClockOn7Segments::exitFromMenu(){
    curentHour = menu->getHour();
    curentMinute = menu->getMinute();
    curentSecond = 0;
    saveSettings(displaySettings);  
    freeMenu();       
    setHardwareClockTo(curentHour, curentMinute, curentSecond); 
    changeStateTo(CUR_TIME);
    
}

void LedClockOn7Segments::menuNextButtonClick(){
    if(menu != NULL){
        menuStates nextMenu = menu->nextMenu();
        if(nextMenu != MENU_SAVE){
            changeStateTo(MENU, 30);
        }else{
            exitFromMenu();
            createSensorStats();
        }
    }
}

void LedClockOn7Segments::menuPlusButtonClick(){
#ifdef DEBAG
    Serial.println("PLus");
#endif
    if(menu != NULL){
        changeStateTo(MENU, 30);        
        menu->increseValue();  
        display.cronCounter = 0;
        //display.setSettings(menu->getSettings());
        //custormColor =  menu->getCurrentColor();
        drowCurentStateOnValueChanging(); 
        display.render(); 
    }
}
void LedClockOn7Segments::menuMinusButtonClick(){
#ifdef DEBAG
    Serial.println("Minus");
#endif
    if(menu != NULL){
        changeStateTo(MENU, 30);
        menu->decreaseValue();  
        display.cronCounter = 0;         
        drowCurentStateOnValueChanging();
        display.render();     
    }
}

statMode LedClockOn7Segments::changeNextStstMode(){
    switch (temperatureMode)
    {
    case CURRENT_T_MODE:
        temperatureMode = MAX_T_MODE;       
        break;
    case MAX_T_MODE:
        temperatureMode = MIN_T_MODE;
        break;
    case MIN_T_MODE:
        temperatureMode = CURRENT_T_MODE;
        break;
    
    default:
        break;
    }
    return temperatureMode;
}

clockStates LedClockOn7Segments::changeNextAvailable(){
#ifdef DEBAG   
    Serial.println(state);
#endif
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
    
    stateStartMilles = millis();
    workPeriodInSeconds = worckDuration;
    state = st;
    temperatureMode = CURRENT_T_MODE;
}


clockStates LedClockOn7Segments::getClockState(){   
    if(state == CUR_TIME){
        if(millis() >= stateStartMilles + displaySettings.periodTemp*1000U){
            if(outdoorStats != NULL && outdoorStats->canBeShowed(curentDateTimeInMinutes)){
                
                changeStateTo(CUR_T_OUTDOOR, displaySettings.durationTemp);
                return state;
            }else{
                // changeStateTo(CUR_TIME, 60);
            }
        }
    }
    
    if(millis() < (stateStartMilles + workPeriodInSeconds*1000U)){
        return state;
    }else{
        
        changeStateTo(CUR_TIME, 60);
        
        return state;
    }
}

LedClockOn7Segments ledClock = LedClockOn7Segments();
