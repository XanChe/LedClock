#ifndef Typedefs_h
#define Typedefs_h
#include <FastLED.h>
#include <LibEnums.h>

struct DisplaySettings{
    byte brightness = 20;
    CHSV subZeroColor = CHSV(HUE_BLUE, 255, 255);       // цвет отрисовки температуры 
    CHSV plusZeroColor = CHSV(HUE_ORANGE, 255, 255);    // цвет отрисовки температуры
    CHSV clockColor = CHSV(HUE_GREEN, 255, 255);        // цвет отрисовки времени 
    CHSV custormColor = CHSV(HUE_GREEN, 255, 255);
};

struct LedPixel{
    bool isShowed = false;
    CRGB color = CRGB::Green;
    showingLedEffects effect = DAYLY;
};

#endif