#include <LedClockOn7Segments.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

CRGBArray<NUM_LEDS> leds;
CRGBArray<NUM_ICON_LEDS> ledIcons;
#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}
void updateTime(){
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  
  ledClock.setCurTime((byte)now.Hour(), (byte)now.Minute(), (byte)now.Second());
  

  
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
  
  //start_clockDS3231();
  
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2811, DATA_ICON_PIN, GRB>(ledIcons, NUM_ICON_LEDS);
  FastLED.setBrightness( 15 );
  FastLED.setTemperature( TEMPERATURE_1 );
  
  ledClock.assignFastLED(FastLED);
  ledClock.setCurTime(8,58,33);
  //ledClock.setTimeUpdateCallbackFunction(updateTime);
  ledClock.drowTimeOnDispley();
  
  // put your setup code here, to run once:
}

void loop() {
  
  

  
  ledClock.tic();
  //ledClock.drowHour(88,PLUS_ZERO);
  //ledClock.drowMinutes(88,SUB_ZERO);
  ledClock.render(leds);
  
 
  ledClock.drowIcon(FADE_ALL);
  ledClock.renderIcons(ledIcons);

}


