#define FASTLED_ALLOW_INTERRUPTS 0

#include <LedClockOn7Segments.h>
#include <math.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RTClib.h>
#include <OneButton.h>
#define DS_TEMP_TYPE float
#include <microDS18B20.h>
#include <Gyver433.h>

//Gyver433_RX<2, 20> rx;

RTC_DS3231 rtc;

MicroDS18B20<12> sensor;

CRGBArray<NUM_LEDS> leds;
CRGBArray<NUM_ICON_LEDS> ledIcons;

OneButton switchModeButton(9, true);
OneButton statsButton(10, true);




void updateTime(){
  /*RtcDateTime now = Rtc.GetDateTime(); 
  
  ledClock.setCurTime((byte)now.Day(), (byte)now.Hour(), (byte)now.Minute(), (byte)now.Second());*/
  DateTime now = rtc.now();
  ledClock.setCurTime((byte)now.day(), (byte)now.hour(), (byte)now.minute(), (byte)now.second());
  
}
unsigned long mm =0;
void requestTemp(){
  mm = millis();
  sensor.requestTemp();
  Serial.print(1);
  Serial.print(" \t");

}

void getAnswerTemp(){
 
  /*Serial.print((millis() - mm));
  Serial.print(" \t");*/
  ledClock.setCurentIndoorTemperature(round(sensor.getTemp()));
  Serial.println(sensor.getTemp());
}
void start_clockDS3231(){
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
}
void clickSwitchMenu(){
  Serial.println("CLick1");
  ledClock.switchModeButtonClick();
}
void clickStatsButton(){
  Serial.println("CLick Stats");
  ledClock.menuButtonClick();
}
// тикер вызывается в прерывании
void isr() {
 // rx.tickISR();
}

void setup() {
  Serial.begin(9600);
  
  start_clockDS3231();
  sensor.setResolution(11);
  //attachInterrupt(0, isr, CHANGE);


  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2811, DATA_ICON_PIN, GRB>(ledIcons, NUM_ICON_LEDS);
  FastLED.setBrightness( 15 );
  FastLED.setTemperature( TEMPERATURE_1 );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  ledClock.assignFastLED(FastLED);
  ledClock.attachMainLedsArray(leds);
  ledClock.attachIconLedsArray(ledIcons);
  ledClock.attachTimeUpdateFunction(updateTime);
  ledClock.attachRequestTempFunction(requestTemp);
  ledClock.attachGetAnswerTempFunction(getAnswerTemp);
  updateTime();
  //ledClock.setCurTime(1,8,58,33);
  ledClock.setCurentIndoorTemperature(23);
  //ledClock.setCurentOutdoorTemperature(-15);
  
  

  
 // ledClock.outdoorStats.putCurrentTemperature(1510,1,10,-25.0);
  
  delay(1000);
  updateTime();
  //ledClock.setCurentOutdoorTemperature(-15);
  ledClock.setCurentIndoorTemperature(23);
 
   switchModeButton.attachClick( clickSwitchMenu);
   statsButton.attachClick(clickStatsButton);
  // put your setup code here, to run once:
}

void loop() {
 /*if (rx.gotData()) {   // если больше 0    
    // ЧИТАЕМ. СПОСОБ 1
    // я знаю, что передатчик отправляет char[15]
    char data;
    
    // читаем принятые данные в data
    // если данные совпадают по размеру - ок
    if (rx.readData(data)) Serial.print(data);
    else Serial.print("Data error");

    
  }*/
  switchModeButton.tick();
  statsButton.tick();
 
  
  ledClock.tick();
  //ledClock.drowHour(88,PLUS_ZERO);
  //ledClock.drowMinutes(88,SUB_ZERO);

  //ledClock.drowTemperatureOnDispley(-35);
  //ledClock.renderIcons(ledIcons);
  ledClock.render();
  
  

}


