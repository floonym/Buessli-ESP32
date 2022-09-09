#ifndef sensor_h
#define sensor_h

#include <Arduino.h>


class  Sensor
{
private:
  bool state;
  String id;
  uint8_t pin;
  uint16_t data;
  float factor;
  float offset;
  bool visible;
  uint32_t tLastUpdate;

public:
  //(ID, State, Factor, Offset)
  void setup(String idIn, uint8_t pinIn, float factorIn, float offsetIn) {
    id = idIn;
    pin = pinIn;
    factor = factorIn;
    offset = offsetIn;
    pinMode(pin,INPUT);
  }

  void setVisibility(bool visIn) {
    if (!visible && visIn) {
      visible = visIn;
      displayRefresh();
    }
    visible = visIn;
    
  }

  void getValue() {
    data = analogRead(pin)*factor + offset;
    displayRefresh();
    tLastUpdate = millis();
  }

  //Check if idIn is id of component
  bool checkId(String idIn) {
    if(idIn == id){
      return true;
    } else {
      return false;
    }
  }

  //Refreshes Display State
  void displayRefresh() {
    if(visible) {
      Serial2.print(id);
      Serial2.print(".val=");
      Serial2.print(data);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial.print("disp");
      Serial.println(data);
    }
  }

  uint16_t getData() {
    return data;
  }

  uint32_t getLastUpdate() {
    return tLastUpdate;
  }
};

#endif