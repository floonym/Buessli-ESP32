#ifndef expander_h
#define expander_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

class  Expander
{
private:
  Adafruit_MCP23X17 mcp1;
  Adafruit_MCP23X17 mcp2;

  void out_pull() {
    
    Wire.beginTransmission(0x20);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(0x20);
    Wire.write(0x01);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(0x27);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(0x27);
    Wire.write(0x01);
    Wire.write(0x00);
    Wire.endTransmission();
    

    Wire.beginTransmission(0x20);
    Wire.write(0x0C);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(0x20);
    Wire.write(0x0D);
    Wire.write(0xFF);
    Wire.endTransmission();

    Wire.beginTransmission(0x27);
    Wire.write(0x0C);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(0x27);
    Wire.write(0x0D);
    Wire.write(0xFF);
    Wire.endTransmission();
  }

    

public:
  void setup() {
    Wire.begin();
    if (!mcp1.begin_I2C(0x20, &Wire)) {
      Serial.println("Error1.");
      while (1);
    } else {
      Serial.println("Success");
    }
    if (!mcp2.begin_I2C(0x27, &Wire)) {
      Serial.println("Error2.");
      while (1);
    } else {
      Serial.println("Success");
    }

    out_pull();
    
  }

  uint8_t write(uint8_t pin, bool state){
    uint8_t fail = 0;
    Serial.println((String)"Pin:"+pin+" State:"+state);
    
    if (pin<16) {
      while (fail++ < 8) {
        mcp1.digitalWrite(pin, state);
        if (mcp1.digitalRead(pin)==state) {
          return 1;
        }
        Serial.println((String)"Failed:"+fail+"   Pin:"+pin+" State:"+state);
        delay(200);
      }
    } else if (pin<32) {
      while (fail++ < 8) {
        mcp2.digitalWrite(pin-16, state);
        if (mcp2.digitalRead(pin-16)==state) {
          return 1;
        }
        Serial.println((String)"Failed:"+fail+"   Pin:"+(pin-16)+" State:"+state);
        delay(200);
      }
    } else { //pin not in range
      return 2;
    } //failed to set pin
    return 0;
  }
};

#endif