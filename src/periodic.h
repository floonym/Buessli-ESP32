#ifndef periodic_h
#define periodic_h

#include <Arduino.h>
#include <component.h>
#include <group.h>
#include <sensor.h>


class Periodic
{
private:
    Component* c1;  //12v-12v
    Component* c2a; //12v-60v
    Component* c2b; //60v-12v
    Sensor* sAuto;
    Sensor* s12V;
    Sensor* s60V;
    uint32_t tRefresh1;
    uint32_t tSleep;
    bool refreshEnabled;
    bool c1Forced = 1; //If 1, User set relay c1 to on. If so it will not auto turn off


public:
    void setup(Component* c1In, Component* c2aIn, Component* c2bIn, Sensor* sAutoIn, Sensor* s12VIn, Sensor* s60VIn) {
        c1 = c1In;   
        c2a = c2aIn;
        c2b = c2bIn;
        sAuto = sAutoIn;
        s12V = s12VIn;
        s60V = s60VIn;
    }

    // Gets new Voltage Data from Sensors
    void refreshData() {
        if (refreshEnabled) {
            if (millis() - tRefresh1 > 60000) {
                sAuto->getValue();
                s12V->getValue();
                s60V->getValue();
                tRefresh1 = millis();
            }
        }
    }

    // Enables Periodic Voltage Refresh
    void enableRefresh(bool enable) {
        if (enable && !refreshEnabled) {
            refreshEnabled = enable;
            refreshData();
        }
        refreshEnabled = enable;
        
    }
    // Turns on and off c1 when driving automatically
    void lightsleep() {
        refreshData();
        delay(2000);
        if (sAuto->getData() > 0 && millis() - sAuto->getLastUpdate() < 5000) {
            if (c1->getState()) {
                if (sAuto->getData() < 130 && !c1Forced) {
                    c1->setState(0);
                }
            } else {
                if (sAuto->getData() > 130) {
                    c1->setState(1);
                    c1Forced = 0;
                }
            }
        }
    }
};


#endif