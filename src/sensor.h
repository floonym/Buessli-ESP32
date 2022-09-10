#ifndef sensor_h
#define sensor_h

#include <Arduino.h>


class  Sensor
{
private:
  String id;
  uint8_t pin;
  uint16_t data;
  uint16_t dataDisp;
  float factor;
  float offset;
  bool visible;

public:
  //(ID, Pin, Factor, Offset)
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
      displayRefresh(1);
    }
    visible = visIn;
    
  }

  //Update Data and refresh Display
  uint16_t getValue() {
    data = analogRead(pin)*factor + offset;
    displayRefresh(0);
    return data;
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
  void displayRefresh(bool force) {
    if((dataDisp!=data || force) && visible) {
      dataDisp = data;
      Serial2.print(id);
      Serial2.print(".val=");
      Serial2.print(dataDisp);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);

      Serial.print((String)"Sens-Disp Pin:"+pin+" Data:"+data);

      delay(50);
    }
  }

  uint16_t getData() {
    return data;
  }

};

#endif