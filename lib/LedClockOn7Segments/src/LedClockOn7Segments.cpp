#include <LedClockOn7Segments.h>

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

void LedClockOn7Segments::tic(){
    if((millis() - mil) >=500){
        cronCounter++;
        mil = millis();

        
    }

}

void LedClockOn7Segments::assignFastLED(CFastLED fLED){
    CurFastLED = fLED;
}


void LedClockOn7Segments::setCurTime(byte hour, byte minutes, byte seconds){
    curentTime = hour * 3600 + minutes * 60 + seconds;
}

void LedClockOn7Segments::clearDispley(){
    for(byte i =0;i < NUM_LEDS; i++){
        ledSource[i] = CRGB::Black;
        ledEffects[i] = DAYLY;
    }
}
void LedClockOn7Segments::drowTimeOnDispley(showingLedEffects effect){
    drowTimeOnDispley(curentTime / 3600, (curentTime % 3600) / 60, effect);
}

void LedClockOn7Segments::drowDotes(showingLedEffects effect){
    for(byte i = SEGMENT_LED_COUNT*7*2; i < SEGMENT_LED_COUNT*7*2 +DOTES_LED_COUNT; i++){
        ledSource[i] = clockColor;
        ledEffects[i] = effect;
    }
}

void LedClockOn7Segments::drowTimeOnDispley(byte hour, byte minutes, showingLedEffects effect){     
    
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
    isDisplayModifyded = true; 

    CRGB segmentColor = CRGB::Black;
    int z = 0;
    for (int i = 0; i < 7; i++) {
        segmentColor = ((numbers[number] & 1 << i) == 1 << i) ? clockColor : CRGB::Black;
        for(int j = 0; j < SEGMENT_LED_COUNT; j++){
            ledSource[z + startindex] = segmentColor;
            ledEffects[z + startindex] = effect;//эффект показа, тут
            z++;
        }
  }
}

void LedClockOn7Segments::regularRender(){
    
}

void LedClockOn7Segments::cron(){
    
    
}
void LedClockOn7Segments::render(CRGB displayLed[]){
    if(cronCounter % 2 == 0){
        for(int i = 0; i < NUM_LEDS; i++){
            displayLed[i] = applyEffectsToDisplayLedByIndex(i);
        }
        isDisplayModifyded = false;
        CurFastLED.show();
    }
    
}

CRGB LedClockOn7Segments::applyEffectsToDisplayLedByIndex(byte i){
    switch (ledEffects[i])
    {
    case DAYLY:
        return ledSource[i];
        break;
    case OFF:
        return  CRGB::Black;
        break;
    case BLINK:
        if(cronCounter % 4 < 2) {
            return ledSource[i];
        }
        else{
            return CRGB::Black;
        }
        break;
    
    default:
        return ledSource[i];
        break;
    }
    
}

LedClockOn7Segments ledClock = LedClockOn7Segments();
