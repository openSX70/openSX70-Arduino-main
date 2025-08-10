#include "Arduino.h"
#include "camera_functions.h"
#include "meter.h"
#include "open_sx70.h"
#include "sx70_pcb.h"
#include "settings.h"


extern bool multipleExposureMode;

HardwareTimer *SolenoidPWM = new HardwareTimer(TIM1);

/*
Camera::Camera(uDongle *dongle){
  _dongle = dongle;
  //   io_init();
  //  init_EEPROM();
}
*/
Camera::Camera(void){

}

//setup for timers used for PWM
void Camera::solenoid_init(){
  SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 0); // 62khz, 0% dutycycle
  SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 0); // 62khz, 0% dutycycle
}

void Camera::shutterCLOSE(){
  Camera::solenoid_init();
  #if BASICDEBUG
    DEBUG_OUTPUT.println("shutterCLOSE");
  #endif
  SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 100);
  delay (PowerDownDelay);
  SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 30);
}

void Camera::shutterOPEN(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("shutterOPEN");
  #endif
  SolenoidPWM->setPWM(1, PIN_SOL1, 62000, 0);
}

void Camera::sol2Engage(){
  SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 100);
}

void Camera::sol2LowPower(){
  SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 130);
}

void Camera::sol2Disengage(){
  SolenoidPWM->setPWM(2, PIN_SOL2, 62000, 0);
}

void Camera::mirrorDOWN(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("mirrorDOWN");
  #endif
  digitalWrite(PIN_MOTOR, HIGH);
  while (Camera::DebouncedRead(PIN_S5) != LOW){
    #if BASICDEBUG
      DEBUG_OUTPUT.println("Wait for PIN_S5 to go LOW");
    #endif
  }
  digitalWrite(PIN_MOTOR, LOW);
  
}

void Camera::mirrorUP(){
  #if BASICDEBUG
    DEBUG_OUTPUT.println("mirrorUP");
  #endif
  if(digitalRead(PIN_S3) != HIGH){
    digitalWrite(PIN_MOTOR, HIGH);
  }

  while (DebouncedRead(PIN_S5) != HIGH){
    #if BASICDEBUG
      DEBUG_OUTPUT.println("Wait for S5 to go high");
    #endif
  }

  digitalWrite(PIN_MOTOR, LOW);
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


void Camera::ManualExposure(uint8_t selector){
  pinMode(PIN_S3, INPUT_PULLUP); // GND
  while (digitalRead(PIN_S3) != HIGH){            //waiting for S3 to OPEN
     #if BASICDEBUG
     DEBUG_OUTPUT.println("waiting for S3 to OPEN");
     #endif
  }
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
