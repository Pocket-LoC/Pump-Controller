
#define SOFTWARE_VERSION  "Pocket LoC Pump Controller Firmware v2"
uint8_t bBoardversion = 0;

#include <Wire.h>

uint8_t sensors = 0;

#include "Interface.h"
#include "Serial_interface.h"

#define CYCLE_TIME 10

uint32_t timestamp = 0;

void setup() {
  Serial_init();
  Wire.begin();
  Board_init();
  Serial_println("Multiboard Ready");
}

void loop() {
  Serial_input();
  
  delay(CYCLE_TIME);
  timestamp++;
}
