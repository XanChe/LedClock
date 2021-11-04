#include <LedClockOn7Segments.h>
#include <stdlib.h>
/*
 *       ---
 *     |  6  |
 *    7|     |5
 *       ---
 *     |  4  |
 *    3|     |1
 *       ---
 *       2
 */
byte numbers[] = {
    0b01110111, // 0    
    0b00010001, // 1
    0b00111110, // 2
    0b00111011, // 3
    0b01011001, // 4
    0b01101011, // 5
    0b01101111, // 6
    0b00110001, // 7
    0b01111111, // 8
    0b01111011, // 9 
    0b01111000, // CHAR_UP_ZERO
    0b01100110, // CHAR_C
  };

LedClockOn7Segments::LedClockOn7Segments(){
    loadSettings();
}

void LedClockOn7Segments::tick(){
    if((millis() - mil) >=50){
        cronCounter++;
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
            cronCounter = 18;
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
    
    if(cronCounter % 10 == 0 ){
        if(menu == NULL){
            render();
        }else{
            render(menu->getBtightness());
        }
    }
}

void LedClockOn7Segments::setSettingsFromMenu(){
    curentHour = menu->getHour();
    curentMinute = menu->getMinute();
    curentSecond = 0;
    setHardwareClockTo(curentHour, curentMinute, curentSecond);

    clockColor = menu->getClockColor();
    subZeroColor = menu->getSubZeroColor();
    plusZeroColor = menu->getPlusZeroColor();
    brightness = menu->getBtightness();


}

void LedClockOn7Segments::saveSettings(){
    Settings settings = Settings(clockColor.hue, subZeroColor.hue, plusZeroColor.hue, brightness);
    settings.save();
}

void LedClockOn7Segments::loadSettings(){
    Settings settings = Settings(clockColor.hue, subZeroColor.hue, plusZeroColor.hue, brightness);
    settings.load();
    clockColor = CHSV(settings.getClockColorHue(), 255, 255);
    subZeroColor = CHSV(settings.getSuZeroColorHue(), 255, 255);
    plusZeroColor = CHSV(settings.getPlusZeroColorHue(), 255, 255);
    brightness = settings.getBrightness();
}



void LedClockOn7Segments::drowCurentState(){
    
    
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
        drowMenuTimeOnDispley();
        //drowColorPallete();        
        break;
    case MENU_PLUS_COLOR:        
        drowTemperatureOnDispley(35);
        //drowColorPallete();
        break;
    case MENU_SUB_COLOR:        
        drowTemperatureOnDispley(-35);
       // drowColorPallete();
        break;
    case CUR_TIME:
        if(menu != NULL) delete menu;
        drowTimeOnDispley();
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
        drowTimeOnDispley();
        break;
    }
    
}

