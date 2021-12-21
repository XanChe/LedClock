#define FASTLED_ALLOW_INTERRUPTS 0
//#define DEBAG
#ifdef DEBAG 
#define logg(x) Serial.println(x)
#else
#define logg(x) 
#endif
#define FAST_ALGORITHM
#include "LedClockOn7Segments.h"
#include <math.h>
#define DS_TEMP_TYPE float
#include "microDS18B20.h" // для работы с датчиком температуры на борту
#include <SPI.h>
#include "RH_NRF905.h" // для работы с радио модулем
#include "GyverEncoder.h" 
#include "microDS3231.h"// для работы с модулем реального времени

#define CLK 3   // куда подключать энкодер
#define DT 4    // ---
#define SW 5    // ---
#define DATA_PIN 6 //основная лента
#define DATA_ICON_PIN 7 // лента с иконками

MicroDS3231 rtc;
Encoder enc1(CLK, DT, SW); 
RH_NRF905 nrf905;
MicroDS18B20<A0> sensor;

CRGBArray<NUM_LEDS> leds;
CRGBArray<NUM_ICON_LEDS> ledIcons;

void configurateFastLED();
void configurateLedClock();
void configurateControls();
void configurateSensors();

void updateTime();
void saveTime(byte, byte, byte);

void requestTemp();
void getAnswerTemp();

void start_clockDS3231();

void clickSwitchMenu();
void clickStatsButton();
void checkRemoteSensor();
void controlInspection();

void setup() {
    Serial.begin(9600);

    start_clockDS3231();
    configurateControls();
    configurateSensors();
    configurateFastLED();
    configurateLedClock();
    /*
    updateTime();   
    ledClock.setCurentOutdoorTemperature(-15);
    delay(1000);
    updateTime();
    ledClock.setCurentOutdoorTemperature(-15);*/
    delay(1000);
}

void loop() {
    checkRemoteSensor();
    controlInspection();
    ledClock.tick();
    
}
void controlInspection(){
    
    enc1.tick();
    if(enc1.isRight()) ledClock.menuPlusButtonClick();
    if(enc1.isLeft())   ledClock.menuMinusButtonClick();
    if(enc1.isHolded()) ledClock.menuButtonClick();
    if(enc1.isSingle())  ledClock.menuNextButtonClick();
}
void checkRemoteSensor(){
    if (nrf905.available()){
        // Should be a message for us now   
        uint8_t buf[RH_NRF905_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (nrf905.recv(buf, &len)){
            logg("got request: ");
            int t = buf[1];
            if(buf[0] == 1) t *= -1;
            ledClock.setCurentOutdoorTemperature(t);
            logg(t);            
        }
        else{
            logg("recv failed");
        }
    }
}

void configurateControls(){
    enc1.setType(TYPE2);
    enc1.setTickMode(false);
}

void configurateSensors(){
    sensor.setResolution(11); // точность датчика температуры на борту

    if (!nrf905.init()){    // подключение к радиомодулю
        logg("RF init failed");
    }else{
        logg("RF init sacsess");
    }
}

void configurateFastLED(){
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, DATA_ICON_PIN, GRB>(ledIcons, NUM_ICON_LEDS);
    FastLED.setBrightness( 15 );
    FastLED.setTemperature( TEMPERATURE_1 );
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
}

void configurateLedClock(){
    ledClock.attachFastLED(FastLED);
    ledClock.attachMainLedsArray(leds);
    ledClock.attachIconLedsArray(ledIcons);
    ledClock.attachGetTimeFunction(updateTime);
    ledClock.attachRequestTempFunction(requestTemp);
    ledClock.attachGetAnswerTempFunction(getAnswerTemp);
}

void updateTime(){  
    ledClock.setCurTime((byte)rtc.getDay(), (byte)rtc.getHours(), (byte)rtc.getMinutes(), (byte)rtc.getSeconds());
}

void saveTime(byte hour, byte minute, byte second){
    rtc.setTime(second, minute, hour, rtc.getDay(),rtc.getMonth(), rtc.getYear());
}

unsigned long mm =0;
void requestTemp(){
    mm = millis();
    sensor.requestTemp();
}
void getAnswerTemp(){
    ledClock.setCurentIndoorTemperature(round(sensor.getTemp()));
    logg(sensor.getTemp());
}
void start_clockDS3231(){
    logg("Start clock");
    if (!rtc.begin()) {
        logg("DS3231 not found");
        for(;;);
    }    
    // АВТОМАТИЧЕСКАЯ УСТАНОВКА ВРЕМЕНИ  
    if (rtc.lostPower()){// при потере питания        
        // визуально громоздкий, но более "лёгкий" с точки зрения памяти способ установить время компиляции
        rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
    }
    logg("Clock hardware enabled");    
}
void clickSwitchMenu(){
    logg("CLick1");
    ledClock.switchModeButtonClick();
}
void clickStatsButton(){
    logg("CLick Stats");
    ledClock.menuButtonClick();
}


