#ifndef page_h
#define page_h

#include <component.h>
#include <group.h>
#include <sensor.h>

class  Page
{
private:
  uint8_t pageNr;
  Group* group[20];
  byte glen;
  Component* comp[50];
  byte clen;
  Sensor* sens[10];
  byte slen;

public:
  //(pageNr, group, grouplen, component, componentlen, sensor, sensorlen)
  void setup(uint8_t pageNrIn, Group* groupIn[20], byte glenIn, Component* compIn[50], byte clenIn, Sensor* sensIn[10], byte slenIn) {
    pageNr = pageNrIn;
    glen = glenIn;
    clen = clenIn;
    slen = slenIn;
    for(int w=0; w<glen; w++) {
      group[w] = groupIn[w];
    }
    for(int w=0; w<clen; w++) {
      comp[w] = compIn[w];    
    }
    for(int w=0; w<slen; w++) {
      sens[w] = sensIn[w];
    }
  }

  void setVisibility(bool visIn) {
    for (int w=0; w<glen; w++) {
      group[w]->setVisibility(visIn);
    }
    for (int w=0; w<clen; w++) {
      comp[w]->setVisibility(visIn);
    }
    for (int w=0; w<slen; w++) {
      sens[w]->setVisibility(visIn);
    }
  }

  void checkIdSwitch(String* x) {
    if((*x).substring(1,2).toInt() == pageNr){
      for(int w = 0; w<clen; w++){
        if(comp[w]->checkIdSwitch(x)) {
          w = 100;
        }
      }
    }
  }
  //(Force)
  void groupButtonRefresh(bool force) {
    for(int w = 0; w<glen; w++) {
      group[w]->buttonRefresh(force);
    }
  }


  void setupArduinoEnable(uint32_t* tArduinoEnableInIn, byte* arduinoEnablePinInIn) {
    for(int w=0; w<clen; w++) {
      comp[w]->setupArduino(tArduinoEnableInIn,arduinoEnablePinInIn);
    }
  }
  
};

#endif