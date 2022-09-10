#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "driver/touch_pad.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Adafruit_MCP23X17.h>
#include <Wire.h>

#include <component.h>
#include <group.h>
#include <sensor.h>
#include <page.h>
#include <touch.h>
#include <led.h>
#include <powersaver.h>
#include <periodic.h>
#include <expander.h>



#define displayPowerPin 4
bool displayPower = 1;

uint32_t tSleep;
String x;

uint32_t tD;

byte page;
Periodic Per;

Expander Ex;

//********** Sensors **********
Sensor s0;       //CarBat Voltage
Sensor s1;       //12V Voltage
Sensor s2;       //60V Voltage

Sensor* sAll[3];    //All Sensors
Sensor* sP1[3];     //Sensors Page1
Sensor* sP2[0];     //Sensors Page2

//********** Components **********
Component c1;    //12-12
Component c2a;   //12-60
Component c3a;   //60-12
Component c3b;   //60-12 After
Component c4a;   //Charger
Component c4b;   //Bridge
Component c4c;   //Inverter
Component c5;    //Pump
Component c6;    //Fridge
Component c7;    //Heizung
Component c8a;   //LED
Component c8b;   //LED After
Component c9a;   //Devices
Component c9b;   //Devices After
Component c9c;   //Beamer
Component c10;   //Audio Switch

Component* cAllVis[13]; //All Components with Buttons
Component* cAll[16];    //All Components
Component* cP1[13];     //Components Page1
Component* cP2[1];      //Components Page2

//********** Groups **********
Group g0;           //All Shutoff
Group gall;         //All Components

Group* gAll[1];     //All Groups
Group* gP1[1];      //Groups Page1  
Group* gP2[1];      //Groups Page2

//********** Pages **********
Page p1;
Page p2;

Page* pAll[2];      //All Pages

//********** Touches **********
Touch t1;
#define TOUCH_THRESHOLD 60


void callback() {

}

uint8_t wakeup_reason(){

  switch(esp_sleep_get_wakeup_cause()){
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); return 1; break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); return 2; break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); return 3; break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); return 4; break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); return 5; break;
    default : Serial.printf("Wakeup was not caused by deep sleep"); return 6; break;
  }
}

void sleeping(bool deepsleep) {
    delay(100);

    if (!gall.activeComp()) { //Deactivate Arduinos
      digitalWrite(LED_BUILTIN,0);
    }

    digitalWrite(displayPowerPin,0); //Power own Display
    displayPower = 0;

    touchAttachInterrupt(T9, callback, TOUCH_THRESHOLD); // Setup Sleep
    esp_sleep_enable_touchpad_wakeup();
    esp_sleep_enable_timer_wakeup(10000000);


    delay(100);
    esp_light_sleep_start();


    Serial.println("Waking Up");
    touch_pad_intr_disable();

    while (Serial2.available()) { Serial2.read(); }

    tSleep = millis();

    if (wakeup_reason() == 4) { //Touch Startup
      digitalWrite(displayPowerPin,1);
      displayPower = 1;
      delay(100);
      p1.setVisibility(1);
      
    } else { // Periodic Startup
      Per.refreshData(1);
    }
    

}
 
