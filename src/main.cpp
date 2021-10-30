#define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <LedClockOn7Segments.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
#include <OneButton.h>
RtcDS3231<TwoWire> Rtc(Wire);

CRGBArray<NUM_LEDS> leds;
CRGBArray<NUM_ICON_LEDS> ledIcons;

OneButton switchModeButton(9, true);
OneButton statsButton(10, true);


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
  
  ledClock.setCurTime((byte)now.Day(), (byte)now.Hour(), (byte)now.Minute(), (byte)now.Second());
  

  
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
  
  //start_clockDS3231();
  
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2811, DATA_ICON_PIN, GRB>(ledIcons, NUM_ICON_LEDS);
  FastLED.setBrightness( 15 );
  FastLED.setTemperature( TEMPERATURE_1 );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  ledClock.assignFastLED(FastLED);
  ledClock.setCurTime(1,8,58,33);
  ledClock.setCurentIndoorTemperature(23);
  ledClock.setCurentOutdoorTemperature(-15);
  
  

  ledClock.setCurTime(1,1,10,0);
 // ledClock.outdoorStats.putCurrentTemperature(1510,1,10,-25.0);
  ledClock.setCurentOutdoorTemperature(-15);
  delay(1000);
  ledClock.setCurTime(1,1,11,0);
  ledClock.setCurentOutdoorTemperature(-12);
 
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
  ledClock.renderIcons(ledIcons);
  ledClock.render(leds);
  
 
  
  

}


