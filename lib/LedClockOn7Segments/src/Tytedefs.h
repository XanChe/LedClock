#ifndef Typedefs_h
#define Typedefs_h
#include <LibEnums.h>
/*
struct DisplaySettings{
    byte brightness = 20;
    CHSV subZeroColor = CHSV(HUE_BLUE, 255, 255);       // цвет отрисовки температуры 
    CHSV plusZeroColor = CHSV(HUE_ORANGE, 255, 255);    // цвет отрисовки температуры
    CHSV clockColor = CHSV(HUE_GREEN, 255, 255);        // цвет отрисовки времени 
    CHSV custormColor = CHSV(HUE_GREEN, 255, 255);
};*/

struct DisplaySettings{
    int8_t brightness = 60;
    int8_t subZeroColor = 0;       // цвет отрисовки температуры 
    int8_t plusZeroColor = 20;    // цвет отрисовки температуры
    int8_t clockColor = 150;        // цвет отрисовки времени 
    int8_t custormColor = 43;
    DisplaySettings operator=(DisplaySettings const &other){
        brightness = other.brightness;
        subZeroColor = other.subZeroColor;
        plusZeroColor = other.plusZeroColor;
        clockColor = other.clockColor;
        custormColor = other.custormColor;
        return *this;
    }
    int8_t getBrightness(){
        if(brightness < 60) brightness = 60;
        return brightness;
    }
};

struct LedPixel{
    bool isShowed = false;
    int8_t color = 0;
    showingLedEffects effect = DAYLY;
};

/*struct LedPixel{
    bool isShowed = false;
    CRGB color = CRGB::Green;
    showingLedEffects effect = DAYLY;
};*/

#endif