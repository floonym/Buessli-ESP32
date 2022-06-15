#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <component.h>
#include <group.h>
#include <sensor.h>
#include <page.h>
#include <touch.h>
#include <led.h>
#include <powersaver.h>


//int LED_BUILTIN = 2;
#define displayPowerPin 13 //13
bool displayPower = 1;

byte arduinoEnablePin = 5;
uint32_t tArduinoEnable;

// #define PIN 22 LED Pin


String x;
String id;
char z[10];

bool b;

byte y;
uint32_t tD;

byte page;

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
Component c2b;   //12-60 After
Component c3a;   //60-12
Component c3b;   //60-12 After
Component c4a;   //Charger
Component c4b;   //Inverter
Component c4c;   //Bridge
Component c5;    //Pump
Component c6;    //Fridge
Component c9a;   //USB&Beamer
Component c9b;   //USB&Beamer After
Component c8a;   //LED
Component c8b;   //LED After
Component c8c;   //LED Switch
Component c10;   //Audio Switch

Component* cAllVis[12]; //All Components with Buttons
Component* cAll[16];    //All Components
Component* cP1[12];     //Components Page1
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
#define TOUCH_THRESHOLD 30


void callback() {

}


void sleep(bool deepsleep) {
    delay(100);
    digitalWrite(displayPowerPin,0);
    touchAttachInterrupt(T4, callback, TOUCH_THRESHOLD);
    esp_sleep_enable_touchpad_wakeup();
    //esp_sleep_enable_timer_wakeup(time_in_us)
    esp_light_sleep_start();

    Serial.println("Waking Up");
    touch_pad_intr_disable();
    digitalWrite(displayPowerPin,1);
    delay(100);
    p1.setVisibility(1);
}
 
//***************************************** SETUP *****************************************
void setup() {
  //********** Serial Setup **********
  Serial.begin(115200);                       //PC
  Serial1.begin(9600, SERIAL_8N1, 18, 19);    //ARDUINO RX TX 
  Serial2.begin(9600, SERIAL_8N1, 16, 17);    //DISPLAY RX TX
  
  //********** Arduino-Enable **********
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(arduinoEnablePin,OUTPUT);
  digitalWrite(arduinoEnablePin,1);
  digitalWrite(LED_BUILTIN,1);
  tArduinoEnable = millis();

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
  cAllVis[8] = &c8a;
  cAllVis[9] = &c8c;
  cAllVis[10] = &c9a;
  cAllVis[11] = &c10;

  cAll[0] = &c1;
  cAll[1] = &c2a;
  cAll[2] = &c3a;
  cAll[3] = &c4a;
  cAll[4] = &c4b;
  cAll[5] = &c4c;
  cAll[6] = &c5;
  cAll[7] = &c6;
  cAll[8] = &c8a;
  cAll[9] = &c8c;
  cAll[10] = &c9a;
  cAll[11] = &c10;
  cAll[12] = &c2b;
  cAll[13] = &c3b;
  cAll[14] = &c9b;
  cAll[15] = &c8b;

  cP1[0] = &c1;
  cP1[1] = &c2a;
  cP1[2] = &c3a;
  cP1[3] = &c4a;
  cP1[4] = &c4b;
  cP1[5] = &c4c;
  cP1[6] = &c5;
  cP1[7] = &c6;
  cP1[8] = &c8a;
  cP1[9] = &c8c;
  cP1[10] = &c9a;
  cP1[11] = &c10;

  cP2[0] = &c1;

  //********** Group Arrays **********
  gAll[0] = &g0;

  gP1[0] = &g0;

  gP2[0] = &g0;


  //********** Page Arrays **********
  pAll[0] = &p1;
  pAll[1] = &p2;


  //********** Setups **********
  p1.setup(1, gP1, 1, cP1, 12, sP1, 3);
  p2.setup(2, gP2, 1, cP2, 1, sP2, 0);

  gall.setup("b999", 0, cAll, 16);
  g0.setup("b116", 0, cAllVis, 12);

  //p1.setupArduinoEnable(&tArduinoEnable, &arduinoEnablePin);
  //p2.setupArduinoEnable(&tArduinoEnable, &arduinoEnablePin);

  gall.setupArduinoEnable(&tArduinoEnable, &arduinoEnablePin);

  p1.setVisibility(1);

  c1.setup("b100", "010", 0);
  c2a.setup("b101", "013", 0);
  c2b.setup("b500", "010", 0);
  c3a.setup("b102", "010", 0);
  c3b.setup("b500", "010", 0);
  c4a.setup("b104", "010", 0);
  c4b.setup("b105", "010", 0);
  c4c.setup("b106", "010", 0);
  c5.setup("b108", "010", 0);
  c6.setup("b109", "010", 0);
  c9a.setup("b110", "010", 0);
  c9b.setup("b500", "010", 0);
  c8a.setup("b114", "010", 0);
  c8b.setup("b500", "010", 0);
  c8c.setup("b113", "010", 0);
  c10.setup("b112", "010", 0);

  // Delayed
  c2a.setupSecondCDelay(&c2b);
  c3a.setupSecondCDelay(&c3b);
  c8a.setupSecondCDelay(&c8b);
  c9a.setupSecondCDelay(&c9b);


  // Exclusive
  c2a.setupSecondCExclusive(&c3a);
  c3a.setupSecondCExclusive(&c2a);
  c4b.setupSecondCExclusive(&c4c);
  c4c.setupSecondCExclusive(&c4b);



  s0.setup("x0", "14", 1, 0, &tArduinoEnable, &arduinoEnablePin);
  s1.setup("x1", "15", 1, 0, &tArduinoEnable, &arduinoEnablePin);
  s2.setup("x2", "16", 1, 0, &tArduinoEnable, &arduinoEnablePin);

  t1.setup(T9, 30);
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
  }
  
  while(Serial1.available()) {
    x = Serial1.readStringUntil('\n');
    Serial.println(x);

    if(x.startsWith("v")) {
      for(int u = 0; u<3; u++) {
        sAll[u]->checkPinDataLoad(x);
      }
    }
  }
  /*
  switch (t1.pressed())
  {
  case 1:
    Serial.println("short press, going to Sleep");
    break;
  case 2:
    Serial.println("long press, going to Deepsleep");
    break;
  default:
    break;
  }
  */

  if (millis()-tArduinoEnable > 5000) {
    digitalWrite(arduinoEnablePin,0);
    digitalWrite(LED_BUILTIN,0);
    //Serial.println("Shutdown Arduino");
  }
  delay(100);
  /*
  if(millis()-tD > 5000) {
    s0.displayRefresh();
    s0.getValue();
    tD = millis();
  }*/
  
} 