void LedClockOn7Segments::drowCurentStateOnValueChanging(){
    
    
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
        drowMenuTimeOnDispley();
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
void LedClockOn7Segments::attachMainLedsArray(CRGB lesArray[]){
    mainLedsArray = lesArray;
}
void LedClockOn7Segments::attachIconLedsArray(CRGB lesArray[]){
    iconLedsArray = lesArray;
}

void LedClockOn7Segments::attachFastLED(CFastLED &fLED){
    CurFastLED = fLED;
}



void LedClockOn7Segments::setCurTime( byte day, byte hour, byte minutes, byte seconds){
    curentHour = hour;
    curentMinute = minutes;
    curentSecond = seconds;
    //curentTime = ((unsigned long)hour * 3600) + ((unsigned long)minutes * 60) + (unsigned long)seconds;
    curentDateTimeInMinutes = (unsigned long)day * 1440 + (unsigned long)hour * 60 + (unsigned long)minutes;
        
}

void LedClockOn7Segments::clearDispley(){     
    drowLedSegment(ledMain, 0, NUM_LEDS, CRGB::Black, OFF, false);    
}

void LedClockOn7Segments::clearIcons(){     
    drowLedSegment(ledIcons, 0, NUM_ICON_LEDS, CRGB::Black, OFF, false);    
}

void LedClockOn7Segments::drowIcon(icons icon){    
    switch (icon)
    {
    case icons::INDOOR_T:
        drowLedSegment(ledIcons, 6 , 4, CRGB::AliceBlue, DAYLY);        
        break;
    case icons::OUTDOOR_T:
        drowLedSegment(ledIcons, 10 , 4, CRGB::AliceBlue, DAYLY);
        break;
    case icons::MAX_T:
        drowLedSegment(ledIcons, 4 , 1, CRGB::AliceBlue, DAYLY);
        break;
    case icons::MIN_T:
        drowLedSegment(ledIcons, 5 , 1, CRGB::AliceBlue, DAYLY);
        break;
    case icons::SWITCH_BUTTON:
        drowLedSegment(ledIcons, 2 , 2, CRGB::AliceBlue, DAYLY);
        break;
    case icons::STAT_BUTTON:
        drowLedSegment(ledIcons, 0 , 2, CRGB::AliceBlue, DAYLY);
        break;
    case icons::ALL:
        drowLedSegment(ledIcons, 0 , 14, CRGB::AliceBlue, NIGHTLY);
        break;
    case icons::FADE_ALL:
        drowLedSegment(ledIcons, 0 , 14, CRGB::Black, OFF);
        break;
    default:
        drowLedSegment(ledIcons, 0 , 14, CRGB::Black, OFF);
        break;
    }
}


void LedClockOn7Segments::fillColorThreeHorizonLines(byte start, byte colorIndex, CRGB colorPallete[]){
    byte iBottom = start + SEGMENT_LED_COUNT;
    byte iMiddle = start + 4 * SEGMENT_LED_COUNT -1;
    byte iTop = start + 5 * SEGMENT_LED_COUNT;
    for(byte i = colorIndex; i < colorIndex + SEGMENT_LED_COUNT; i++){
        
        ledMain[iBottom++].color = colorPallete[i];
        ledMain[iMiddle--].color = colorPallete[i];
        ledMain[iTop++].color = colorPallete[i];

    }    
}
void LedClockOn7Segments::fillNubreByColorPallete(byte start, byte colorIndex, CRGB colorPallete[]){

    

    fillColorToLedSegment(ledMain,start , SEGMENT_LED_COUNT, colorPallete[colorIndex]);
    fillColorToLedSegment(ledMain,start + 4 * SEGMENT_LED_COUNT, SEGMENT_LED_COUNT, colorPallete[colorIndex]);
    fillColorThreeHorizonLines(start, colorIndex + 1, colorPallete);
    fillColorToLedSegment(ledMain,start + 2 * SEGMENT_LED_COUNT , SEGMENT_LED_COUNT, colorPallete[colorIndex + SEGMENT_LED_COUNT]);
    fillColorToLedSegment(ledMain,start + 6 * SEGMENT_LED_COUNT , SEGMENT_LED_COUNT, colorPallete[colorIndex + SEGMENT_LED_COUNT]);

}
void LedClockOn7Segments::drowColorPallete(){

    byte gradientLen = 9 + SEGMENT_LED_COUNT * 4;
    fill_gradient(mainLedsArray, 0, CHSV(custormColor.hue - 1, 255, 255), gradientLen , CHSV(custormColor.hue + 1, 255, 255), TGradientDirectionCode::LONGEST_HUES);
    fillNubreByColorPallete(0, 0, mainLedsArray);
    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7, SEGMENT_LED_COUNT + 2, mainLedsArray);
    drowDotes(CUSTOM_COLOR);
    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7 * 2 + DOTES_LED_COUNT, SEGMENT_LED_COUNT * 2 + 4 + 1, mainLedsArray);
    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7 * 3 + DOTES_LED_COUNT, SEGMENT_LED_COUNT * 3 + 6 + 1, mainLedsArray);
}

void LedClockOn7Segments::fillColorToLedSegment(LedPixel ledArray[], byte start, byte count, CRGB color){
    for(byte i = start; i < start + count; i++){
            ledArray[i].color = color;
            
    }
}



void LedClockOn7Segments::drowLedSegment(LedPixel ledArray[], byte start, byte count, CRGB color, showingLedEffects effect, bool isShowed){
    for(byte i = start; i < start + count; i++){
            ledArray[i].color = color;
            ledArray[i].isShowed = isShowed;
            ledArray[i].effect = effect;
    }
}

bool LedClockOn7Segments::drowTemperatureIfCan(TemperatureSensorStats tStats){
    if(tStats.canBeShowed(curentDateTimeInMinutes)){
        drowTemperatureOnDispley(tStats.getCurentTemperature());
        drowIcon(tStats.getIcon());
        return true;
    }else{
        changeStateTo(CUR_TIME);
        return false;
    }
}

void LedClockOn7Segments::drowTemperatureOnDispley(int temperature){
    clearDispley();
    int templ = abs(temperature) / 10;
    int tempr = abs(temperature) % 10;    
    
    showingLedEffects effect = temperature >= 0 ? PLUS_ZERO : SUB_ZERO;
    
    drowNumber(0, CHAR_C, effect);    
    drowNumber(SEGMENT_LED_COUNT*7, CHAR_UP_ZERO, DAYLY);
    drowDotes(OFF) ;
    drowNumber(SEGMENT_LED_COUNT*7*2+DOTES_LED_COUNT, tempr, effect);
    drowNumber(SEGMENT_LED_COUNT*7*3+DOTES_LED_COUNT, templ, effect);   
    if(temperature<0)drowSign();   
      
}

void LedClockOn7Segments::drowSign(){
    
    drowLedSegment(ledMain, SEGMENT_LED_COUNT*28+DOTES_LED_COUNT, 2, subZeroColor, SUB_ZERO);    
}

void LedClockOn7Segments::drowTimeOnDispley(showingLedEffects effect){
    drowTimeOnDispley(curentHour, curentMinute, effect);
}

