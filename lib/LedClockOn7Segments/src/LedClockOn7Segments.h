#ifndef LedClockOn7Segments_h
#define LedClockOn7Segments_h
#include <Arduino.h>
//#include <libConfig.h>
#include <FastLED.h>

#define NUM_LEDS 86     
#define DATA_PIN 8
#define SEGMENT_LED_COUNT 3
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

enum ledEffects {
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

        clockStates state = CUR_TIME;

        CRGB ledSource[NUM_LEDS];
        ledEffects showEffects[NUM_LEDS];
        CRGB clockColor = CRGB::Green;

        bool isDisplayModifyded = 0;

        void drowNumber(int startindex, byte number, ledEffects effect = DAYLY);
        CRGB applyEffectsToDisplayLedByIndex(byte i);
    public:
        void tic();
        void setCurTime(byte hour, byte minutes, byte seconds);
        void drowTime(ledEffects effect = DAYLY);
        void drowTime(byte hour, byte minutes, ledEffects effect = DAYLY);
        void drowDotes();
        void drowSign(int8_t t);
        void drowTemperature(int8_t t);
        void drowIcon(icons icon);
        void setStateTo(clockStates state);

        bool render(CRGB displayLed[]);


};
extern LedClockOn7Segments ledClock;

#endif