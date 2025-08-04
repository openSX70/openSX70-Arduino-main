#include "Arduino.h"
#include "camera_functions.h"
#include "meter.h"
#include "open_sx70.h"
#include "sx70_pcb.h"
#include "settings.h"
#include "udongle2.h"


extern bool multipleExposureMode;

#ifdef ARDUINO_GENERIC_G030K8TX
HardwareTimer *SolenoidPWM = new HardwareTimer(TIM1);
#endif

Camera::Camera(uDongle *dongle){
  _dongle = dongle;
  //   io_init();
  //  init_EEPROM();
}

void Camera::S1F_Focus(){
    #if FOCUSDEBUG
      DEBUG_OUTPUT.println("Focus on");
    #endif
    digitalWrite(PIN_S1F_FBW, HIGH);
    return;
}

void Camera::S1F_Unfocus(){
    #if FOCUSDEBUG
      DEBUG_OUTPUT.println("Focus off");
    #endif
    digitalWrite (PIN_S1F_FBW, LOW);
    return;
}

//setup for timers used for PWM
void Camera::solenoid_init(){
    #ifdef ARDUINO_AVR_PRO
        const byte n =224;      // for example, 71.111 kHz
        //PWM high speed
        //one N_Mosfet powerdown
        //taken from: https://www.gammon.com.au/forum/?id=11504
        /*
        Timer 0
        input     T0     pin  6  (D4)
        output    OC0A   pin 12  (D6)
        output    OC0B   pin 11  (D5)
    
        Timer 1
        input     T1     pin 11  (D5)
        output    OC1A   pin 15  (D9)
        output    OC1B   pin 16  (D10)
    
        Timer 2
        output    OC2A   pin 17  (D11)
        output    OC2B   pin  5  (D3)
        */
        TCCR2A = bit (WGM20) | bit (WGM21) | bit (COM2B1); // fast PWM, clear OC2A on compare
        TCCR2B = bit (WGM22) | bit (CS20);                 // fast PWM, no prescaler
        OCR2A =  n;                                        // Value to count to - from table
        OCR2B = ((n + 1) / 2) - 1;                         // 50% duty cycle
        //THIS AFFECTS OUTPUT 3 (Solenoid1) AND OUTPUT 11 (Solenoid2)    
    #elif defined ARDUINO_GENERIC_G030K8TX
        SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 0); // 62khz, 0% dutycycle
        SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 0); // 62khz, 0% dutycycle
    #endif
}

void Camera::shutterCLOSE(){
  Camera::solenoid_init();
  #if BASICDEBUG
    DEBUG_OUTPUT.println("shutterCLOSE");
  #endif
  #ifdef ARDUINO_AVR_PRO
    analogWrite(PIN_SOL1, 255);
    delay (PowerDownDelay);
    analogWrite (PIN_SOL1, 130);
  #endif
  #ifdef ARDUINO_GENERIC_G030K8TX
    SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 100);
    delay (PowerDownDelay);
    SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 30);
  #endif

  return;
}

void Camera::shutterOPEN(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("shutterOPEN");
  #endif
  #ifdef ARDUINO_AVR_PRO
    analogWrite (PIN_SOL1, 0);
  #endif
  #ifdef ARDUINO_GENERIC_G030K8TX
    SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 0);
  #endif

  return; //Added 26.10.
}

void Camera::sol2Engage(){
  #ifdef ARDUINO_AVR_PRO
    analogWrite(PIN_SOL2, 255);
  #endif
  #ifdef ARDUINO_GENERIC_G030K8TX
    SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 100);
    //SolenoidPWM->setCaptureCompare(2, 100, PERCENT_COMPARE_FORMAT);
  #endif
}

void Camera::sol2LowPower(){
  #ifdef ARDUINO_AVR_PRO
    analogWrite(PIN_SOL2, 77);
  #endif
  #ifdef ARDUINO_GENERIC_G030K8TX
    SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 130);
  #endif
}

