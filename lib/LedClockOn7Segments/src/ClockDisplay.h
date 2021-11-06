#ifndef ClockDisplay_h
#define ClockDisplay_h
#include <Arduino.h>

#include <FastLED.h>
#include <Tytedefs.h>

#define NUM_LEDS 88     
#define NUM_ICON_LEDS 14 

#define SEGMENT_LED_COUNT 3
#define SIGN_LED_COUNT 2
#define DOTES_LED_COUNT 2
#define BLINK_MENU_FR 3// разз в две мигания точек
#define COLOR_COUNT 256

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky
#define BRIGHTNESS  20

#define CHAR_UP_ZERO 10
#define CHAR_C 11

/**
 *       ---
 *     |  6  |
 *    7|     |5
 *       ---
 *     |  4  |
 *    3|     |1
 *       ---
 *       2
 **/
const byte numbers[] = {
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



class ClockDisplay
{
private:
    unsigned long cronCounter = 0; // счетчик циклов, условно - длится 50 млс. 

    LedPixel ledIcons[NUM_ICON_LEDS];   // масив, что и как отрисовывать на основном дисплее. Сюда всё рисуется
    LedPixel ledMain[NUM_LEDS];         // тоже самае для иконок

    CRGB *mainLedsArray;    // ссылка на массив светодиодов, тот же, что привязывается и FstLED 
    CRGB *iconLedsArray;    // тоже самае для иконок   
    
    DisplaySettings displaySettings;
     
    CFastLED CurFastLED;
    
    /**
     * Методы отрисовки отдельных элементов на главном дисплее
     * */
    void clearDispley();
    void clearIcons();
    void drowLedSegment(LedPixel ledArray[], byte start, byte count, CRGB color, showingLedEffects effect, bool isShowed = true);
    void drowNumber(int startindex, byte number, showingLedEffects effect = DAYLY);
    void drowHour(byte hour, showingLedEffects effect = DAYLY);
    void drowMinutes(byte minutes, showingLedEffects effect = DAYLY);        
    void drowDotes(showingLedEffects effect = BLINK);
    void drowSign();        
    void fillNubreByColorPallete(byte start, byte colorIndex, CRGB colorPallete[]);
    void fillColorThreeHorizonLines(byte start, byte colorIndex, CRGB colorPallete[]);
    void fillColorToLedSegment(LedPixel ledArray[], byte start, byte count, CRGB color);
    void drowIcon(icons icon);

    
     /**
     * Методы генерации итогового состояния светодидов для FastLED и отправка отрисованного на ленту
     * */
    CRGB applyPixelEffect(LedPixel ledPixel);
    void render(CRGB displayLed[]);
    void renderIcons(CRGB displayLed[]);
    
        
public:   
    
    ClockDisplay(DisplaySettings settings){
        displaySettings = settings;
    }

    void tick(){
        cronCounter++;
    }

    void setSettings(DisplaySettings settings){
        displaySettings = settings;
    }
    
    void attachFastLED(CFastLED &fLED);
    void attachMainLedsArray(CRGB lesArray[]);
    void attachIconLedsArray(CRGB lesArray[]);
    /**
     * Комплексная отрисовка для каждого из режимов
     * */
    void drowTimeOnDispley(showingLedEffects effect = DAYLY);
    void drowMenuTimeOnDispley(showingLedEffects effect = DAYLY);
    void drowTimeOnDispley( byte hour, byte minutes, showingLedEffects effect = DAYLY);
    void drowMenuTimeOnDispley(byte hour, byte minutes, clockStates st);
     // рисуем иконки 
    void drowTemperatureOnDispley(int t, icons ic);        
    void drowColorPallete(int8_t hue);
    void clear();
    void render(byte brigth = 0);
};

#endif