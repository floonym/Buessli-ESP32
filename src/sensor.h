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

  //Update Data and refresh Display
  uint16_t getValue() {
    uint16_t dataOld = data;
    data = analogRead(pin)*factor + offset;
    if (data==dataOld) {
      displayRefresh();
    }
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
  void displayRefresh() {
    if(visible) {
      Serial2.print(id);
      Serial2.print(".val=");
      Serial2.print(data);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      delay(50);

      Serial.println((String)"disp pin:"+pin+" data"+data);
    }
  }

  uint16_t getData() {
    return data;
  }

};

#endif