#ifndef led_h
#define led_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <component.h>



class  LED
{
private:
  Component* ledpower; //StepDown LED
  byte PIN; // Pin for Data to LED
  bool powered = 0; // State of StepUp
  byte dim = 100; // Dimming Level
  Adafruit_NeoPixel strip;

  uint32_t white = strip.Color(255, 0, 0, 0);
  uint32_t red = strip.Color(255, 0, 0, 0);
  uint32_t green = strip.Color(255, 0, 0, 0);
  uint32_t blue = strip.Color(255, 0, 0, 0);

public:

void setup(Component* ledpowerin, byte PINin) {
  ledpower = ledpowerin;
  PIN = PINin;
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(270, PIN, NEO_RGBW + NEO_KHZ800);
}

void power(bool x) { //Enable oder Disable Stepdown Converter
  if (powered != x) {
    ledpower->setState(powered);
    if (x) {
      strip.begin();
      strip.setBrightness(dim);
      strip.show();
    }
  }
  powered = x;
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void fillColor(uint32_t c) { //Fill with Color
  if (!powered) {
    power(1);
  }
  colorWipe(c, 0);
}

void setDim(byte dimIn) { //Set Brightness from 0 to 100
  dim = dimIn;
  strip.setBrightness(dim);
}

void fillWhite() {
  fillColor(white);
}



};

#endif