#include <ClockDisplay.h>

void ClockDisplay::clear(){    
    clearDispley();
    clearIcons();    
}

void ClockDisplay::clearDispley(){     
    drowLedSegment(ledMain, 0, NUM_LEDS, CRGB::Black, OFF, false);    
}

void ClockDisplay::clearIcons(){     
    drowLedSegment(ledIcons, 0, NUM_ICON_LEDS, CRGB::Black, OFF, false);    
}

void ClockDisplay::fillColorThreeHorizonLines(byte start, byte colorIndex, CRGB colorPallete[]){
    byte iBottom = start + SEGMENT_LED_COUNT;
    byte iMiddle = start + 4 * SEGMENT_LED_COUNT -1;
    byte iTop = start + 5 * SEGMENT_LED_COUNT;
    for(byte i = colorIndex; i < colorIndex + SEGMENT_LED_COUNT; i++){
        
        ledMain[iBottom++].color = colorPallete[i];
        ledMain[iMiddle--].color = colorPallete[i];
        ledMain[iTop++].color = colorPallete[i];

    }    
}
void ClockDisplay::fillNubreByColorPallete(byte start, byte colorIndex, CRGB colorPallete[]){

    

    fillColorToLedSegment(ledMain,start , SEGMENT_LED_COUNT, colorPallete[colorIndex]);
    fillColorToLedSegment(ledMain,start + 4 * SEGMENT_LED_COUNT, SEGMENT_LED_COUNT, colorPallete[colorIndex]);
    fillColorThreeHorizonLines(start, colorIndex + 1, colorPallete);
    fillColorToLedSegment(ledMain,start + 2 * SEGMENT_LED_COUNT , SEGMENT_LED_COUNT, colorPallete[colorIndex + SEGMENT_LED_COUNT]);
    fillColorToLedSegment(ledMain,start + 6 * SEGMENT_LED_COUNT , SEGMENT_LED_COUNT, colorPallete[colorIndex + SEGMENT_LED_COUNT]);

}
void ClockDisplay::drowColorPallete(int8_t hue){

    byte gradientLen = 9 + SEGMENT_LED_COUNT * 4;
    fill_gradient(mainLedsArray, 0, CHSV(hue - 1, 255, 255), gradientLen , CHSV(hue + 1, 255, 255), TGradientDirectionCode::LONGEST_HUES);
    fillNubreByColorPallete(0, 0, mainLedsArray);
    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7, SEGMENT_LED_COUNT + 2, mainLedsArray);
    
    fillColorToLedSegment(ledMain,SEGMENT_LED_COUNT * 7 * 2,  DOTES_LED_COUNT, mainLedsArray[(SEGMENT_LED_COUNT * 2 + 4)]);

    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7 * 2 + DOTES_LED_COUNT, SEGMENT_LED_COUNT * 2 + 4 + 1, mainLedsArray);
    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7 * 3 + DOTES_LED_COUNT, SEGMENT_LED_COUNT * 3 + 6 + 1, mainLedsArray);
}

void ClockDisplay::fillColorToLedSegment(LedPixel ledArray[], byte start, byte count, CRGB color){
    for(byte i = start; i < start + count; i++){
            ledArray[i].color = color;
            
    }
}



void ClockDisplay::drowLedSegment(LedPixel ledArray[], byte start, byte count, CRGB color, showingLedEffects effect, bool isShowed){
    for(byte i = start; i < start + count; i++){
            ledArray[i].color = color;
            ledArray[i].isShowed = isShowed;
            ledArray[i].effect = effect;
    }
}



void ClockDisplay::drowTemperatureOnDispley(int temperature, icons ic){
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
    drowIcon(ic);
      
}

void ClockDisplay::drowSign(){
    
    drowLedSegment(ledMain, SEGMENT_LED_COUNT*28+DOTES_LED_COUNT, 2, displaySettings.subZeroColor, SUB_ZERO);    
}
/*
void ClockDisplay::drowTimeOnDispley(showingLedEffects effect){
    drowTimeOnDispley(curentHour, curentMinute, effect);
}*/
/*
void ClockDisplay::drowMenuTimeOnDispley(showingLedEffects effect){
    if(menu != NULL)drowTimeOnDispley(menu->getHour(), menu->getMinute(), effect);
}
*/
void ClockDisplay::drowDotes(showingLedEffects effect){
    
    drowLedSegment(ledMain, SEGMENT_LED_COUNT*7*2, DOTES_LED_COUNT, displaySettings.clockColor, effect);    
}

