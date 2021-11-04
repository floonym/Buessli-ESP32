#ifndef touch_h
#define touch_h

//Setup (pin,limit), Capacitive Touch Button Class
class Touch
{
private:
  uint8_t vallimit, valcurrent;
  uint8_t pin, state, output;
  uint16_t tPress, click_short = 500;

public:
  //Setup (pin,limit)
  void setup(uint8_t pinin, uint8_t vallimitin) {
    pin = pinin;
    vallimit = vallimitin;
  }

  //Returns 0 if not Pressed, 1 if Pressed, 2 if Longpressed
  int pressed()
  { 
    output = 0;
    valcurrent = touchRead(pin);
    switch(state) {
      case 0: //When Idling/Button gets pressed
        if (valcurrent < vallimit) {
          tPress = millis();
          state = 1;
        }
      break;
      case 1: //Checking if Button gets released in short time
        if (valcurrent > vallimit) {
          if (millis() - click_short < tPress) {
            output = 1;
          } else {
            output = 2;
          }
          state = 2;
          tPress = millis();
        }
      break;
      case 2: // Debounce
        if (millis() - 200 > tPress) {
          state = 0;
        }
      break;
    }
    return output;
  }

  //Set Limit
  void setlimit(uint8_t vallimitin) {
    vallimit = vallimitin;
  }

  //Set Delay for Longpress
  void setDelay(uint16_t click_short_in) {
    click_short = click_short_in;
  }
};

#endif