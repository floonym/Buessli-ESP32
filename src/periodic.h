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
    Component* c3a; //60v-12v
    Sensor* sAuto;
    Sensor* s12V;
    Sensor* s60V;
    uint32_t tRefresh1;
    uint32_t tSleep;
    bool refreshEnabled = 1;
    bool c1self = 0; //if 1 c1 was turned on by periodic, else by user


public:
    void setup(Component* c1In, Component* c2aIn, Component* c3aIn, Sensor* sAutoIn, Sensor* s12VIn, Sensor* s60VIn) {
        c1 = c1In;   
        c2a = c2aIn;
        c3a = c3aIn;
        sAuto = sAutoIn;
        s12V = s12VIn;
        s60V = s60VIn;
    }

    // Gets new Voltage Data from Sensors
    void refreshData(bool force) {
        if (refreshEnabled || force) {
            if (millis() - tRefresh1 > 60000 || force) {
                s12V->getValue();
                sAuto->getValue();
                
                s60V->getValue();
                tRefresh1 = millis();
            }
        }
    }

    // Enables Periodic Voltage Refresh
    void enableRefresh(bool enable) {
        if (enable && !refreshEnabled) {
            refreshEnabled = enable;
            refreshData(1);
        }
        refreshEnabled = enable;
        
    }

    // Turns on and off c1 when driving automatically
    bool automated() {
        refreshData(0);
        if (millis() - sAuto->getLastUpdate() < 5000) {
            if (sAuto->getData() > 0) {
                if (c1->getState()) {
                    if (sAuto->getData() < 130 && c1self) {
                        c1->setState(0);
                        c1self = 0;
                    }
                } else {
                    if (sAuto->getData() > 130) {
                        c1->setState(1);
                        c1self = 1;
                    } else {
                        c1self = 0;
                    }
                }
                return 1;
            }
        }
        return 0;
    }
    


};


#endif