#include "Arduino.h"
//#include <EEPROM.h>
//#include "Arduino.h"
#include "sx70_meroe_pcb.h"
#include "open_SX70.h"
#include "meter.h"

//For SONAR PCB
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
  #if SONAR == 0
  pinMode(PIN_S1, INPUT_PULLUP); //S1T on Sonar
  #endif
  pinMode(PIN_S2, INPUT_PULLUP); //changed from INPUT_PULLUP 27_04
  pinMode(PIN_S3, INPUT_PULLUP);
  pinMode(PIN_S5, INPUT_PULLUP);
  pinMode(PIN_S8, INPUT_PULLUP);
  pinMode(PIN_S9, INPUT_PULLUP);

 
  #if SONAR
  pinMode(PIN_S1, INPUT); //S1T on Sonar
  //pinMode(PIN_FT, INPUT_PULLUP); //doesnt work in SonarV1!!
  pinMode(PIN_S1F, INPUT);
  pinMode(PIN_GTD, INPUT_PULLUP);
  #endif
  
}