void LedClockOn7Segments::drowMenuTimeOnDispley(showingLedEffects effect){
    if(menu != NULL)drowTimeOnDispley(menu->getHour(), menu->getMinute(), effect);
}

void LedClockOn7Segments::drowDotes(showingLedEffects effect){
    
    drowLedSegment(ledMain, SEGMENT_LED_COUNT*7*2, DOTES_LED_COUNT, clockColor, effect);    
}

void LedClockOn7Segments::drowTimeOnDispley(byte hour, byte minutes, showingLedEffects effect){     

    clearDispley();    
    drowMinutes(minutes, state != MENU_MINUTES ? effect : BLINK);    
    drowDotes(state == CUR_TIME ? BLINK : OFF);  
    drowHour(hour, state != MENU_HOUR ? effect : BLINK);     
}

void LedClockOn7Segments::drowHour(byte hour, showingLedEffects effect){

    drowNumber(SEGMENT_LED_COUNT*7*2+DOTES_LED_COUNT, hour % 10, effect); // +DOTES_LED_COUNT сдесь + DOTES_LED_COUNT мигающие точки
    drowNumber(SEGMENT_LED_COUNT*7*3+DOTES_LED_COUNT, hour / 10, effect);
}

void LedClockOn7Segments::drowMinutes(byte minutes, showingLedEffects effect){
    drowNumber(0, minutes % 10, effect);    
    drowNumber(SEGMENT_LED_COUNT*7, minutes / 10, effect);
}

void LedClockOn7Segments::drowNumber(int startindex, byte number, showingLedEffects effect){ 
    CRGB segmentColor = CRGB::Black;
    int z = 0;
    for (int i = 0; i < 7; i++) {
        bool isShowed = (numbers[number] & 1 << i) == 1 << i;
        segmentColor = isShowed ? (CRGB)clockColor : CRGB::Black;
        drowLedSegment(ledMain, z + startindex, SEGMENT_LED_COUNT, segmentColor, effect, isShowed);
        z += SEGMENT_LED_COUNT;      
    }
}

void LedClockOn7Segments::render(byte brigth){
    //if(brigth == 0) brigth = brightness;
    if(iconLedsArray != NULL) {
        renderIcons(iconLedsArray);
    }
    if(mainLedsArray != NULL){
        render(mainLedsArray);
    }
    CurFastLED.setBrightness(brigth==0 ? brightness: brigth);
    CurFastLED.show();
    cronCounter++;
   
}

void LedClockOn7Segments::render(CRGB displayLed[]){
    
        //Serial.println(cronCounter);
        for(int i = 0; i < NUM_LEDS; i++){            
            displayLed[i] = applyPixelEffect(ledMain[i]);       
        }
        
}

void LedClockOn7Segments::renderIcons(CRGB displayLed[]){  
        for(int i = 0; i < NUM_ICON_LEDS; i++){
            displayLed[i] = applyPixelEffect(ledIcons[i]);
        }           
}


CRGB LedClockOn7Segments::applyPixelEffect(LedPixel ledPixel){
    switch (ledPixel.effect)
    {
    case DAYLY:
        return ledPixel.isShowed  ? ledPixel.color : CRGB::Black;;
        break;
    case OFF:
        return  CRGB::Black;        
        break;
    case SUB_ZERO:
        return  ledPixel.isShowed  ? (CRGB)subZeroColor : CRGB::Black;
        break;
    case PLUS_ZERO:
        return  ledPixel.isShowed  ? (CRGB)plusZeroColor : CRGB::Black;
        break;
    case BLINK:       
        if(cronCounter % 40 < 20) {           
            return ledPixel.color;
        }
        else{
            return CRGB::Black;
        }
        break;
    case CUSTOM_COLOR:
            ledPixel.isShowed  ? (CRGB)custormColor : CRGB::Black;
        break;
    default:
        return ledPixel.color;
        break;
    }
    
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
        menu = new ClockMenu(this->curentHour, this->curentMinute, this->clockColor, this->subZeroColor, this->plusZeroColor);
        changeStateTo(MENU_HOUR, 30);
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
            setSettingsFromMenu(); //TODO: набор сохраняемых параметров выделить в одельную сущность
            saveSettings();
            delete menu;
        }  
    }
}
void LedClockOn7Segments::menuPlusButtonClick(){
    Serial.println("PLus");
    if(menu != NULL){
       changeStateTo(menu->currentMenu(), 30);
       menu->increseValue();  
       cronCounter = 0;  
       custormColor =  menu->getCurrentColor();
       drowCurentStateOnValueChanging(); 
       render(menu->getBtightness()); 
    }
}
void LedClockOn7Segments::menuMinusButtonClick(){
    Serial.println("Minus");
    if(menu != NULL){
       changeStateTo(menu->currentMenu(), 30);
       menu->decreaseValue();  
       cronCounter = 0;  
       custormColor =  menu->getCurrentColor();//TODO дублируемый код
       drowCurentStateOnValueChanging();
       render(menu->getBtightness());     
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
