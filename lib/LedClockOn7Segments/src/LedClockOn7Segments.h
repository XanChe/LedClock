#ifndef LedClockOn7Segments_h
#define LedClockOn7Segments_h
#include <Arduino.h>
#include <LibEnums.h>
#include <TemperatureSensorStats.h>
#include <ClockState.h>
#include <LibConfig.h>
#include <FastLED.h>

#define NUM_LEDS 88     
#define NUM_ICON_LEDS 14 
#define DATA_PIN 8
#define DATA_ICON_PIN 7
#define SEGMENT_LED_COUNT 3
#define DOTES_LED_COUNT 2
#define BLINK_MENU_FR 3// разз в две мигания точек
#define COLOR_COUNT 256

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky
#define BRIGHTNESS  254

#define CHAR_UP_ZERO 10
#define CHAR_C 11



struct LedPixel
{
    bool isShowed = false;
    CRGB color = CRGB::Green;
    showingLedEffects effect = DAYLY;
};
 

class LedClockOn7Segments{
    private:
        
        byte curentHour = 0;
        byte curentMinute = 0;
        byte curentSecond = 0;
        unsigned long curentTime = 0;
        unsigned long curentDateTimeInMinutes = 0;
       
        unsigned long cronCounter = 0;      

        void (*timeUpdateCallbackFunction)();

        unsigned long mil =0;

        CFastLED CurFastLED;

        ClockState clockState = ClockState(CUR_TIME);
       // clockStates state = CUR_T_OUTDOOR;
        

        LedPixel ledIcons[NUM_ICON_LEDS];
        LedPixel ledMain[NUM_LEDS];             
        
        CHSV subZeroColor = CHSV(HUE_BLUE, 255, 255);
        CHSV plusZeroColor = CHSV(HUE_ORANGE, 255, 255);
        CHSV clockColor = CHSV(HUE_GREEN, 255, 255);
        
        bool isDisplayModifyded = false;
        bool isIconsModifyded = false;
        
        void drowLedSegment(LedPixel ledArray[], byte start, byte count, CRGB color, showingLedEffects effect, bool isShowed = true);
        void drowNumber(int startindex, byte number, showingLedEffects effect = DAYLY);
        void drowHour(byte hour, showingLedEffects effect = DAYLY);
        void drowMinutes(byte minutes, showingLedEffects effect = DAYLY);
        
        void drowDotes(showingLedEffects effect = BLINK);
        void drowSign();

        void drowTemperatureIfCan(TemperatureSensorStats tStats);

        void drowCurentState();

       // CRGB applyEffectsToDisplayLedByIndex(byte i);
        CRGB applyPixelEffect(LedPixel ledPixel);
        
        void setCurentTemperature(TemperatureSensorStats &tStats, float t);
        
    public:
        
        TemperatureSensorStats outdoorStats = TemperatureSensorStats(OUTDOOR_T);
        TemperatureSensorStats indoorStats = TemperatureSensorStats(INDOOR_T);
        
        
        void assignFastLED(CFastLED fLED);        
        void setStateTo(clockStates state);
        void setTimeUpdateCallbackFunction(void (*func)());
        void setCurTime(byte day, byte hour, byte minutes, byte seconds);
        void setCurentIndoorTemperature(float t);
        void setCurentOutdoorTemperature(float t);
        void tick();

        void switchModeButtonClick();
        void statsButtonClick();
        void menuButtonClick();
        void menuNextButtonClick();
        void menuPlusButtonClick();
        void menuMinusButtonClick();

        void clearDispley();
        void clearIcons();

        void drowTimeOnDispley(showingLedEffects effect = DAYLY);
        void drowTimeOnDispley( byte hour, byte minutes, showingLedEffects effect = DAYLY);
        
        void drowTemperatureOnDispley(int t);        

        void drowIcon(icons icon);        

        void render(CRGB displayLed[]);
        void renderIcons(CRGB displayLed[]);


};

extern LedClockOn7Segments ledClock;

#endif