#include "Arduino.h"
#include "camera_functions.h"
#include "meter.h"
#include "open_sx70.h"
#include "sx70_meroe_pcb.h"
//#include "Clickbutton.h"
#include "settings.h"
#include "uDongle2.h"

static byte multipleExposureMode = false;

Camera::Camera(uDongle *dongle)
{
  _dongle = dongle;
  //   io_init();
  //  init_EEPROM();
}
//const uint8_t DEBOUNCECOUNT = 10;

void Camera::shutterCLOSE()
{
  #if BASICDEBUG
    Serial.println ("shutterCLOSE");
  #endif
    Camera::HighSpeedPWM();
    analogWrite(PIN_SOL1, 255);
    delay (PowerDownDelay);
    analogWrite (PIN_SOL1, PowerDown);
    
}

void Camera::shutterOPEN()
{
  #if BASICDEBUG
    Serial.println ("shutterOPEN");
  #endif
    analogWrite (PIN_SOL1, 0);
    
}

void Camera::motorON()
{
#if BASICDEBUG
  Serial.println("motorON");
#endif
  digitalWrite(PIN_MOTOR, HIGH);
}

void Camera::motorOFF()
{
#if BASICDEBUG
  Serial.println ("motorOFF");
#endif
  digitalWrite(PIN_MOTOR, LOW);
}

void Camera::mirrorDOWN()
{
  #if BASICDEBUG
    Serial.println ("mirrorDOWN");
  #endif
  //unsigned long imillis = millis();
  Camera::motorON();

  while (Camera::DebouncedRead(PIN_S5) != LOW){
    //why?
  }
  motorOFF();
  
}

void Camera::mirrorUP()
{
  #if BASICDEBUG
    Serial.println ("mirrorUP");
  #endif
  motorON ();
  //while (digitalRead(PIN_S5) != HIGH)            //waiting for S5 to OPEN do NOTHING
  while (DebouncedRead(PIN_S5) != HIGH)
  {
    //wait for S% to go low
  }
  //S5 OPENS
  //S1 MAY BE OPEN NOW (DON'T KNOW HOW TO DO THIS YET)
  // Motor Brake
  motorOFF ();
  
}

void Camera::darkslideEJECT()
{
  #if SIMPLEDEBUG
    Serial.println ("darkslideEJECT camerafunctons");
  #endif
  Camera::shutterCLOSE();
  Camera::mirrorUP();
  Camera::mirrorDOWN();
  Camera::shutterOPEN();
  
}

void Camera::DongleFlashNormal()
{
  pinMode(PIN_S2, OUTPUT);
  digitalWrite(PIN_SOL2, LOW); //So FFA recognizes the flash as such
  digitalWrite(PIN_FFA, HIGH);   //FLASH TRIGGERING
  delay (1);                 //FLASH TRIGGERING
  digitalWrite(PIN_FFA, LOW);    //FLASH TRIGGERING
  pinMode(PIN_SOL2, INPUT_PULLUP);  //S2 back to dongle mode
}

void Camera::Ydelay ()
{
  //Only one 120ms delay.
  delay (120);
  
}

bool Camera::DebouncedRead(uint8_t pin)
{
  bool lastState = digitalRead(pin);
  uint8_t stablecount = 0;
  while (stablecount < DEBOUNCECOUNT)
  {
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

void Camera::HighSpeedPWM()
{
  //const byte n = 224;  // for example, 71.111 kHz
  const byte n =224;
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
  TCCR2B = bit (WGM22) | bit (CS20);         // fast PWM, no prescaler
  OCR2A =  n;                                // Value to count to - from table
  OCR2B = ((n + 1) / 2) - 1;                 // 50% duty cycle
  //THIS AFFECTS OUTPUT 3 (Solenoid1) AND OUTPUT 11 (Solenoid2)
}

void Camera::BlinkTimerDelay(byte led1, byte led2, byte time) {
  // DONGLE-LED BLINKS ON COUNTDOWN (10secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.
  //unsigned long startTimer = millis();
  //*******************************************************
  unsigned long steps = (time * 1000) / 4;
  // DS2408 LED
  Camera::Blink (1000, steps, led1, 2);
  Camera::Blink (600, steps, led1, 2);
  Camera::Blink (200, steps, led1, 2);
  steps = steps / 2;
  Camera::Blink (80, steps, led1, 2);
  Camera::Blink (80, steps, led2, 2);
}

// blink (blink interval=blinking speed, timer=duration blinking, Pin=pin of LED
//type 1 = LED
//type 0 = Piezo //REMOVED NO MORE PIEZO!
//type 2 = DS2480 //REMOVED DON'T KNOW HOW TO DO ITT
// blink is a standalone function

void Camera::Blink (unsigned int interval, int timer, int Pin, byte type)
{
  int ledState = LOW;             // ledState used to set the LED
  pinMode(Pin, OUTPUT);
  unsigned long previousMillis = 0;        // will store last time LED was updated
  unsigned long currentMillisTimer = millis();
  while (millis() < (currentMillisTimer + timer))
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == 0) {
        ledState = 1;
      } else {
        ledState = 0;
      }
      // set the LED with the ledState of the variable:
      if (type == 1) {
        digitalWrite (Pin, ledState);
      }  else if (type == 2) {
        //        Serial.println ("TYPE 2");
        _dongle->Write_DS2408_PIO (Pin, ledState);
      }
    }
  }
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
  Camera::shutterOPEN();
  multipleExposureMode = false;
}

