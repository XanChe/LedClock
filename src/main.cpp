#include <LedClockOn7Segments.h>


CRGBArray<NUM_LEDS> leds;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( 255 );
  FastLED.setTemperature( TEMPERATURE_1 );
  FastLED.setBrightness(  255 );
  ledClock.setCurTime(8,58,33);
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