#include <Arduino.h>
//#include <NeoPixelBus.h>

#include <component.h>
#include <group.h>
#include <sensor.h>
#include <page.h>
#include <touch.h>
#include <led.h>


//int LED_BUILTIN = 2;
#define displayPowerPin 12
bool displayPower = 1;

byte arduinoEnablePin = 14;
uint32_t tArduinoEnable;

String x;
String id;
char z[10];

bool b;

byte y;
uint32_t tD;

byte page;

Sensor s0;
Sensor s1;
Sensor s2;

Sensor* sall[3];
Sensor* sP1[3];
Sensor* sP2[0];

Component c0;
Component c1;
Component c2;
Component c3;
Component c4;

Component* all[5];
Component* on[2];
Component* off[3];
Component* cP1[5];
Component* cP2[2];


Group g0;
Group g1;
Group g2;

Group* gP1[3];
Group* gP2[2];
Group* gall[3];

Page p1;
Page p2;

Page* pAll[2];

Touch t1;
#define TOUCH_THRESHOLD 30

void callback() {

}


void sleep() {
    Serial.println("long press, going to Sleep");
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
 

void setup() {
  Serial.begin(115200);                       //PC
  Serial1.begin(9600, SERIAL_8N1, 18, 19);    //ARDUINO
  Serial2.begin(9600, SERIAL_8N1, 16, 17);    //DISPLAY
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(displayPowerPin,OUTPUT);
  pinMode(arduinoEnablePin,OUTPUT);
  digitalWrite(displayPowerPin,1);
  digitalWrite(arduinoEnablePin,1);

  tArduinoEnable = millis();

  delay(100);

  s0.setup("x0", "14", 0.1, 0, &tArduinoEnable, &arduinoEnablePin);
  s1.setup("x1", "15", 1, 0, &tArduinoEnable, &arduinoEnablePin);
  s2.setup("x2", "16", 1, 0, &tArduinoEnable, &arduinoEnablePin);

  sall[0] = &s0;
  sall[1] = &s1;
  sall[2] = &s2;

  sP1[0] = &s0;
  sP1[1] = &s1;
  sP1[2] = &s2;
  

  c0.setup("b100", "010", 0, &tArduinoEnable, &arduinoEnablePin);
  c1.setup("b101", "011", 0, &tArduinoEnable, &arduinoEnablePin);
  c2.setup("b102", "013", 0, &tArduinoEnable, &arduinoEnablePin);
  c3.setup("b103", "113", 0, &tArduinoEnable, &arduinoEnablePin);
  c4.setup("b104", "009", 0, &tArduinoEnable, &arduinoEnablePin);

  all[0] = &c0;
  all[1] = &c1;
  all[2] = &c2;
  all[3] = &c3;
  all[4] = &c4;
  
  on[0] = &c0;
  on[1] = &c1;

  off[0] = &c2;
  off[1] = &c3;
  off[2] = &c4;

  cP1[0] = &c0;
  cP1[1] = &c1;
  cP1[2] = &c2;
  cP1[3] = &c3;
  cP1[4] = &c4;

  cP2[0] = &c0;
  cP2[1] = &c4;
  

  g0.setup("b116", 0, all, 5);
  g1.setup("b117", 0, on, 2);
  g2.setup("b118", 0, off, 2);

  gall[0] = &g0;
  gall[1] = &g1;
  gall[2] = &g2;

  gP1[0] = &g0;
  gP1[1] = &g1;
  gP1[2] = &g2;

  gP2[0] = &g0;
  gP2[1] = &g2;


  p1.setup(gP1, 3, cP1, 5, sP1, 3);
  p2.setup(gP2, 2, cP2, 2, sP2, 0);

  p1.setVisibility(1);

  pAll[0] = &p1;
  pAll[1] = &p2;

  y = millis();


  t1.setup(T4, 30);

  //setCpuFrequencyMhz(20);
  Serial.println(getXtalFrequencyMhz());
  Serial.println(getCpuFrequencyMhz());
}


void loop() {
  while(Serial2.available()){
    x = Serial2.readStringUntil('x');
    Serial.println(x);

    if (x.startsWith("b")) {
      for(int u = 0; u<5; u++){
        if(all[u]->checkIdSwitch(&x)) {
          u = 100;
        }
      }
      g0.checkIdSwitch(&x);
      g1.checkIdSet(&x, 1);
      g2.checkIdSet(&x, 0);

      for(int i = 0; i<3; i++) {
        gall[i]->buttonRefresh(1);
      }
    
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
        sall[u]->checkPinDataLoad(x);
      }
    }
  }
  switch (t1.pressed())
  {
  case 1:
    Serial.println("short press");
    break;
  
  case 2:
    sleep();
    break;
  default:
    break;
  }

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