void Camera::ManualExposure(int _selector, bool mEXP) //ManualExposure
{
  currentPicture++; 
  WritePicture(currentPicture);
  #if SIMPLEDEBUG
    Serial.print("take single Picture on  Manual Mode");
    Serial.print(", current Picture: ");
    Serial.println(currentPicture);
  #endif
  if(multipleExposureMode == false){
    Camera::shutterCLOSE ();
    delay (100); //added to fix bad photos WITH LESS delay IS THIS NEEDED??? auto does not have it. -ZANE
    Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  }
  while (digitalRead(PIN_S3) != HIGH){
    //waiting for S3 to OPEN
    //Serial.println("wait for s3");
  }
  delay (YDelay);
  // startCounterCalibration();
  //int ShutterSpeed[] = {17, 20, 23, 25, 30, 35, 45, 55, 68, 102, 166, AUTO600BW, AUTO600, AUTO100, POST, POSB }; //reduced speeds from 25 (slot5) to compensate flash firing
  int ShutterSpeedDelay = (ShutterSpeed[_selector] + ShutterConstant) ;
  if (_selector >= 6)
  {
    ShutterSpeedDelay = (ShutterSpeedDelay - flashDelay);
  }
  #if ADVANCEDDEBUG
    extern int selector;
    Serial.print("Manual Exposure Debug: ");
    //Serial.print("ShutterSpeed[");
    //Serial.print(_selector);
    //Serial.print("] :");
    //Serial.println(ShutterSpeed[_selector]);
    Serial.print("ShutterConstant:");
    Serial.println(ShutterConstant);
    Serial.print("ShutterSpeedDelay:");
    Serial.println(ShutterSpeedDelay);
  #endif

  //sei(); //Interupts restart -- Take the Picture
  Camera::shutterOPEN();  //SOLENOID OFF MAKES THE SHUTTER TO OPEN!
  //delay (ShutterSpeedDelay);
  unsigned long initialMillis = millis();
  while (millis() <= (initialMillis + ShutterSpeedDelay))
    ;
  if (_selector >= 3) // changed the flash selection
  {
    #if SIMPLEDEBUG
        Serial.println("FF");
    #endif
    Camera::FastFlash ();
  }

  //multiple exposure test
  Camera::ExposureFinish(mEXP);
  
}

void Camera::AutoExposure(int _myISO, bool mEXP)
{
  meter_set_iso(_myISO); //Set the correct compare Table for the set ISO
  if(!multipleExposureMode){
    Camera::shutterCLOSE();
    Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  }
  while (digitalRead(PIN_S3) != HIGH){
    //waiting for S3 to OPEN}
  }
  delay(YDelay);                            //S3 is now open start Y-delay (40ms)
  //startCounter();
  meter_init();
  meter_integrate();
  Camera::shutterOPEN();
  while (meter_update() == false){
  }
  //multiple exposure test
  Camera::ExposureFinish(mEXP);
}

