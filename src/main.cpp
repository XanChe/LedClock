#define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <LedClockOn7Segments.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RTClib.h>
#include <OneButton.h>

RTC_DS3231 rtc;

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
  
  /*
  Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }*/
}
void clickSwitchMenu(){
  Serial.println("CLick1");
  ledClock.switchModeButtonClick();
}
void clickStatsButton(){
  Serial.println("CLick Stats");
  ledClock.menuButtonClick();
}
void setup() {
  Serial.begin(9600);
  
  start_clockDS3231();
  
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2811, DATA_ICON_PIN, GRB>(ledIcons, NUM_ICON_LEDS);
  FastLED.setBrightness( 15 );
  FastLED.setTemperature( TEMPERATURE_1 );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  ledClock.assignFastLED(FastLED);
  ledClock.attachMainLedsArray(leds);
  ledClock.attachIconLedsArray(ledIcons);
  ledClock.attachTimeUpdateFunction(updateTime);
  updateTime();
  //ledClock.setCurTime(1,8,58,33);
  ledClock.setCurentIndoorTemperature(23);
  ledClock.setCurentOutdoorTemperature(-15);
  
  

  
 // ledClock.outdoorStats.putCurrentTemperature(1510,1,10,-25.0);
  
  delay(1000);
  updateTime();
  ledClock.setCurentOutdoorTemperature(-15);
  ledClock.setCurentIndoorTemperature(23);
 
   switchModeButton.attachClick( clickSwitchMenu);
   statsButton.attachClick(clickStatsButton);
  // put your setup code here, to run once:
}

void loop() {
 
  switchModeButton.tick();
  statsButton.tick();
 
  
  ledClock.tick();
  //ledClock.drowHour(88,PLUS_ZERO);
  //ledClock.drowMinutes(88,SUB_ZERO);

  //ledClock.drowTemperatureOnDispley(-35);
  //ledClock.renderIcons(ledIcons);
  ledClock.render();
  
  

}


