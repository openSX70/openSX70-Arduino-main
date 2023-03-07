#include "Arduino.h"
#include "camera_functions.h"
#include "meter.h"
#include "open_sx70.h"
#include "sx70_pcb.h"
//#include "Clickbutton.h"
#include "settings.h"
#include "udongle2.h"

extern bool mEXPFirstRun;
extern bool multipleExposureMode;
//

int GTD = 0;

Camera::Camera(uDongle *dongle){
  _dongle = dongle;
  //   io_init();
  //  init_EEPROM();
}

#if SONAR
int Camera::getGTD(){
   GTD = digitalRead(PIN_GTD);
   return GTD;  
}

void Camera::S1F_Focus(){
    #if FOCUSDEBUG
      Serial.println("Focus on");
    #endif
    digitalWrite(PIN_S1F_FBW, HIGH);
    return;
}


void Camera::S1F_Unfocus(){
    #if FOCUSDEBUG
      Serial.println("Focus off");
    #endif
    digitalWrite (PIN_S1F_FBW, LOW);
    return;
}
#endif

#if APERTURE_PRIORITY
void Camera::AperturePriority(){
  #if FFDEBUG
    Serial.println("SOL2 ON");
  #endif
  #if ECM_PCB
    digitalWrite(PIN_SOL2, HIGH);
    digitalWrite(PIN_SOL2LOW, HIGH);
  #else
    Camera::HighSpeedPWM();
    analogWrite(PIN_SOL2, 255); //SOL2 POWER UP (S2 Closed)
  #endif
}
#endif

/*
#if ECM_PCB
  void Camera::sol1LowPower(){
    digitalWrite(PIN_SOL1, LOW);
  }
#endif
*/

void Camera::SelfTimerMUP(){
  #if BASICDEBUG
    Serial.println("Selftimer preMirror Up");
  #endif
  Camera::mirrorUP();
}

void Camera::shutterCLOSE(){
  #if BASICDEBUG
    Serial.println("shutterCLOSE");
  #endif
  #if ECM_PCB
    digitalWrite(PIN_SOL1, HIGH);    //ENGAGING SOLENOID 1
    //digitalWrite(PIN_SOL1LOW, HIGH); //ENGAGING SOLENOID 1 LOW POWER PIN
  #else
    Camera::HighSpeedPWM();
    analogWrite(PIN_SOL1, 255);
    delay (PowerDownDelay);
    analogWrite (PIN_SOL1, PowerDown);
  #endif
  return;
}

void Camera::shutterOPEN(){
  #if BASICDEBUG
    Serial.println("shutterOPEN");
  #endif
  #if ECM_PCB
    //digitalWrite(PIN_SOL1LOW, LOW); //SOL1 LOW POWER OFF REMAINING ENGAGED
    digitalWrite(PIN_SOL1, LOW);    //SOL1 POWER OFF JUST IN CASE
  #else
    analogWrite (PIN_SOL1, 0);
  #endif

  return; //Added 26.10.
}

void Camera::sol2Engage(){
  #if ECM_PCB
    digitalWrite(PIN_SOL2, HIGH);
    digitalWrite(PIN_SOL2LOW, HIGH);
  #else
    Camera::HighSpeedPWM();
    analogWrite(PIN_SOL2, 255); //SOL2 POWER UP (S2 Closed)
  #endif
}

void Camera::sol2Disengage(){
  #if ECM_PCB
    digitalWrite(PIN_SOL2, LOW);
    digitalWrite(PIN_SOL2LOW, LOW);
  #else
    Camera::HighSpeedPWM();
    analogWrite(PIN_SOL2, 0);
  #endif
}



void Camera::motorON(){
  #if BASICDEBUG
    Serial.println("motorON");
  #endif
  digitalWrite(PIN_MOTOR, HIGH);
}

void Camera::motorOFF(){
  #if BASICDEBUG
    Serial.println("motorOFF");
  #endif
  digitalWrite(PIN_MOTOR, LOW);
}

void Camera::mirrorDOWN(){
  #if BASICDEBUG
    Serial.println("mirrorDOWN");
  #endif
  Camera::motorON();
  while (Camera::DebouncedRead(PIN_S5) != LOW){
    #if BASICDEBUG
      Serial.println("Wait for PIN_S5 to go LOW");
    #endif
  }
  motorOFF();
  
}

void Camera::mirrorUP(){
  #if BASICDEBUG
    Serial.println("mirrorUP");
  #endif
  if(digitalRead(PIN_S3) != HIGH){
    motorON ();
  }

  while (DebouncedRead(PIN_S5) != HIGH){
    #if BASICDEBUG
      Serial.println("Wait for S5 to go high");
    #endif
  }

  motorOFF ();
}

