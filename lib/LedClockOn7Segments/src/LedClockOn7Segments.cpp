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

void LedClockOn7Segments::tick(){
    if((millis() - mil) >=50){
        cronCounter++;
        mil = millis();        
    }    
    
    switch (cronCounter % 20){
    case 1:
        
        //cronCounter++;
        break;
    case 19:
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
     
    
    

}

void LedClockOn7Segments::drowCurentState(){
    clearDispley();
    clearIcons();
    switch (clockState.getClockState())
    {
    case MENU_HOUR:
        
        drowTimeOnDispley();
        break;
    case CUR_TIME:
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

void LedClockOn7Segments::attachTimeUpdateFunction(void (*func)()){
    timeUpdateCallbackFunction = func;
}

void LedClockOn7Segments::attachMainLedsArray(CRGB lesArray[]){
    mainLedsArray = lesArray;
}
void LedClockOn7Segments::attachIconLedsArray(CRGB lesArray[]){
    iconLedsArray = lesArray;
}

void LedClockOn7Segments::assignFastLED(CFastLED &fLED){
    CurFastLED = fLED;
}


void LedClockOn7Segments::setCurTime( byte day, byte hour, byte minutes, byte seconds){
    curentHour = hour;
    curentMinute = minutes;
    curentSecond = seconds;
    curentTime = ((unsigned long)hour * 3600) + ((unsigned long)minutes * 60) + (unsigned long)seconds;
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
        clockState.changeStateTo(CUR_TIME);
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
    drowTimeOnDispley(curentTime / 3600, (curentTime % 3600) / 60, effect);
}

void LedClockOn7Segments::drowDotes(showingLedEffects effect){
    
    drowLedSegment(ledMain, SEGMENT_LED_COUNT*7*2, DOTES_LED_COUNT, clockColor, effect);    
}

void LedClockOn7Segments::drowTimeOnDispley(byte hour, byte minutes, showingLedEffects effect){     

    clearDispley();    
    drowMinutes(minutes, !clockState.equals(MENU_MINUTES) ? effect : BLINK);    
    drowDotes(clockState.equals(CUR_TIME) ? BLINK : OFF);  
    drowHour(hour, !clockState.equals(MENU_HOUR) ? effect : BLINK);     
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

void LedClockOn7Segments::render(){
    if(cronCounter % 10 == 0 ){
        if(iconLedsArray != NULL) {
            renderIcons(iconLedsArray);
        }
        if(mainLedsArray != NULL){
            render(mainLedsArray);
        }
        CurFastLED.setBrightness( 20 );
        CurFastLED.show();
        cronCounter++;
    }
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
        return ledPixel.color;
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

void LedClockOn7Segments::switchModeButtonClick(){
    clockState.changeNextAvailable();
}
void LedClockOn7Segments::statsButtonClick(){
    //TODO:
}
void LedClockOn7Segments::menuButtonClick(){
    clockState.changeStateTo(MENU_HOUR);
}
void LedClockOn7Segments::menuNextButtonClick(){
    //TODO:
}
void LedClockOn7Segments::menuPlusButtonClick(){
    //TODO:
}
void LedClockOn7Segments::menuMinusButtonClick(){
   //TODO:
}

LedClockOn7Segments ledClock = LedClockOn7Segments();
