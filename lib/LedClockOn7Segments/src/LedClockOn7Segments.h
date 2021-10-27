#ifndef LedClockOn7Segments_h
#define LedClockOn7Segments_h
#include <Arduino.h>
//#include <libConfig.h>
#include <FastLED.h>


#define NUM_LEDS 88     
#define DATA_PIN 8
#define SEGMENT_LED_COUNT 1
#define DOTES_LED_COUNT 2
#define BLINK_MENU_FR 3// разз в две мигания точек
#define COLOR_COUNT 256

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky
#define BRIGHTNESS  254

#define CHAR_UP_ZERO 10
#define CHAR_C 11

enum clockStates {
    CUR_TIME,
    CUR_T_OUTDOOR,
    CUR_T_INDOOR,
    MENU_MINUTES,
    MENU_HOUR,
};

enum showingLedEffects {
    DAYLY,
    NIGHTLY,
    BLINK,
    OFF,
    SUB_ZERO,
    PLUS_ZERO,
};

enum icons {
    MAX_T,
    MIN_T,
    OUTDOOR_T,
    INDOOR_T,
    SWITCH_BUTTON,
    STAT_BUTTON,
};

class LedClockOn7Segments{
    private:
        
        int curentTime = 0;
        int tOut = -200;
        int tIn  = -200;
        
        void (*timeUpdateCallbackFunction)();

        unsigned long mil =0;

        CFastLED CurFastLED;

        clockStates state = CUR_TIME;

        CRGB ledSource[NUM_LEDS];
        showingLedEffects ledEffects[NUM_LEDS];
        
        CRGB clockColor = CRGB::Green;
        CRGB subZeroColor = CRGB::Blue;
        CRGB plusZeroColor = CRGB::Orange;

        
        bool isDisplayModifyded = 0;

        void drowNumber(int startindex, byte number, showingLedEffects effect = DAYLY);
        void drowHour(byte hour, showingLedEffects effect = DAYLY);
        void drowMinutes(byte minutes, showingLedEffects effect = DAYLY);
        void drowDotes(showingLedEffects effect = BLINK);
        void drowSign();
        CRGB applyEffectsToDisplayLedByIndex(byte i);
        
        
    public:
        unsigned long cronCounter = 0;
        
        void assignFastLED(CFastLED fLED);        
        void setStateTo(clockStates state);
        void setTimeUpdateCallbackFunctionName(void (*func)());
        void setCurTime(byte hour, byte minutes, byte seconds);

        void tic();

        

        void clearDispley();
        void clearIcons();

        void drowTimeOnDispley(showingLedEffects effect = DAYLY);
        void drowTimeOnDispley(byte hour, byte minutes, showingLedEffects effect = DAYLY);
        
        void drowTemperatureOnDispley(int8_t t);
        

        void drowIcon(icons icon);

        

        void render(CRGB displayLed[]);


};
extern LedClockOn7Segments ledClock;

#endif