void Camera::startMeter(int _myISO){
  lmTimer_stop(); 
  meter_set_iso(_myISO);
  meter_init();
  meter_integrate();
}



void Camera::darkslideEJECT(){
  #if SIMPLEDEBUG
    Serial.println(F("darkslideEJECT"));
  #endif
  Camera::shutterCLOSE();
  Camera::mirrorUP();
  Camera::mirrorDOWN();
  Camera::shutterOPEN();
}

void Camera::DongleFlashNormal(){
  pinMode(PIN_S2, OUTPUT);
  #if ECM_PCB
    digitalWrite(PIN_FPIN, HIGH); //F- connected from GND
  #endif
  digitalWrite(PIN_S2, LOW);      //So FFA recognizes the flash as such
  digitalWrite(PIN_FF, HIGH);       //FLASH TRIGGERING
  delay (1);                        //FLASH TRIGGERING
  digitalWrite(PIN_FF, LOW);        //FLASH TRIGGERING
  #if ECM_PCB
    digitalWrite(PIN_FPIN, LOW); //F- disconnected from GND
  #endif
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

#if !ECM_PCB
void Camera::HighSpeedPWM(){
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
}
#endif

void Camera::BlinkTimerDelay(byte led1, byte led2, byte time) {
  // DONGLE-LED BLINKS ON COUNTDOWN (10secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.
  //uint32_t startTimer = millis();
  //*******************************************************
  uint32_t steps = (time * 1000) / 4;

  // DS2408 LED BLINK

  #if SONAR
    Camera::S1F_Unfocus(); 
  #endif
  Camera::Blink (1000, steps, led1, 2);
  Camera::Blink (600, steps, led1, 2);
  #if SONAR
    Camera::S1F_Focus();
  #endif
  Camera::Blink (200, steps, led1, 2);
  steps = steps / 2;
  
  #if TIMER_MIRROR_UP
    Camera::shutterCLOSE();
  #endif
  Camera::Blink (80, steps, led1, 2);
  #if TIMER_MIRROR_UP
    Camera::SelfTimerMUP();
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
        //Serial.println("TYPE 1 - PCB Only");
        digitalWrite (PinPCB, ledState);
      }  
      else if (type == 2) {
        //Serial.println("TYPE 2 - PCB and DONGLE");
        digitalWrite (PinPCB, ledState);
        _dongle->Write_DS2408_PIO (PinDongle, ledState);
      }
    }
  }
}

void Camera::ManualExposure(int _myISO, uint8_t selector){
  uint32_t initialMillis;
  bool flashFlag = (selector>= Dongle_Flash_Limit) ? true : false;
  bool varianceFlag = ((selector>SELECTOR_LIMIT_VARIANCE) && (selector<12)) ? true : false;
  int ShutterSpeedDelay = (flashFlag) ? (ShutterSpeed[selector] - Flash_Capture_Delay) : ShutterSpeed[selector];
  int MinShutterSpeedDelay = ShutterSpeedDelay -ShutterVariance[selector];

  #if SIMPLEDEBUG
    Serial.print("take single Picture on  Manual Mode");
    Serial.print(", current Picture: ");
    Serial.println(currentPicture);
  #endif
  #if ADVANCEDEBUG
    Serial.print("Manual Exposure Debug: ");
    Serial.print("ShutterSpeed[");
    Serial.print(selector);
    Serial.print("] :");
    Serial.println(ShutterSpeed[selector]);
    Serial.print("flashflag: ");
    Serial.println(flashFlag);
    Serial.print("varianceflag:");
    Serial.println(varianceFlag);
    Serial.print("ShutterSpeedDelay:");
    Serial.println(ShutterSpeedDelay);
    
  #endif
  #if APERTURE_PRIORITY
    AperturePriority();
  #endif

  delay (YDelay);

  if(varianceFlag){
    Camera::startMeter(_myISO);
    initialMillis = millis();
    uint32_t maxMillis = initialMillis + ShutterSpeedDelay;
    
    Camera::shutterOPEN();
    delay(MinShutterSpeedDelay);

    while(meter_update() == false){
      if(millis() >= maxMillis){
        break;
      }
    }

  }
  else{
    Camera::shutterOPEN();
    initialMillis = millis();
    delay(ShutterSpeedDelay);
    /*
    while (millis() < (initialMillis + ShutterSpeedDelay)){
      //Take the Picture
    }
    */
  }

  if(flashFlag){
    Camera::FastFlash();
    delay(Flash_Capture_Delay);
  }

  #if LMDEBUG
    uint32_t shutterCloseTime = millis(); //Shutter Debug
  #endif
  Camera::ExposureFinish();
  #if LMDEBUG
    uint32_t exposureTime = shutterCloseTime - initialMillis; //Shutter Debug
    Serial.print("ExposureTime on Manualmode: ");
    Serial.println(exposureTime);
  #endif
  return;
}

