#include <ClockDisplay.h>

void ClockDisplay::clear(){    
    clearDispley();
    clearIcons();    
}

void ClockDisplay::clearDispley(){     
    drowLedSegment(ledMain, 0, NUM_LEDS, 0, OFF, false);    
}

void ClockDisplay::clearIcons(){ 
    drowLedSegment(ledIcons, 0, NUM_ICON_LEDS, 0, OFF, false);    
}

void ClockDisplay::fillColorThreeHorizonLines(byte start, byte colorIndex, byte colorPallete[]){
    byte iBottom = start + SEGMENT_LED_COUNT;
    byte iMiddle = start + 4 * SEGMENT_LED_COUNT -1;
    byte iTop = start + 5 * SEGMENT_LED_COUNT;
    for(byte i = colorIndex; i < colorIndex + SEGMENT_LED_COUNT; i++){
        
        ledMain[iBottom++].color = colorPallete[i];
        ledMain[iMiddle--].color = colorPallete[i];
        ledMain[iTop++].color = colorPallete[i];

    }    
}

void ClockDisplay::fillNubreByColorPallete(byte start, byte colorIndex, byte colorPallete[]){

    fillColorToLedSegment(ledMain,start , SEGMENT_LED_COUNT, colorPallete[colorIndex]);
    fillColorToLedSegment(ledMain,start + 4 * SEGMENT_LED_COUNT, SEGMENT_LED_COUNT, colorPallete[colorIndex]);
    fillColorThreeHorizonLines(start, colorIndex + 1, colorPallete);
    fillColorToLedSegment(ledMain,start + 2 * SEGMENT_LED_COUNT , SEGMENT_LED_COUNT, colorPallete[colorIndex + SEGMENT_LED_COUNT]);
    fillColorToLedSegment(ledMain,start + 6 * SEGMENT_LED_COUNT , SEGMENT_LED_COUNT, colorPallete[colorIndex + SEGMENT_LED_COUNT]);
}

void ClockDisplay::drowColorPallete(int8_t color){

    //byte gradientLen = 9 + SEGMENT_LED_COUNT * 4;
   // fill_gradient(mainLedsArray, 0, CHSV(color - 1, 255, 255), gradientLen , CHSV(color + 1, 255, 255), TGradientDirectionCode::LONGEST_HUES);
    byte dColor = COLOR_COUNT / NUM_LEDS;
    int startGradient = COLOR_COUNT + color - dColor * (NUM_LEDS / 2);
    //byte mainLedsArray[gradientLen];
    for(int i = startGradient, j = 0; j < NUM_LEDS; i += dColor, j++){
        ledMain[j].color = i % COLOR_COUNT;
        /*Serial.print(i % COLOR_COUNT);
        Serial.print(",");*/
        ledMain[j].isShowed = true;
    }
    //Serial.println("");
/*
    fillNubreByColorPallete(0, 0, mainLedsArray);
    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7, SEGMENT_LED_COUNT + 2, mainLedsArray);
    
    fillColorToLedSegment(ledMain,SEGMENT_LED_COUNT * 7 * 2,  DOTES_LED_COUNT, mainLedsArray[(SEGMENT_LED_COUNT * 2 + 4)]);

    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7 * 2 + DOTES_LED_COUNT, SEGMENT_LED_COUNT * 2 + 4 + 1, mainLedsArray);
    fillNubreByColorPallete(SEGMENT_LED_COUNT * 7 * 3 + DOTES_LED_COUNT, SEGMENT_LED_COUNT * 3 + 6 + 1, mainLedsArray);*/
}

void ClockDisplay::fillColorToLedSegment(LedPixel ledArray[], byte start, byte count, byte color){
    for(byte i = start; i < start + count; i++){
            ledArray[i].color = color;            
    }
}

void ClockDisplay::drowLedSegment(LedPixel ledArray[], byte start, byte count, byte color, showingLedEffects effect, bool isShowed){
    
    for(byte i = start; i < start + count; i++){
            ledArray[i].color = color;
            ledArray[i].isShowed = isShowed;
            ledArray[i].effect = effect;
            
    }
}

void ClockDisplay::drowSettingsBtigthOnDispley(){   
    drowVauleOnDispley(CHAR_UP_ZERO, CHAR_DOWN_ZERO, (int)((settings.brightness / 255.0) * 100) - 1);
}

void ClockDisplay::drowVauleOnDispley(byte char1, byte char2, byte value){
    clearDispley();
    Serial.println(value);
    char templ = value / 10;
    char tempr = value % 10; 
    drowNumber(0, char1);    
    drowNumber(SEGMENT_LED_COUNT*7, char2, DAYLY);
    drowDotes(OFF) ;
    drowNumber(SEGMENT_LED_COUNT*7*2+DOTES_LED_COUNT, tempr);
    drowNumber(SEGMENT_LED_COUNT*7*3+DOTES_LED_COUNT, templ); 
}

void ClockDisplay::drowTemperatureOnDispley(int temperature, icons ic){    
    clearDispley();
    clearIcons();
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
    
    drowLedSegment(ledMain, SEGMENT_LED_COUNT*28+DOTES_LED_COUNT, 2, settings.subZeroColor, SUB_ZERO);    
}

void ClockDisplay::drowDotes(showingLedEffects effect){
    
    drowLedSegment(ledMain, SEGMENT_LED_COUNT*7*2, DOTES_LED_COUNT, settings.clockColor, effect);    
}