void ClockDisplay::drowTimeOnDispley(byte hour, byte minutes, showingLedEffects effect){     

    clearDispley();    
    drowMinutes(minutes, effect);    
    drowDotes(BLINK);  
    drowHour(hour, effect);     
}

void ClockDisplay::drowMenuTimeOnDispley(byte hour, byte minutes, menuStates st){ 
    clearDispley();    
    drowMinutes(minutes,st == MENU_MINUTES ? BLINK : DAYLY);    
    drowDotes(OFF);  
    drowHour(hour, st == MENU_HOUR ? BLINK : DAYLY);     
}

void ClockDisplay::drowHour(byte hour, showingLedEffects effect){

    drowNumber(SEGMENT_LED_COUNT*7*2+DOTES_LED_COUNT, hour % 10, effect); // +DOTES_LED_COUNT сдесь + DOTES_LED_COUNT мигающие точки
    drowNumber(SEGMENT_LED_COUNT*7*3+DOTES_LED_COUNT, hour / 10, effect);
}

void ClockDisplay::drowMinutes(byte minutes, showingLedEffects effect){
    drowNumber(0, minutes % 10, effect);    
    drowNumber(SEGMENT_LED_COUNT*7, minutes / 10, effect);
}

void ClockDisplay::drowNumber(int startindex, byte number, showingLedEffects effect){ 
    CRGB segmentColor = CRGB::Black;
    int z = 0;
    for (int i = 0; i < 7; i++) {
        bool isShowed = (numbers[number] & 1 << i) == 1 << i;
        segmentColor = isShowed ? (CRGB)displaySettings.clockColor : CRGB::Black;
        drowLedSegment(ledMain, z + startindex, SEGMENT_LED_COUNT, segmentColor, effect, isShowed);
        z += SEGMENT_LED_COUNT;      
    }
}
void ClockDisplay::periodicalRender(byte brigth){
    if(cronCounter % 10 == 0 ){
        render(brigth);        
    }
    
}
void ClockDisplay::render(byte brigth){
    if(iconLedsArray != NULL) {
        renderIcons(iconLedsArray);
    }
    if(mainLedsArray != NULL){
        render(mainLedsArray);
    }
    CurFastLED.setBrightness(brigth==0 ? displaySettings.brightness: brigth);
    CurFastLED.show();
}

void ClockDisplay::render(CRGB displayLed[]){
    
        //Serial.println(cronCounter);
        for(int i = 0; i < NUM_LEDS; i++){            
            displayLed[i] = applyPixelEffect(ledMain[i]);       
        }
        
}

void ClockDisplay::renderIcons(CRGB displayLed[]){  
        for(int i = 0; i < NUM_ICON_LEDS; i++){
            displayLed[i] = applyPixelEffect(ledIcons[i]);
        }           
}


CRGB ClockDisplay::applyPixelEffect(LedPixel ledPixel){
    switch (ledPixel.effect)
    {
    case DAYLY:
        return ledPixel.isShowed  ? ledPixel.color : CRGB::Black;;
        break;
    case OFF:
        return  CRGB::Black;        
        break;
    case SUB_ZERO:
        return  ledPixel.isShowed  ? (CRGB)displaySettings.subZeroColor : CRGB::Black;
        break;
    case PLUS_ZERO:
        return  ledPixel.isShowed  ? (CRGB)displaySettings.plusZeroColor : CRGB::Black;
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
            ledPixel.isShowed  ? (CRGB)displaySettings.custormColor : CRGB::Black;
        break;
    default:
        return ledPixel.color;
        break;
    }
    
}

void ClockDisplay::drowIcon(icons icon){    
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

void ClockDisplay::attachFastLED(CFastLED &fLED){
    CurFastLED = fLED;
}

void ClockDisplay::attachMainLedsArray(CRGB lesArray[]){
    mainLedsArray = lesArray;
}
void ClockDisplay::attachIconLedsArray(CRGB lesArray[]){
    iconLedsArray = lesArray;
}