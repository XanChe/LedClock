#ifndef ClockDisplay_h
#define ClockDisplay_h
#include <Arduino.h>
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
#define CHAR_DOWN_ZERO 12
#define CHAR_DOWN_P 13
#define CHAR_DOWN_R 14

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
        0b00001111, // CHAR_DOWN_ZERO
        0b01111100, // CHAR_DOWN_P
        0b01110101, // CHAR_DOWN_R
    };


class ClockDisplay
{
private:
     
    byte static const COLOR_BLACK = 0;
    byte static const COLOR_ICONS = 155;
    LedPixel ledIcons[NUM_ICON_LEDS];   // масив, что и как отрисовывать на основном дисплее. Сюда всё рисуется
    LedPixel ledMain[NUM_LEDS];         // тоже самае для иконок

     // тоже самае для иконок   
    
    DisplaySettings settings;
     
   // CustomLED CurFastLED;
    void (*setLedColor)(byte index, byte color, byte brigth);
    void (*setLedIconColor)(byte index, byte color, byte brigth);
    void (*showMainLed)(byte);
    void (*showIconLed)(byte);
    /**
     * Методы отрисовки отдельных элементов на главном дисплее
     * */
    void clearDispley();
    void clearIcons();
    void drowLedSegment(LedPixel ledArray[], byte start, byte count, byte color, showingLedEffects effect, bool isShowed = true);
    void drowNumber(byte startindex, byte number, showingLedEffects effect = DAYLY);
    void drowHour(byte hour, showingLedEffects effect = DAYLY);
    void drowMinutes(byte minutes, showingLedEffects effect = DAYLY);        
    void drowDotes(showingLedEffects effect = BLINK);
    void drowSign();
           
    void fillNubreByColorPallete(byte start, byte colorIndex, byte colorPallete[]);
    void fillColorThreeHorizonLines(byte start, byte colorIndex, byte colorPallete[]);
    void fillColorToLedSegment(LedPixel ledArray[], byte start, byte count, byte color);
    

    
     /**
     * Методы генерации итогового состояния светодидов для FastLED и отправка отрисованного на ленту
     * */
    byte applyPixelEffect(LedPixel ledPixel);
    void renderStrip();
    void renderIconStrip();
    
        
public:   
    unsigned long cronCounter = 0; // счетчик циклов, условно - длится 50 млс.
    ClockDisplay(){}
    void tick(){
        cronCounter++;
    }

    void setSettings(DisplaySettings settings){
        this->settings = settings;
    }
    void attachSetLedColor(void (*func)(byte, byte, byte));
    void attachSetLedIconColor(void (*func)(byte, byte, byte));
    void attachShowMainLed(void (*func)(byte));
    void attachShowIconLed(void (*func)(byte));
   /*
    void attachFastLED(CustomLED &fLED);
    void attachMainLedsArray(LedData lesArray[]);
    void attachIconLedsArray(LedData lesArray[]);*/
    /**
     * Комплексная отрисовка для каждого из режимов
     * */
    void drowTimeOnDispley(showingLedEffects effect = DAYLY);
    void drowMenuTimeOnDispley(showingLedEffects effect = DAYLY);
    void drowTimeOnDispley( byte hour, byte minutes, showingLedEffects effect = DAYLY);
    void drowMenuTimeOnDispley(byte hour, byte minutes, menuStates st);
     // рисуем иконки 
    void drowTemperatureOnDispley(int t, icons ic);    
    void drowIcon(icons icon);
    void drowVauleOnDispley(byte char1, byte char2, byte value); 
    void drowSettingsBtigthOnDispley();    
    void drowColorPallete(int8_t color);
    void clear();
    void periodicalRender(byte brigth = 0);
    void render(byte brigth = 0);
};

#endif