#ifndef LedClockOn7Segments_h
#define LedClockOn7Segments_h
#include <Arduino.h>
//#include <libConfig.h>
#include <FastLED.h>

#define NUM_LEDS 86     
#define DATA_PIN 8
#define SEGMENT_LED_COUNT 1
#define DOTES_LED_COUNT 2
#define BLINK_MENU_FR 3// разз в две мигания точек
#define COLOR_COUNT 256

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky
#define BRIGHTNESS  254

enum clockStates {
    CUR_TIME,
    CUR_T_OUTDOOR,
    CUR_T_INDOOR,
    MENU,
};

enum showingLedEffects {
    DAYLY,
    NIGHTLY,
    BLINK,
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
        

        unsigned long mil =0;

        CFastLED CurFastLED;

        clockStates state = CUR_TIME;

        CRGB ledSource[NUM_LEDS];
        showingLedEffects ledEffects[NUM_LEDS];
        CRGB clockColor = CRGB::Green;

        
        bool isDisplayModifyded = 0;

        void drowNumber(int startindex, byte number, showingLedEffects effect = DAYLY);
        void drowHour(byte hour, showingLedEffects effect = DAYLY);
        void drowMinute(byte minute, showingLedEffects effect = DAYLY);
        void drowDotes();
        void drowSign(int8_t t);
        CRGB applyEffectsToDisplayLedByIndex(byte i);
        
        void regularRender();
    public:
        unsigned long cronCounter = 0;
        void cron();
        void assignFastLED(CFastLED fLED);
        void tic();
        void setCurTime(byte hour, byte minutes, byte seconds);
        void clearDispley();
        void clearIcons();
        void drowTimeOnDispley(showingLedEffects effect = DAYLY);
        void drowTimeOnDispley(byte hour, byte minutes, showingLedEffects effect = DAYLY);
        
        void drowTemperatureOnDispley(int8_t t);
        void drowIcon(icons icon);
        void setStateTo(clockStates state);

        void render(CRGB displayLed[]);


};
extern LedClockOn7Segments ledClock;

#endif