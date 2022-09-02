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
  Component* secondCExclusive;
  Component* secondCDelay;
  bool exclusive;             //If secondComponent is to be disabled if this is enabled
  bool delayed;               //If secondComponent is to be enabled after this is enabled


public:
  //(Pin, State, ID)
  void setup(String idIn, String pinIn, bool stateIn) {
    id = idIn;
    state = stateIn;
    pin = pinIn;
    setState(state);
  }

  //(&tArduinoEnable, &arduinoEnablePin)
  void setupArduino(uint32_t* tArduinoEnableIn, byte* arduinoEnablePinIn) {
    tArduinoEnable = tArduinoEnableIn;
    arduinoEnablePin = arduinoEnablePinIn;
  }

  //(&secondCIn)
  void setupSecondCExclusive(Component* secondCIn) {
    secondCExclusive = secondCIn;
    exclusive = 1;
  }
  //(&secondCIn)
  void setupSecondCDelay(Component* secondCIn) {
    secondCDelay = secondCIn;
    delayed = 1;
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
      secondCExclusive->setState(0);
      delay(200);
    }

    String out = pin;
    out += state;
    if(millis()-*tArduinoEnable>5000) { //Waking Arduino if not enabled
      digitalWrite(*arduinoEnablePin,1);
      digitalWrite(LED_BUILTIN,1);
      delay(500);
    }
    *tArduinoEnable = millis();
    Serial1.println(out);
    Serial.println(out);
    delay(100);

    if(delayed) { //Delayed Second Relay
      if(state) {
        delay(500);
        secondCDelay->setState(1);
      } else {
        secondCDelay->setState(0);
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