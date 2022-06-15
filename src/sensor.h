#ifndef sensor_h
#define sensor_h


class  Sensor
{
private:
  bool state;
  String id;
  String pin;
  uint16_t data;
  float factor;
  float offset;
  bool visible;
  uint32_t tLastUpdate;
  uint32_t* tArduinoEnable;
  byte* arduinoEnablePin;

public:
  //(ID, State, Factor, Offset)
  void setup(String idIn, String pinIn, float factorIn, float offsetIn, uint32_t* tArduinoEnableIn, byte* arduinoEnablePinIn) {
    id = idIn;
    pin = pinIn;
    factor = factorIn;
    offset = offsetIn;
    tArduinoEnable = tArduinoEnableIn;
    arduinoEnablePin = arduinoEnablePinIn;
  }

  void setVisibility(bool visIn) {
    visible = visIn;
  }

  void getValue() {
    String out = "";
    out += "v";
    out += pin;
    if(millis()-*tArduinoEnable>5000) {
      digitalWrite(*arduinoEnablePin,1);
      digitalWrite(LED_BUILTIN,1);
      delay(100);
    }
    *tArduinoEnable = millis();
    Serial.println(out);
    Serial1.println(out);
  }

  //Check if idIn is id of component
  bool checkId(String idIn) {
    if(idIn == id){
      return true;
    } else {
      return false;
    }
  }

  bool checkPin(String pinIn) {
    if(pinIn == pin) {
      return true;
    } else {
      return false;
    }
  }

  void checkPinDataLoad(String dataIn) {
    String pinIn = dataIn.substring(1,3);
    if(checkPin(pinIn)) {
      int dat = (dataIn.substring(3,7)).toInt();
      Serial.print("dat: ");
      Serial.println(dat);
      if(dat>=0 && dat<= 1023) {
        data = dat*factor + offset;
        displayRefresh();
        tLastUpdate = millis();
      }
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