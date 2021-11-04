#ifndef component_h
#define component_h


class  Component
{
private:
  bool state;
  String id;
  String pin;
  bool visible = 0;
  uint32_t* tArduinoEnable;
  byte* arduinoEnablePin;
  Component* secondComponent;
  bool exclusive;
  bool delayed;

public:
  //(ID, State)
  void setup(String idIn, String pinIn, bool stateIn, uint32_t* tArduinoEnableIn, byte* arduinoEnablePinIn) {
    id = idIn;
    state = stateIn;
    pin = pinIn;
    tArduinoEnable = tArduinoEnableIn;
    arduinoEnablePin = arduinoEnablePinIn;
    setState(state);
  }


  
  //Get current State
  bool getState() {
    return state;
  }

  //Refreshes Display State
  void displayRefresh() {
    if(visible) {
      Serial2.print(id);
      Serial2.print(".bco=");
      if (state) Serial2.print("1032");
      if (!state) Serial2.print("50712");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
    }
  }

  void setVisibility(bool visIn) {
    visible = visIn;
    displayRefresh();
  }

  //Sets State
  void setState(bool stateIn) {
    state = stateIn;

    if(exclusive && stateIn) { //Exclusive Relay Shudown
      secondComponent->setState(0);
      delay(500);
    }

    String out = pin;
    out += state;
    if(millis()-*tArduinoEnable>5000) { //Waking Arduino if not enabled
      digitalWrite(*arduinoEnablePin,1);
      digitalWrite(LED_BUILTIN,1);
      delay(100);
    }
    *tArduinoEnable = millis();
    Serial1.println(out);
    Serial.println(out);

    if(delayed) { //Delayed Second Relay
      if(state) {
        delay(500);
        secondComponent->setState(1);
      } else {
        secondComponent->setState(0);
      }
    }

    displayRefresh();
  }

  //Switches State and sets it
  void switchState() {
    state = !state;
    setState(state);
  }
  
  //Check if idIn is id of component
  bool checkId(String* idIn) {
    if((*idIn).substring(0,4) == id){
      return true;
    } else {
      return false;
    }
  }

  //Check if idIn is id of component, switches state if true
  bool checkIdSwitch(String* idIn) {
    if(checkId(idIn)){
      switchState();
      return 1;
    } else {
      return 0;
    }
  }


};

#endif