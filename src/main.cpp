#include <LedClockOn7Segments.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

CRGBArray<NUM_LEDS> leds;

void updateTime(){
  RtcDateTime now = Rtc.GetDateTime();
  ledClock.setCurTime(now.Hour(), now.Minute(), now.Second());
}

void start_clockDS3231(){
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
    }
}

void setup() {
  Serial.begin(9600);
  
  start_clockDS3231();
  
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( 255 );
  FastLED.setTemperature( TEMPERATURE_1 );
  

  ledClock.setCurTime(8,58,33);
  ledClock.setTimeUpdateCallbackFunctionName(updateTime);
  ledClock.drowTimeOnDispley();
  
  // put your setup code here, to run once:
}

void loop() {
  int min = 60;
  if(millis()/1000%2==0){
    ledClock.setCurTime(10,min++%60,33);
    ledClock.drowTimeOnDispley();
    
   // Serial.println(millis()/1000%60);
    //leds[millis()/1000%60] = CRGB::Green;
  }

  Serial.println(ledClock.cronCounter);
  ledClock.tic();
  ledClock.render(leds);
}