void ClockDisplay::drowTimeOnDispley(byte hour, byte minutes, showingLedEffects effect){     
    
    clear();    
    drowMinutes(minutes, effect);    
    drowDotes(BLINK);  
    drowHour(hour, effect);     
}

void ClockDisplay::drowMenuTimeOnDispley(byte hour, byte minutes, menuStates st){ 
    clear();    
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
    drowNumber(SEGMENT_LED_COUNT*7, minutes / 10, effect == DAYLY ? DAYLY_CORRECT: effect);
}

void ClockDisplay::drowNumber(byte startindex, byte number, showingLedEffects effect){ 
    byte segmentColor = COLOR_BLACK;
    byte z = 0;
    for (int i = 0; i < 7; i++) {
        bool isShowed = (numbers[number] & 1 << i) == 1 << i;
        segmentColor = isShowed ? settings.clockColor : COLOR_BLACK;
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
    if(setLedIconColor != NULL && showIconLed != NULL) {
        renderIconStrip();
        showIconLed(brigth==0 ? settings.getBrightness(): brigth);
    }
    if(setLedColor != NULL && showMainLed != NULL){
        renderStrip();
        showMainLed(brigth==0 ? settings.getBrightness(): brigth);
    }
}

void ClockDisplay::renderStrip(){  
    for(int i = 0; i < NUM_LEDS; i++){ 
        applyPixelEffect(i, ledMain[i], setLedColor);           
        //setLedColor(i, curColor, curColor == 0 ? 0 : settings.brightness);       
    }        
}

void ClockDisplay::renderIconStrip(){ 
   
    for(int i = 0; i < NUM_ICON_LEDS; i++){
        applyPixelEffect(i, ledIcons[i],setLedIconColor);
    }           
}

void ClockDisplay::applyPixelEffect(byte index, LedPixel ledPixel,void (*func)(byte, byte, byte)){
    byte curColor = 0;
    switch (ledPixel.effect)
    {
    case DAYLY:
        curColor = ledPixel.isShowed ? ledPixel.color : 0;
        func(index, curColor, curColor == 0 ? 0 : 255 - settings.brghtCorrector);        
        break;
    case DAYLY_CORRECT:
        curColor = ledPixel.isShowed ? ledPixel.color : 0;
        func(index, curColor, curColor == 0 ? 0 : 255);        
        break;
    case OFF:
         func(index, curColor, 0);       
        break;
    case SUB_ZERO:
        curColor = ledPixel.isShowed ? settings.subZeroColor : 0;
        func(index, curColor, curColor == 0 ? 0 : 255- settings.brghtCorrector);         
        break;
    case PLUS_ZERO:
        curColor = ledPixel.isShowed ? settings.plusZeroColor : 0;
        func(index, curColor, curColor == 0 ? 0 : 255- settings.brghtCorrector);        
        break;
    case BLINK:       
        if(cronCounter % 40 < 20) {  
            curColor = ledPixel.color;
            
        }else{
            curColor = 0;
        }
        func(index, curColor, curColor == 0 ? 0 : 255- settings.brghtCorrector);
        break;
    case CUSTOM_COLOR:
        curColor = ledPixel.isShowed ? settings.iconsColor : 0;
        func(index, curColor, curColor == 0 ? 0 : 255- settings.brghtCorrector);
        break;
    default:
        curColor = ledPixel.isShowed ? ledPixel.color : 0;
        func(index, curColor, curColor == 0 ? 0 : 255- settings.brghtCorrector);
        break;
    }    
}

void ClockDisplay::drowIcon(icons icon){    
    switch (icon)
    {
    case icons::INDOOR_T:
        drowLedSegment(ledIcons, 6 , 4, settings.iconsColor, DAYLY);        
        break;
    case icons::OUTDOOR_T:
        drowLedSegment(ledIcons, 10 , 4, settings.iconsColor, DAYLY);
        break;
    case icons::MAX_T:
        drowLedSegment(ledIcons, 4 , 1, settings.iconsColor, DAYLY);
        break;
    case icons::MIN_T:
        drowLedSegment(ledIcons, 5 , 1, settings.iconsColor, DAYLY);
        break;
    case icons::SWITCH_BUTTON:
        drowLedSegment(ledIcons, 2 , 2, settings.iconsColor, DAYLY);
        break;
    case icons::STAT_BUTTON:
        drowLedSegment(ledIcons, 0 , 2, settings.iconsColor, DAYLY);
        break;
    case icons::ALL:
        drowLedSegment(ledIcons, 0 , 14, settings.iconsColor, NIGHTLY);
        break;
    case icons::FADE_ALL:
        drowLedSegment(ledIcons, 0 , 14, COLOR_BLACK, OFF, false);
        break;
    default:
        drowLedSegment(ledIcons, 0 , 14, COLOR_BLACK, OFF);
        break;
    }
}

void ClockDisplay::attachSetLedColor(void (*func)(byte, byte, byte)){
    setLedColor = func;
}

void ClockDisplay::attachSetLedIconColor(void (*func)(byte, byte, byte)){
    setLedIconColor = func;
}

void ClockDisplay::attachShowMainLed(void (*func)(byte)){
    showMainLed = func;
}

void ClockDisplay::attachShowIconLed(void (*func)(byte)){
    showIconLed = func;
}