void Camera::sol2Disengage(){
  #ifdef ARDUINO_AVR_PRO
    analogWrite(PIN_SOL2, 0);
  #endif
  #ifdef ARDUINO_GENERIC_G030K8TX
    SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 0);
    //SolenoidPWM->setCaptureCompare(2, 0, PERCENT_COMPARE_FORMAT);
  #endif
}

void Camera::motorON(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("motorON");
  #endif
  digitalWrite(PIN_MOTOR, HIGH);
}

void Camera::motorOFF(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("motorOFF");
  #endif
  digitalWrite(PIN_MOTOR, LOW);
}

void Camera::mirrorDOWN(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("mirrorDOWN");
  #endif
  Camera::motorON();
  while (Camera::DebouncedRead(PIN_S5) != LOW){
    #if BASICDEBUG
      DEBUG_OUTPUT.println("Wait for PIN_S5 to go LOW");
    #endif
  }
  motorOFF();
  
}

void Camera::mirrorUP(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("mirrorUP");
  #endif
  if(digitalRead(PIN_S3) != HIGH){
    motorON ();
  }

  while (DebouncedRead(PIN_S5) != HIGH){
    #if BASICDEBUG
      DEBUG_OUTPUT.println("Wait for S5 to go high");
    #endif
  }

  motorOFF ();
}

void Camera::darkslideEJECT(){
  #if SIMPLEDEBUG
    DEBUG_OUTPUT.println(F("darkslideEJECT"));
  #endif
  Camera::shutterCLOSE();
  Camera::mirrorUP();
  Camera::mirrorDOWN();
  Camera::shutterOPEN();
}

void Camera::DongleFlashNormal(){
  pinMode(PIN_S2, OUTPUT);
  digitalWrite(PIN_S2, LOW);      //So FFA recognizes the flash as such
  digitalWrite(PIN_FF, HIGH);       //FLASH TRIGGERING
  delay (1);                        //FLASH TRIGGERING
  digitalWrite(PIN_FF, LOW);        //FLASH TRIGGERING
  pinMode(PIN_S2, INPUT_PULLUP);  //S2 back to dongle mode
}

void Camera::Ydelay (){
  delay (120);
}

bool Camera::DebouncedRead(uint8_t pin){
  pinMode(pin, INPUT_PULLUP); // GND
  bool lastState = digitalRead(pin);
  uint8_t stablecount = 0;
  while (stablecount < DEBOUNCECOUNT){
    delay(1);
    bool thisState = digitalRead(pin);
    if (thisState == lastState)
      stablecount++;
    else
      stablecount = 0;
    lastState = thisState;
  }
  return lastState;
}

void Camera::BlinkTimerDelay(byte led1, byte led2, byte time) {
  // DONGLE-LED BLINKS ON COUNTDOWN (10secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.
  //uint32_t startTimer = millis();
  //*******************************************************
  uint32_t steps = (time * 1000) / 4;

  // DS2408 LED BLINK

  Camera::S1F_Unfocus(); 
  Camera::Blink (1000, steps, led1, 2);
  Camera::Blink (600, steps, led1, 2);
  Camera::Blink (200, steps, led1, 2);
  steps = steps / 2;
  Camera::S1F_Focus();
  #if TIMER_MIRROR_UP
    Camera::shutterCLOSE();
  #endif
  Camera::Blink (80, steps, led1, 2);
  #if TIMER_MIRROR_UP
    Camera::mirrorUP();
  #endif
  Camera::Blink (80, steps, led2, 2);
}


// blink (blink interval=blinking speed, timer=duration blinking, Pin=pin of LED
//type 1 = ONBOARD LED
//type 2 = DONGLE LED

