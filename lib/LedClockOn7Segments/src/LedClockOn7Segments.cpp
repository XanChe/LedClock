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


void LedClockOn7Segments::setCurTime(byte hour, byte minutes, byte seconds){
    curentTime = hour * 3600 + minutes * 60 + seconds;
}

void LedClockOn7Segments::drowTime(ledEffects effect = DAYLY){
    drowTime(curentTime / 3600, (curentTime % 3600) / 60, effect);
}

void LedClockOn7Segments::drowTime(byte hour, byte minutes, ledEffects effect = DAYLY){
    byte hl = hour / 10;
    byte hr = hour % 10;
    byte ml = minutes / 10;
    byte mr = minutes % 10;    
    
    drowNumber(0, 8, effect);    
    drowNumber(SEGMENT_LED_COUNT*7, 8, effect);
     
    drowNumber(SEGMENT_LED_COUNT*7*2+2, hr, effect); // +2 сдесь + 2 мигающие точки
    drowNumber(SEGMENT_LED_COUNT*7*3+2, hl, effect);   
}

void LedClockOn7Segments::drowNumber(int startindex, byte number, ledEffects effect = DAYLY){    
    isDisplayModifyded = true; 

    CRGB segmentColor = CRGB::Black;
    int z = 0;
    for (int i = 0; i < 7; i++) {
        segmentColor = ((numbers[number] & 1 << i) == 1 << i) ? clockColor : CRGB::Black;
        for(int j = 0; j < SEGMENT_LED_COUNT; j++){
            ledSource[z + startindex] = segmentColor;
            showEffects[z + startindex] = effect;//эффект показа, тут
            z++;
        }
  }
}

bool LedClockOn7Segments::render(CRGB displayLed[]){
    if(!isDisplayModifyded) return false;
    for(int i = 0; i < NUM_LEDS; i++){
        displayLed[i] = applyEffectsToDisplayLedByIndex(i);
    }
    isDisplayModifyded = false;
   
    return true;
}

CRGB LedClockOn7Segments::applyEffectsToDisplayLedByIndex(byte i){
    if(showEffects[i] == DAYLY) return ledSource[i];
    else return ledSource[i];
}

LedClockOn7Segments ledClock = LedClockOn7Segments();
