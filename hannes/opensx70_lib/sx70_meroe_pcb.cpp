#include "Arduino.h"
//#include <EEPROM.h>
//#include "Arduino.h"
#include "sx70_meroe_pcb.h"
#include "open_SX70.h"
#include "meter.h"
void io_init() {
  // Output definition
  pinMode(PIN_SOL1, OUTPUT);
  pinMode(PIN_SOL2, OUTPUT);
  pinMode(PIN_FFA, OUTPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  // output default state set.
  digitalWrite(PIN_SOL1, LOW);
  digitalWrite(PIN_SOL2, LOW);
  digitalWrite(PIN_FFA, LOW);
  digitalWrite(PIN_MOTOR, LOW);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  // input definition
  pinMode(PIN_S1, INPUT_PULLUP);
  pinMode(PIN_S2, INPUT_PULLUP);
  pinMode(PIN_S3, INPUT_PULLUP);
  pinMode(PIN_S5, INPUT_PULLUP);
  pinMode(PIN_S8, INPUT);
  pinMode(PIN_S9, INPUT);
}