void Camera::Blink(unsigned int interval, int timer, int Pin, byte type){
  int ledState = LOW;             // ledState used to set the LED
  pinMode(Pin, OUTPUT);
  uint32_t previousMillis = 0;        // will store last time LED was updated
  uint32_t currentMillisTimer = millis();
  while (millis() < (currentMillisTimer + timer)){
    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == 0){
        ledState = 1;
      } 
      else{
        ledState = 0;
      }
      // set the LED with the ledState of the variable:
      if (type == 1){
        digitalWrite (Pin, ledState);
      } 
      else if (type == 2){
        _dongle->Write_DS2408_PIO (Pin, ledState);
      }
    }
  }
}

void Camera::Blink (unsigned int interval, int timer, int PinDongle, int PinPCB, byte type){
  int ledState = LOW;             // ledState used to set the LED
  pinMode(PinDongle, OUTPUT);
  pinMode(PinPCB, OUTPUT);
  uint32_t previousMillis = 0;        // will store last time LED was updated
  uint32_t currentMillisTimer = millis();
  while (millis() < (currentMillisTimer + timer)){
    uint32_t currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == 0) {
        ledState = 1;
      } 
      else {
        ledState = 0;
      }
      // set the LED with the ledState of the variable:
      if (type == 1) {
        //DEBUG_OUTPUT.println("TYPE 1 - PCB Only");
        digitalWrite (PinPCB, ledState);
      }  
      else if (type == 2) {
        //DEBUG_OUTPUT.println("TYPE 2 - PCB and DONGLE");
        digitalWrite (PinPCB, ledState);
        _dongle->Write_DS2408_PIO (PinDongle, ledState);
      }
    }
  }
}

void Camera::ManualExposure(int _myISO, uint8_t selector){
  uint32_t initialMillis;

  pinMode(PIN_S3, INPUT_PULLUP); // GND
  while (digitalRead(PIN_S3) != HIGH){            //waiting for S3 to OPEN
     #if BASICDEBUG
     DEBUG_OUTPUT.println("waiting for S3 to OPEN");
     #endif
  }
  #if APERTURE_PRIORITY
    AperturePriority();
  #endif
  delay (YDelay);

  

  if (selector >= Dongle_Flash_Limit){
    int ShutterSpeedDelay = (ShutterSpeed[selector] - Flash_Capture_Delay);

    Camera::shutterOPEN();
    delay(ShutterSpeedDelay);
    Camera::FastFlash();
    delay(Flash_Capture_Delay);
  }
  else{
    int ShutterSpeedDelay = ShutterSpeed[selector];
    Camera::shutterOPEN();
    delay(ShutterSpeedDelay);
  }

  #if LMDEBUG
    uint32_t shutterCloseTime = millis(); //Shutter Debug
  #endif
  Camera::ExposureFinish();
  #if LMDEBUG
      uint32_t exposureTime = shutterCloseTime - initialMillis; //Shutter Debug
      DEBUG_OUTPUT.print("ExposureTime on Manualmode: ");
      DEBUG_OUTPUT.println(exposureTime);
  #endif
  return; //Added 26.10.
}

void Camera::AutoExposure(int _myISO){
  delay(YDelay);

  meter_set_iso(_myISO);
  meter_reset();

  Camera::shutterOPEN();

  uint32_t timeoutThreshold = millis() + AUTO_TIMEOUT;
  while (meter_update() == false){
    // timeout if current exposure hits AUTO_TIMEOUT
    if ((millis() >= timeoutThreshold) ){
      break;
    }
  }

  Camera::ExposureFinish();
  
  return; //Added 26.10.
}

