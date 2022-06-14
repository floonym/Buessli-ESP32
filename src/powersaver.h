#ifndef powersaver_h
#define powersaver_h

#include <Arduino.h>
#include <WiFi.h>
#include <BluetoothSerial.h>
#include "driver/adc.h"
#include <esp_bt.h>

void disableWiFi(){
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
}
void disableBluetooth(){
    // Quite unusefully, no relevable power consumption
    btStop();
}
 
void setModemSleep() {
    disableWiFi();
    disableBluetooth();
}

#endif