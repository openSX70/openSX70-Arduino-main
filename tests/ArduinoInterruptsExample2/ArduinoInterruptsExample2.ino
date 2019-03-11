#include "LowPower.h"

const uint8_t buttonPin = 3;
uint8_t ledState = LOW;

void setup() {
  pinMode(13,OUTPUT);

  pinMode(3,INPUT);

    attachInterrupt(digitalPinToInterrupt(buttonPin),toggleLed,RISING);
//  attachInterrupt(1,turnLEDOff,FALLING);
}

void loop() {

 LowPower.powerDown(SLEEP_FOREVER , ADC_OFF, BOD_OFF); 
 //turnLEDOff();
}

void toggleLed()
{
  ledState = !ledState;
  digitalWrite(13,ledState);
}