// TODO Explore this one a bit. It may be possible to remove the hard coded timing
// and move purely to a meter-based approach. Would be faster and more consistent.
void Camera::AutoExposureFF(int _myISO){
  uint16_t FD_MN = 0;
  uint16_t FF_MN = 0;  //FlashDelay Magicnumber
  
  if(_myISO == ISO_SX70){
    FD_MN = FD100;
    FF_MN = FF100;  
  }
  else if(_myISO == ISO_600){
    FD_MN = FD600;
    FF_MN = FF600; 
  }

  Camera::sol2Engage();
  delay(YDelay);
  meter_set_iso(FD_MN);
  Camera::sol2LowPower();
  meter_reset();

  uint32_t integrationStartTime = millis();
  Camera::shutterOPEN(); //Power released from SOL1 - 25ms to get Shutter full open
  while (meter_update() == false){ //Start FlashDelay: Integrate with the 1/3 of the Magicnumber in Automode of selected ISO
    if((millis() - integrationStartTime) >= Flash_Max_Time){ //Flash can occure anytime of the Flash Delay 56+-7ms depending on scene brightness
      break;
    }  
  }

  meter_set_iso(FF_MN);
  //digitalWrite(PIN_FF, HIGH);  //FireFlash
  FastFlash();
  delay(Flash_Capture_Delay);   //Capture Flash 
  uint32_t FFStartTime = millis();
  while (meter_update() == false){
    if ((millis() - FFStartTime) >= Flash_Capture_Max_Time){
      break;
    }
  }

  Camera::sol2Disengage();
  Camera::ExposureFinish();

  return; //Added 26.10.
}

void Camera::ShutterB()
{

  delay (YDelay);

  Camera::shutterOPEN ();

  while (digitalRead(PIN_S1) == S1Logic){
    
  }
  Camera::FastFlash();
  delay(Flash_Capture_Delay);   //Capture Flash 

  ExposureFinish();
  return; //Added 26.10.
}

void Camera::ShutterT(){

  delay (40);
  
  while (digitalRead(PIN_S1F) == HIGH){
  }

  Camera::shutterOPEN ();

  // Catch button press, wait for depress
  while(DebouncedRead(PIN_S1) == S1Logic){
  }
  while (digitalRead(PIN_S1) == !S1Logic){
  }
  Camera::FastFlash();
  delay(Flash_Capture_Delay);   //Capture Flash 

  ExposureFinish();
  return; //Addes 26.10.
}

void Camera::ExposureFinish()
{
  Camera::shutterCLOSE();

  delay (30); //Was 20

  if(multipleExposureMode == true){
    while(digitalRead(PIN_S1) == S1Logic);
    #if MXDEBUG
      DEBUG_OUTPUT.println("mEXP");
    #endif
    return;
  }
  else{
    delay (100);
    #if EJECT_AFTER_DEPRESSING
      while(digitalRead(PIN_S1) == S1Logic); // wait for s1 to be depressed
      Camera::mirrorDOWN ();
      delay (30);
    #else
      Camera::mirrorDOWN ();
      delay (30);
      while(digitalRead(PIN_S1) == S1Logic); // wait for s1 to be depressed
    #endif
    Camera::shutterOPEN();
    delay (100);
  }
  return;
}

void Camera::multipleExposureLastClick(){
  Camera::mirrorDOWN(); 
  Camera::shutterOPEN();
  multipleExposureMode = false;
}

void Camera::FastFlash(){
  pinMode(PIN_S2, OUTPUT);
  digitalWrite (PIN_S2, LOW);     //So FFA recognizes the flash as such
  digitalWrite(PIN_FF, HIGH);    //FLASH TRIGGERING
  delay(1);
  digitalWrite(PIN_FF, LOW);     //FLASH TRIGGERING
  pinMode(PIN_S2, INPUT_PULLUP);  //S2 back to normal
}



bool Camera::setLIGHTMETER_HELPER(bool state){
  #if LMHELPERDEBUG
    DEBUG_OUTPUT.print("Set Lightmeterhelper status: ");
    DEBUG_OUTPUT.println(String(state));
  #endif
  lightmeterHelper = state;
  return state;
}

bool Camera::getLIGHTMETER_HELPER(){
  #if LMHELPERDEBUG
    //DEBUG_OUTPUT.println("Lightmeterhelper status: ");
    //DEBUG_OUTPUT.println(lightmeterHelper));
  #endif
  return lightmeterHelper;
}
