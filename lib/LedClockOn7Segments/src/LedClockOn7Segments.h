#ifndef LedClockOn7Segments_h
#define LedClockOn7Segments_h
#include <Arduino.h>
#include <LibEnums.h>
#include <TemperatureSensorStats.h>
//#include <ClockState.h>
#include <LibConfig.h>
#include <ClockMenu.h>
#include <Settings.h>
#include <FastLED.h>

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



struct LedPixel
{
    bool isShowed = false;
    CRGB color = CRGB::Green;
    showingLedEffects effect = DAYLY;
};
 

class LedClockOn7Segments{
    private:
        /**
         *  текущее время
         * */        
        byte curentHour = 0;
        byte curentMinute = 0;
        byte curentSecond = 0;
        //unsigned long curentTime = 0;
        unsigned long curentDateTimeInMinutes = 0; // время в минутах ??
       
        unsigned long cronCounter = 0; // счетчик циклов, условно - длится 50 млс. 
                                       // нужен для распределения циклических операций во времени
        void (*timeUpdateCallbackFunction)(); /* функция обратного вызова в которой необходимо вызвать setCurTime, 
                                                позволят отвязаться от конкретного железа и библиотек*/
        void (*setHardwareClockTo)(byte hour, byte minute, byte second);
        void (*requestTempCallbackFunction)(); // обратный вызов онборд датчика температуры: запрос к датчику
        void (*getAnswerTempCallbackFunction)();// /--/: получение данных с датчика, вызывается ~ через секунду после запроса 

        unsigned long mil =0; // нужен для инкрумента cronCounter каждые 50 млс

        clockStates state = CUR_TIME; // текущее состояние часов
        unsigned long stateStartMilles = 0; // когда часы перешли в тек. состояние
        byte workPeriodInSeconds = 15; // сколько положено работать в тек. состоянии

        CFastLED CurFastLED;  // нужен, чтобы вызвать FastLED.show() по расписанию
        //ClockState clockState = ClockState(CUR_TIME); // 
       
        ClockMenu *menu = NULL; // ссылка на меню, для режима MENU_*

        LedPixel ledIcons[NUM_ICON_LEDS];   // масив, что и как отрисовывать на основном дисплее. Сюда всё рисуется
        LedPixel ledMain[NUM_LEDS];         // тоже самае для иконок

        CRGB *mainLedsArray;    // ссылка на массив светодиодов, тот же, что привязывается и FstLED 
        CRGB *iconLedsArray;    // тоже самае для иконок   
        
        CHSV subZeroColor = CHSV(HUE_BLUE, 255, 255);       // цвет отрисовки температуры 
        CHSV plusZeroColor = CHSV(HUE_ORANGE, 255, 255);    // цвет отрисовки температуры
        CHSV clockColor = CHSV(HUE_GREEN, 255, 255);        // цвет отрисовки времени 
        CHSV custormColor = CHSV(HUE_GREEN, 255, 255);      // для CUSTOM_COLOR эффекта

        byte brightness = BRIGHTNESS; //яркость экрана

        /**
         * методы управление состоянием часов
         * */  
        void changeStateTo(clockStates st, byte worckDuration = 15); 
        clockStates changeNextAvailable();
        clockStates getClockState();
        bool checkStateAvailable(clockStates st);

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
        /**
         * Комплексная отрисовка для каждого из режимов
         * */
        void drowTimeOnDispley(showingLedEffects effect = DAYLY);
        void drowMenuTimeOnDispley(showingLedEffects effect = DAYLY);
        void drowTimeOnDispley( byte hour, byte minutes, showingLedEffects effect = DAYLY);
        bool drowTemperatureIfCan(TemperatureSensorStats tStats);
        void drowTemperatureOnDispley(int t);        
        void drowColorPallete();
        
        void drowIcon(icons icon); // рисуем иконки 

        void drowCurentState(); //полная отрисовка в зависимости от текущего режима
        void drowCurentStateOnValueChanging();
        /**
         * Методы генерации итогового состояния светодидов для FastLED и отправка отрисованного на ленту
         * */
        CRGB applyPixelEffect(LedPixel ledPixel);
        void render(CRGB displayLed[]);
        void renderIcons(CRGB displayLed[]);
        void render(byte brigth = 0);
        
        
        void setCurentTemperature(TemperatureSensorStats &tStats, char t);
        
        void setSettingsFromMenu();
        void saveSettings();
        void loadSettings();

    public:
        LedClockOn7Segments();

        TemperatureSensorStats outdoorStats = TemperatureSensorStats(OUTDOOR_T);    // Накопиль статистики наружной температуры
        TemperatureSensorStats indoorStats = TemperatureSensorStats(INDOOR_T);      // Тоже самое для датчика на борту
        
        /**
         * Методы нужные для привязки конкретного железа с наружи библиотеки
         * 
         * */
        void attachFastLED(CFastLED &fLED);  
        void attachGetTimeFunction(void (*func)());
        void attachSetTimeToHarwareFunction(void (*func)(byte, byte, byte));
        void attachRequestTempFunction(void (*func)());
        void attachGetAnswerTempFunction(void (*func)());
        void attachMainLedsArray(CRGB lesArray[]);
        void attachIconLedsArray(CRGB lesArray[]);
        void setCurTime(byte day, byte hour, byte minutes, byte seconds);
        void setCurentIndoorTemperature(char t);
        void setCurentOutdoorTemperature(char t);

        /**
         * интерфес управления часами
         * */
        void switchModeButtonClick();
        void statsButtonClick();
        void menuButtonClick();
        void menuNextButtonClick();
        void menuPlusButtonClick();
        void menuMinusButtonClick();

        void tick();    // Основной метод вызываемый в loop. Распределяет задачи во времени

};

extern LedClockOn7Segments ledClock;

#endif