//***************************************** SETUP *****************************************
void setup() {
  //********** Serial Setup **********
  Serial.begin(115200);                       //PC
  Serial2.begin(9600, SERIAL_8N1, 16, 17);    //DISPLAY RX TX
  Serial2.setTimeout(100);

  //********** MCP Setup **********
  Ex.setup();
  
  //********** LED_Builtin-Enable **********
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,1);

  //********** Display-Enable **********
  pinMode(displayPowerPin,OUTPUT);
  digitalWrite(displayPowerPin,1);

  delay(100);

  //********** Frequency Adjust **********
  //setCpuFrequencyMhz(20);
  Serial.println(getXtalFrequencyMhz());
  Serial.println(getCpuFrequencyMhz());

  //********** Powersaver **********
  setModemSleep(); //Deactivate Modem

  //********** Periodic **********
  Per.setup(&c1,&c2a,&c3a,&s0,&s1,&s2);

  //********** Sensor Arrays **********
  sAll[0] = &s0;
  sAll[1] = &s1;
  sAll[2] = &s2;

  sP1[0] = &s0;
  sP1[1] = &s1;
  sP1[2] = &s2;

  //sP2[0] = &s0;
  
  //********** Component Arrays **********
  cAllVis[0] = &c1;
  cAllVis[1] = &c2a;
  cAllVis[2] = &c3a;
  cAllVis[3] = &c4a;
  cAllVis[4] = &c4b;
  cAllVis[5] = &c4c;
  cAllVis[6] = &c5;
  cAllVis[7] = &c6;
  cAllVis[8] = &c7;
  cAllVis[9] = &c8a;
  cAllVis[10] = &c9a;
  cAllVis[11] = &c9c;
  cAllVis[12] = &c10;

  cAll[0] = &c1;
  cAll[1] = &c2a;
  cAll[2] = &c3a;
  cAll[3] = &c4a;
  cAll[4] = &c4b;
  cAll[5] = &c4c;
  cAll[6] = &c5;
  cAll[7] = &c6;
  cAll[8] = &c7;
  cAll[9] = &c8a;
  cAll[10] = &c8b;
  cAll[11] = &c9a;
  cAll[12] = &c9b;
  cAll[13] = &c9c;
  cAll[14] = &c10;
  cAll[15] = &c3b;

  cP1[0] = &c1;
  cP1[1] = &c2a;
  cP1[2] = &c3a;
  cP1[3] = &c4a;
  cP1[4] = &c4b;
  cP1[5] = &c4c;
  cP1[6] = &c5;
  cP1[7] = &c6;
  cP1[8] = &c7;
  cP1[9] = &c8a;
  cP1[10] = &c9a;
  cP1[11] = &c9c;
  cP1[12] = &c10;

  cP2[0] = &c1;

  //********** Group Arrays **********
  gAll[0] = &g0;

  gP1[0] = &g0;

  gP2[0] = &g0;


  //********** Page Arrays **********
  pAll[0] = &p1;
  pAll[1] = &p2;


  //********** Setups **********
  p1.setup(1, gP1, 1, cP1, 13, sP1, 3);
  p2.setup(2, gP2, 1, cP2, 1, sP2, 0);

  gall.setup("b999", 0, cAll, 16);
  g0.setup("b116", 0, cP1, 13);

  //p1.setupArduinoEnable(&tArduinoEnable, &arduinoEnablePin);
  //p2.setupArduinoEnable(&tArduinoEnable, &arduinoEnablePin);

  p1.setVisibility(1);

  c1.setup("b100", 0, 0, &Ex);
  c2a.setup("b101", 8, 0, &Ex);
  c3a.setup("b102", 14, 0, &Ex);
  c3b.setup("b500", 11, 0, &Ex);
  c4a.setup("b104", 1, 0, &Ex);
  c4b.setup("b105", 2, 0, &Ex);
  c4c.setup("b106", 3, 0, &Ex);
  c5.setup("b108", 6, 0, &Ex);
  c6.setup("b109", 5, 0, &Ex);
  c7.setup("b109", 4, 0, &Ex);
  c8a.setup("b110", 12, 0, &Ex);
  c8b.setup("b500", 9, 0, &Ex);
  c9a.setup("b114", 12, 0, &Ex);
  c9b.setup("b500", 10, 0, &Ex);
  c9c.setup("b113", 100, 0, &Ex);
  c10.setup("b112", 100, 0, &Ex);

  // Delayed
  c3a.setupSecondCDelay(&c3b);
  c8a.setupSecondCDelay(&c8b);
  c9a.setupSecondCDelay(&c9b);


  // Exclusive
  c2a.setupSecondCExclusive(&c3a);
  c3a.setupSecondCExclusive(&c2a);
  c4b.setupSecondCExclusive(&c4c);
  c4c.setupSecondCExclusive(&c4b);



  s0.setup("x0", 36, 1, 0);
  s1.setup("x1", 39, 1, 0);
  s2.setup("x2", 34, 1, 0);

  t1.setup(T9, 60);

  delay(200);


}

//***************************************** LOOP *****************************************
void loop() {
  while(Serial2.available()){
    x = Serial2.readStringUntil('x');
    Serial.println(x);

    if (x.startsWith("b")) {
      //Check for ComponentButton press
      p1.checkIdSwitch(&x); 
      p2.checkIdSwitch(&x);
      
      //Check for GroupButton press
      g0.checkIdSet(&x, 0); //All off

      //Refresh GroupButtons if needed
      p1.groupButtonRefresh(0);
      p2.groupButtonRefresh(0);
    
    } else if (x.startsWith("p")) {
      byte pageIn = x.substring(1,2).toInt();
      if (pageIn >= 0 && pageIn <= 9) {
        pAll[page]->setVisibility(0);
        pAll[pageIn]->setVisibility(1);
        page = pageIn;
      }

    }
    tSleep = millis();
  }
  
  /*
  switch (t1.pressed())
  {
  case 1:
    Serial.println("short press");
    sleeping(0);
    break;
  case 2:
    Serial.println("long press");
    break;
  default:
    break;
  }
  */

  // Back to Sleep if periodic is complete
  if (Per.automated() && !displayPower) {
    Serial.println("Per auto");
    sleeping(0);
  }
  /*
  if (millis() - tSleep > 20000) {
    Serial.println("tSleep");
    sleeping(0);
  }
  */
  delay(100);

  /*
  if(millis()-tD > 5000) {
    s0.displayRefresh();
    s0.getValue();
    tD = millis();
  }*/
  
} 



