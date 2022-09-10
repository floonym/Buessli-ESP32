#ifndef component_h
#define component_h

#include <Arduino.h>
#include <expander.h>


class  Component
{
private:
  bool state;
  String id;
  uint8_t pin;
  bool visible = 0;
  Component* secondCExclusive;
  Component* secondCDelay;
  bool exclusive;             //If secondComponent is to be disabled if this is enabled
  bool delayed;               //If secondComponent is to be enabled after this is enabled
  Expander* Ex;


public:
  //(Pin, State, ID)
  void setup(String idIn, uint8_t pinIn, bool stateIn, Expander* ExIn) {
    id = idIn;
    pin = pinIn;
    Ex = ExIn;
    setState(stateIn);
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
      delay(50);
    }
  }

  void setVisibility(bool visIn) {
    visible = visIn;
    displayRefresh();
  }

  bool getVisibility() {
    return visible;
  }

  //Sets State
  void setState(bool stateIn) {
    bool stateOld = state;
    state = stateIn;

    if(exclusive && stateIn) { //Exclusive Relay Shudown
      secondCExclusive->setState(0);
      delay(200);
    }

    if (Ex->write(pin,state) == 0) {
      Serial.println("Cant reach Expander, Rebooting");
      delay(100);
      ESP.restart();
    }

    if(delayed) { //Delayed Second Relay
      if(state) {
        delay(500);
        secondCDelay->setState(1);
      } else {
        secondCDelay->setState(0);
      }
    }

    if (stateOld!=state) {
      displayRefresh();
    }

  }

  //Switches State and sets it
  void switchState() {;
    setState(!state);
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

  String getID() {
    return id;
  }


};

#endif