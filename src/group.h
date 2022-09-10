#ifndef group_h
#define group_h

#include <component.h>

class  Group
{
private:
  byte state;
  byte stateDisp;
  String id;
  Component* coms[50];
  int len;
  bool visible = 0;

public:
  //(ID, State)
  void setup(String idIn, byte stateIn, Component* comsIn[50], int lenIn) {
    id = idIn;
    state = stateIn;
    len = lenIn;
    for(int w = 0; w<len; w++) {
      coms[w] = comsIn[w];
    }
  }

  //Refresh state and return
  byte actualState() {
    int count = 0;
    for (int i = 0; i<len; i++) {
      count += coms[i]->getState();
    }
    if (count == 0) {
      return 0;
    } else if (count == len) {
      return 1;
    } else {
      return 2;
    }
  }

  // Refresh state and save and return
  byte getState() {
    state = actualState();
    return state;
  }

  void buttonRefresh(bool force) {
    getState();
    if ((force || state != stateDisp) && visible) {
      stateDisp = state;
      Serial2.print(id);
      Serial2.print(".bco=");
      if (state == 2) Serial2.print("64528");
      if (state == 1) Serial2.print("1032");
      if (state == 0) Serial2.print("50712");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);

      Serial.println((String)"Group-Disp ID:"+id+" State:"+state);

      delay(50);
    }
  }

  void setVisibility(bool visIn) {
    visible = visIn;
    buttonRefresh(1);
  }

  void setState(bool stateIn) {
    state = stateIn;
    for (int i = 0; i<len; i++) {
      coms[i]->setState(state);
    }
    buttonRefresh(0);
  }

  void switchState() {
    if(getState() == 1) {
      state = 0;
    } else {
      state = 1;
    }
    setState(state);
  }

  //return 0 if none, 1 if some, 2 if all
  bool checkIdComp(String* idIn) {
    byte c = 0;
    for(byte i = 0; i<len; i++) {
      c += coms[i]->checkId(idIn);
    }
    if(c==0) {
      return 0;
    } else {
      return 1;
    }
  } 

  bool checkId(String* idIn) {
    if((*idIn).substring(0,4) == id){
      return true;
    } else {
      return false;
    }
  }

  void checkIdSwitch(String* idIn) {
    if(checkId(idIn)){
      switchState();
      Serial.print("GROUPSw  ");
      Serial.println(id);
    }

  }

  void checkIdSet(String* idIn, bool stateIn) {
    if(checkId(idIn)){
      setState(stateIn);
      Serial.print("GROUPSe  ");
      Serial.println(id);
    }
  }

  // Returns true if any Component of Group is active
  bool activeComp() {
    uint8_t i = 0;
    for(int w = 0; w<len; w++) {
      i = i + coms[w]->getState();
    }
    if (i > 0) {
      return 1;
    } else {
      return 0;
    }
  }

  
};

#endif