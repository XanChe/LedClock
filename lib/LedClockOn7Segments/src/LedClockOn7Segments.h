#ifndef LedClockOn7Segments_h
#define LedClockOn7Segments_h
#include <Arduino.h>
#include <LibEnums.h>
#include <TemperatureSensorStats.h>
//#include <ClockState.h>
#include <LibConfig.h>
#include <ClockMenu.h>
#include <Settings.h>
#include <ClockDisplay.h>

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

    TemperatureSensorStats outdoorStats = TemperatureSensorStats(OUTDOOR_T, 12);    // Накопиль статистики наружной температуры
    TemperatureSensorStats indoorStats = TemperatureSensorStats(INDOOR_T, 60);      // Тоже самое для датчика на борту
    
    DisplaySettings displaySettings;
    ClockDisplay display = ClockDisplay(displaySettings);

    unsigned long cronCounter = 0; // счетчик циклов, условно - длится 50 млс. 
                                    // нужен для распределения циклических операций во времени
    void (*timeUpdateCallbackFunction)(); /* функция обратного вызова в которой необходимо вызвать setCurTime, 
                                            позволят отвязаться от конкретного железа и библиотек*/
    void (*setHardwareClockTo)(byte hour, byte minute, byte second);
    void (*requestTempCallbackFunction)(); // обратный вызов онборд датчика температуры: запрос к датчику
    void (*getAnswerTempCallbackFunction)();// /--/: получение данных с датчика, вызывается ~ через секунду после запроса 

    unsigned long mil =0; // нужен для инкримента cronCounter каждые 50 млс

    clockStates state = CUR_TIME; // текущее состояние часов
    unsigned long stateStartMilles = 0; // когда часы перешли в тек. состояние
    byte workPeriodInSeconds = 15; // сколько положено работать в тек. состоянии

       
    ClockMenu *menu = NULL; // ссылка на меню, для режима MENU_*

            // для CUSTOM_COLOR эффекта


    /**
     * методы управление состоянием часов
     * */  
    void changeStateTo(clockStates st, byte worckDuration = 15); 
    clockStates changeNextAvailable();
    clockStates getClockState();
    bool checkStateAvailable(clockStates st);

  
    bool drowTemperatureIfCan(TemperatureSensorStats tStats);
    
    bool isMenuMode();

    void drowMenuState();
    void drowClockState();
    void drowCurentState(); //полная отрисовка в зависимости от текущего режима
    void drowCurentStateOnValueChanging();

    void setCurentTemperature(TemperatureSensorStats &tStats, char t);
  
    void saveSettings(DisplaySettings settings);
    DisplaySettings loadSettings();

public:
    LedClockOn7Segments();
    /**
     * Методы нужные для привязки конкретного железа с наружи библиотеки
     * 
     * */
    void attachFastLED(CFastLED &fLED);  
    void attachGetTimeFunction(void (*func)());
    void attachSetTimeToHarwareFunction(void (*func)(byte, byte, byte));
    void attachRequestTempFunction(void (*func)());
    void attachGetAnswerTempFunction(void (*func)());
    void attachMainLedsArray(CRGB ledArray[]);
    void attachIconLedsArray(CRGB ledArray[]);
    
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