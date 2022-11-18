#include "Highdriver4.h"

#define DRIVER_NONE         0
#define DRIVER_HIGHDRIVER4  1

boolean bPumpState[4]={false,false,false,false};
uint8_t nPumpVoltageByte[4] = {0x00,0x00,0x00,0x00};

uint8_t nFrequencyByte=0x40;

uint8_t current_driver = 0;

void Driver_switchOff(uint8_t _pump=0);

bool Driver_select(uint8_t _driver) {
  if (_driver==DRIVER_NONE) {
    Driver_switchOff();
    current_driver=DRIVER_NONE;
    return true;
  } else if (_driver==DRIVER_HIGHDRIVER4) {
    Driver_switchOff();
    if (Highdriver4_check()) {
      current_driver=DRIVER_HIGHDRIVER4;
      Highdriver4_init();
      return true;
    } else {
      current_driver=DRIVER_NONE;
      return false;
    }
  }
}

void Driver_setfrequency(uint16_t _frequency) {
  switch (current_driver) {
    case DRIVER_NONE:
      break;
    case DRIVER_HIGHDRIVER4:
      Highdriver4_setfrequency(_frequency);
      break;
  }
}

void Driver_setvoltage(uint8_t _pump, uint8_t _voltage) {
  switch (current_driver) {
    case DRIVER_NONE:
      break;
    case DRIVER_HIGHDRIVER4:
      bPumpState[_pump-1]=true;
      Highdriver4_setvoltage(_pump,_voltage);
      break;
  }  
}

void Driver_setvoltage(uint8_t _v1, uint8_t _v2, uint8_t _v3, uint8_t _v4) {
  switch (current_driver) {
    case DRIVER_NONE:
      break;
    case DRIVER_HIGHDRIVER4:
        bPumpState[0] = (_v1 > 0);
        bPumpState[1] = (_v2 > 0);
        bPumpState[2] = (_v3 > 0);
        bPumpState[3] = (_v4 > 0);
      Highdriver4_setvoltage(_v1, _v2, _v3, _v4);
      break;
  }  
}

void Driver_switchOff(uint8_t _pump=0) {
  switch (current_driver) {
    case DRIVER_NONE:
      break;
    case DRIVER_HIGHDRIVER4:
      if (_pump==0) {
        bPumpState[0] = false;
        bPumpState[1] = false;
        bPumpState[2] = false;
        bPumpState[3] = false;
      } else 
        bPumpState[_pump-1] = false;
      Highdriver4_setvoltage();
      break;
  }  
}

void Driver_switchOn(uint8_t _pump=0) {
  switch (current_driver) {
    case DRIVER_NONE:
      break;
    case DRIVER_HIGHDRIVER4:
      if (_pump==0) {
        bPumpState[0] = true;
        bPumpState[1] = true;
        bPumpState[2] = true;
        bPumpState[3] = true;
      } else 
        bPumpState[_pump-1] = true;
      Highdriver4_setvoltage();
      break;
  }  
}

void Board_init() {
  pinMode(A3,INPUT_PULLUP);   //Boardversion detection A3=GND -> rev3
  bBoardversion = ((digitalRead(A3)==HIGH) ? 2 : 3); 
}
