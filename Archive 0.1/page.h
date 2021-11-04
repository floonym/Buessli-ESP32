#ifndef page_h
#define page_h

#include <component.h>
#include <group.h>
#include <sensor.h>

class  Page
{
private:
  Group* group[20];
  byte glen;
  Component* comp[50];
  byte clen;
  Sensor* sens[10];
  byte slen;

public:
  void setup(Group* groupIn[20], byte glenIn, Component* compIn[50], byte clenIn, Sensor* sensIn[10], byte slenIn) {
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
  
};

#endif