void Camera::AutoExposure(int _myISO){
  #if SIMPLEDEBUG
    Serial.print("take a picture on Auto Mode with ISO: ");
    Serial.print(_myISO);
    Serial.print(", current Picture: ");
    Serial.println(currentPicture);
  #endif
  #if LMDEBUG
    Serial.print(F("AUTOMODE ISO: "));
    Serial.println(_myISO);
  #endif
  #if APERTURE_PRIORITY
    AperturePriority();
  #endif

  delay(YDelay);

  Camera::startMeter(_myISO);
  Camera::shutterOPEN();
  #if LMDEBUG
    uint32_t shutterOpenTime = millis(); //Shutter Debug
  #endif
  while (meter_update() == false){
  }
  #if LMDEBUG
    uint32_t shutterCloseTime = millis(); //Shutter Debug
  #endif

  Camera::ExposureFinish();

  #if LMDEBUG
    uint32_t exposureTime = shutterCloseTime - shutterOpenTime; //Shutter Debug
    Serial.print("ExposureTime on Automode: ");
    Serial.println(exposureTime);
  #endif
  return; //Added 26.10.
}

void Camera::AutoExposureFF(int _myISO){
  int FD_MN = (_myISO == ISO_SX70) ? FD100 : FD600;  //FlashDelay Magicnumber
  #if FFDEBUG
    Serial.print("FlashDelay Magicnumber: ");
    Serial.println(FD_MN);
  #endif
  #if SIMPLEDEBUG
    Serial.print("take a picture on Auto Mode + Fill Flash with ISO: ");
    Serial.print(_myISO);
    Serial.print(", current Picture: ");
    Serial.println(currentPicture);
  #endif

  Camera::shutterCLOSE();
  Camera::mirrorUP();
  Camera::sol2Engage();
  delay(YDelay);           //AT Yd and POWERS OFF AT FF
  
  #if LMDEBUG
    uint32_t shutterOpenTime = millis(); //Shutter Debug
  #endif

  /* SOL2 powerdown may not be needed.
  #if ECM_PCB
    //digitalWrite(PIN_SOL1, LOW); //ENTERING POWER SAVE "POWERDOWN" MODE PIN SOL2LOW REMANINS ENGAGED
  #else  
    analogWrite (PIN_SOL2, 130);    //SOL2 Powersaving
  #endif
  */

  Camera::startMeter(FD_MN);
  uint32_t integrationStartTime = millis();
  Camera::shutterOPEN(); 
  //Start FlashDelay 
  while ((meter_update() == false) && ((millis() - integrationStartTime) <= Flash_Min_Time)){ //Start FlashDelay: Integrate with the 1/3 of the Magicnumber in Automode of selected ISO
    if((millis() - integrationStartTime) >= Flash_Max_Time){ //Flash can occure anytime of the Flash Delay 56+-7ms depending on scene brightness
      break;
    }  
  }

  #if FFDEBUG
    Serial.print(millis()-integrationStartTime);
    Serial.println("ms Flash Delay Time, Flash fired!");
  #endif
  digitalWrite(PIN_FF, HIGH);  //FireFlash
  delay(Flash_Capture_Delay);   //Capture Flash 
  #if FFDEBUG
    Serial.print((millis() - flashExposureStartTime));
    Serial.println("ms FlashExposure Integrationtime");
  #endif
  digitalWrite(PIN_FF, LOW);  //Turn FF off
  Camera::sol2Disengage();
  #if ECM_PCB
    digitalWrite(PIN_FPIN, LOW); //F- disconnected from GND
  #endif
  delay(15);
  #if FFDEBUG
    Serial.print((millis()-flashExposureStartTime));
    Serial.println("ms EndFlashExposure: FF and SOL off");
  #endif
  
  #if LMDEBUG
    uint32_t shutterCloseTime = millis(); //Shutter Debug
  #endif

  #if FFDEBUG
  Serial.print("FF Status: ");
  Serial.println(FFState);
  #endif
  Camera::ExposureFinish();

  #if LMDEBUG
    uint32_t exposureTime = shutterCloseTime - shutterOpenTime; //Shutter Debug
    Serial.print("ExposureTime on Automode + FF: ");
    Serial.println(exposureTime);
  #endif

  return; //Added 26.10.
}