void Camera::FlashBAR() //FlashBAR
{
  currentPicture++; 
  WritePicture(currentPicture);
  #if SIMPLEDEBUG
     Serial.print("take Camera Flashbar picture");
     Serial.print(", current Picture: ");
     Serial.println(currentPicture);
  #endif
  Camera::HighSpeedPWM();
  analogWrite(PIN_SOL2, 255);
  if(!multipleExposureMode){
    Camera::shutterCLOSE ();
    Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  }
  while (digitalRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN
    ;
  analogWrite (PIN_SOL2, 130);
  delay (YDelay);                               //S3 is now open start Y-delay (40ms)
  Camera::shutterOPEN ();
  delay (66);
  digitalWrite(PIN_FFA, HIGH);
  delay (2);
  analogWrite (PIN_SOL2, 0);
  digitalWrite(PIN_FFA, LOW);
  delay (20);
  Camera::shutterCLOSE();
  delay (200);                             //AGAIN is this delay necessary?
  Camera::mirrorDOWN();                          //Motor starts, let bring the mirror DOWN
  delay (200);                             //AGAIN is this delay necessary?
  Camera::shutterOPEN();
  
}

void Camera::ShutterB(bool mEXP)
{
  currentPicture++; 
  WritePicture(currentPicture);
  #if SIMPLEDEBUG
     Serial.print("take B Mode Picture");
     Serial.print(", current Picture: ");
     Serial.println(currentPicture);
  #endif
  //Why turn of the LIGHT?
  //digitalWrite(PIN_LED2, LOW);
  //digitalWrite(PIN_LED1, LOW);
  if(!multipleExposureMode){
    Camera::shutterCLOSE ();
    Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!
  }
  while (digitalRead(PIN_S3) != HIGH){            //waiting for S3 to OPEN˚
    //Serial.println("Wait for s3 to open (HIGH)");
    //  while (openSX70.DebouncedRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN˚
  }
  delay (40);                               //S3 is now open start Y-delay (40ms)
  if (_dongle->switch2() ==  1) //CASE Activate Solenoid 2 for Camfollower Function
  {
    analogWrite(PIN_SOL2, 255); //Solenoid 2 activating
  }
  //MutlipleExposure Finish()
  Camera::shutterOPEN ();
  if (_dongle->switch2() ==  1) //CASE Activate Solenoid 2 for Camfollower Function
  {
    analogWrite(PIN_SOL2, 130);
  }
  while (digitalRead(PIN_S1) == S1Logic){
    //  while (sw_S1.depressed)
    //Serial.println("Red Button Pressed)");
  }
  Camera::FastFlash();
  Camera::shutterCLOSE ();
  if (_dongle->switch2() ==  1) //CASE Deactivate Solenoid 2 for Camfollower Function
  {
    analogWrite(PIN_SOL2, 0);
  }

  //multiple exposure test
  if(!mEXP){
    delay (200);
    Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (200);                             //AGAIN is this delay necessary?
    Camera::shutterOPEN();
  }
  
}

void Camera::ShutterT(bool mEXP)
{
  currentPicture++; 
  WritePicture(currentPicture);
  #if SIMPLEDEBUG
     Serial.print("take T Mode picture: ");
     Serial.print(", current Picture: ");
     Serial.println(currentPicture);
  #endif
  //Why turn off the LIGHT?
  //digitalWrite(PIN_LED2, LOW);
  //digitalWrite(PIN_LED1, LOW);
  if(!multipleExposureMode){
    Camera::shutterCLOSE ();
    Camera::mirrorUP();  
  }
   //Motor Starts: MIRROR COMES UP!!!
  while (DebouncedRead(PIN_S3) != HIGH){
    //waiting for S3 to OPEN˚
  }
  delay (40);                               //S3 is now open start Y-delay (40ms)
  //MutlipleExposure Finish()
  #if SONAR
  while (digitalRead(PIN_S1F) == HIGH){
  
  }
  #endif
  Camera::shutterOPEN ();
  while (digitalRead(PIN_S1) == !S1Logic){
    //nothing
    //digitalWrite(PIN_LED2, HIGH); //Turn Blue Led on while Shutter is Open
  }
  if (digitalRead(PIN_S1) == S1Logic)
  {
    Camera::FastFlash();
    Camera::shutterCLOSE ();
  }

  //multiple exposure test
  if(mEXP == false){
    delay (200);                             //AGAIN is this delay necessary?
    Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (200);                             //AGAIN is this delay necessary?
    Camera::shutterOPEN();
  }
}

void Camera::ExposureFinish(bool mEXP)
{
  Camera::shutterCLOSE();
  lmTimer_stop(); //Timer stop
  delay (200); //Was 20

  if(mEXP == true){
    while(digitalRead(PIN_S1) == S1Logic){
      //waiting for s1 to stop being depressed... get well soon
    }
    Serial.println("mEXP");
    multipleExposureMode = true;
  }
  else if (_dongle->checkDongle() > 0){ //Dongle present
    delay (100);                             //AGAIN is this delay necessary?
    Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (100);                  //WAS 60           //AGAIN is this delay necessary?
    while(digitalRead(PIN_S1) == S1Logic){
      //waiting for s1 to stop being depressed... get well soon
    }
    Camera::shutterOPEN();
    #if SIMPLEDEBUG
      Serial.println("Exposure Finish - Dongle Mode, ");
    #endif
  }
  else if (_dongle->checkDongle() == 0){ //No Dongle
    delay (100);                             //AGAIN is this delay necessary?
    Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (300);                 //WAS 60           //AGAIN is this delay necessary?
    while(digitalRead(PIN_S1) == S1Logic){
      //waiting for s1 to stop being depressed... get well soon
    }
    Camera::shutterOPEN();
    #if SIMPLEDEBUG
      Serial.print("Exposure Finish - No Dongle Mode, ");
    #endif
     
  }
}

void Camera::FastFlash()
{
  #if BASICDEBUG
    Serial.println("FastFlash");
  #endif
  pinMode(PIN_S2, OUTPUT);
  //     delay (2);
  digitalWrite (PIN_S2, LOW);     //So FFA recognizes the flash as such
  //     delay (2);
  digitalWrite(PIN_FFA, HIGH);    //FLASH TRIGGERING
  delay (1);                      //FLASH TRIGGERING
  digitalWrite(PIN_FFA, LOW);     //FLASH TRIGGERING
  pinMode(PIN_S2, INPUT_PULLUP);  //S2 back to normal
}

bool Camera::setLIGHTMETER_HELPER(bool active){
  #if LMDEBUG
    Serial.print("Set Lightmeterhelper status: ");
    Serial.println(active);
  #endif
  lightmeterHelper = active;
  return active;
}

bool Camera::getLIGHTMETER_HELPER(){
  #if LMDEBUG
    //Serial.println("Lightmeterhelper status: ");
    //Serial.println(lightmeterHelper));
  #endif
  return lightmeterHelper;
}