void Camera::ShutterB()
{
  #if SIMPLEDEBUG
     Serial.print("take B Mode Picture");
     Serial.print(", current Picture: ");
     Serial.println(currentPicture);
  #endif

  #if APERTURE_PRIORITY
    AperturePriority();
  #endif
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
  #if SIMPLEDEBUG
     Serial.print("take T Mode picture: ");
     Serial.print(", current Picture: ");
     Serial.println(currentPicture);
  #endif


  #if APERTURE_PRIORITY
    AperturePriority();
  #endif

  delay (40);
  

  #if SONAR
  while (digitalRead(PIN_S1F) == HIGH){
  }
  #endif
  Camera::shutterOPEN ();
  while(DebouncedRead(PIN_S1) == S1Logic){
    #if SIMPLEDEBUG
      Serial.println("WAITING FOR BUTTON TO DEPRESS");
    #endif
  }
  while (digitalRead(PIN_S1) == !S1Logic){
    #if SIMPLEDEBUG
      Serial.println("Shutter stays open");
    #endif
    //do nothing
  }
  Camera::FastFlash();
  delay(Flash_Capture_Delay);   //Capture Flash 

  #if APERTURE_PRIORITY
    AperturePriority();
  #endif
  #if SIMPLEDEBUG
    Serial.println("Exp finish T mode");
  #endif

  //multiple exposure test (Should not work in T Mode?!)
  ExposureFinish();
  return; //Addes 26.10.
}

void Camera::ExposureFinish()
{
  Camera::shutterCLOSE();
  #if APERTURE_PRIORITY
    Camera::sol2Disengage();
  #endif
  lmTimer_stop(); //Lightmeter Timer stop
  delay (200); //Was 20

  if(multipleExposureMode == true){
    while(digitalRead(PIN_S1) == S1Logic);
    #if MXDEBUG
      Serial.println("mEXP");
    #endif
    return;
  }
  else{
    delay (100);
    #if EJECT_AFTER_DEPRESSING
      while(digitalRead(PIN_S1) == S1Logic); // wait for s1 to be depressed
      Camera::mirrorDOWN ();
      delay (300);
    #else
      Camera::mirrorDOWN ();
      delay (300);
      while(digitalRead(PIN_S1) == S1Logic); // wait for s1 to be depressed
    #endif
    Camera::shutterOPEN();
    #if SONAR
      delay (100);
      S1F_Unfocus(); //neccesary???
      #if FOCUSDEBUG
        Serial.println("Unfocus");
      #endif
    #endif
  }
  #if SIMPLEDEBUG
    if (_dongle->checkDongle() > 0){ //Dongle present
      #if SIMPLEDEBUG
        Serial.println("Exposure Finish - Dongle Mode, ");
      #endif
    }
    else if (_dongle->checkDongle() == 0){ //No Dongle
      #if SIMPLEDEBUG
        Serial.print("Exposure Finish - No Dongle Mode, ");
      #endif
    }
  #endif
  return;
}

void Camera::multipleExposureLastClick(){
  #if MXDEBUG
    Serial.print("Multiexposure last Red Button Click, mxshots: ");
    Serial.print(mxshots);
    Serial.print(", CurrentPicture: ");
    Serial.println(currentPicture);
  #endif
  Camera::mirrorDOWN(); 
  delay(50);                             //AGAIN is this delay necessary? 100-->50
  #if SONAR
    delay (100);                             //AGAIN is this delay necessary?
    S1F_Unfocus(); //neccesary???
    #if FOCUSDEBUG
      Serial.println("Unfocus");
    #endif
  #endif
  Camera::shutterOPEN();
  multipleExposureMode = false;
}

void Camera::FastFlash(){
  pinMode(PIN_S2, OUTPUT);
  #if BASICDEBUG
    Serial.println("FastFlash");
  #endif
  #if ECM_PCB
    digitalWrite(PIN_FPIN, HIGH); //F- connected from GND
  #endif
  digitalWrite (PIN_S2, LOW);     //So FFA recognizes the flash as such
  digitalWrite(PIN_FF, HIGH);    //FLASH TRIGGERING
  delay (1);                      //FLASH TRIGGERING
  digitalWrite(PIN_FF, LOW);     //FLASH TRIGGERING
  #if ECM_PCB
    digitalWrite(PIN_FPIN, LOW); //F- disconnected from GND
  #endif
  pinMode(PIN_S2, INPUT_PULLUP);  //S2 back to normal
}



bool Camera::setLIGHTMETER_HELPER(bool state){
  #if LMHELPERDEBUG
    Serial.print("Set Lightmeterhelper status: ");
    Serial.println(state);
  #endif
  lightmeterHelper = state;
  return state;
}

bool Camera::getLIGHTMETER_HELPER(){
  #if LMHELPERDEBUG
    //Serial.println("Lightmeterhelper status: ");
    //Serial.println(lightmeterHelper));
  #endif
  return lightmeterHelper;
}
