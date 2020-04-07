/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"

#include "camera_functions.h"
#include "meter.h"
#include "open_sx70.h"
//
//
#include "sx70_alpha_rw_pcb.h"
//#include "Clickbutton.h"
#include "settings.h"
//
#include "uDongle2.h"


Camera::Camera(uDongle *dongle)
{
  _dongle = dongle;
  //   io_init();
  //  init_EEPROM();

}
//const uint8_t DEBOUNCECOUNT = 10;
void Camera::shutterCLOSE()
{
#if SIMPLEDEBUG
  Serial.println ("shutterCLOSE");
#endif
  Camera::HighSpeedPWM();
  analogWrite(PIN_SOL1, 255);
  delay (PowerDownDelay);
  analogWrite (PIN_SOL1, PowerDown);
  return;
}
void Camera::shutterOPEN()
{
#if SIMPLEDEBUG
  Serial.println ("shutterOPEN");
#endif
  analogWrite (PIN_SOL1, 0);
  return;

}
void Camera::motorON()
{
#if SIMPLEDEBUG
  Serial.print ("motorON :");
  Serial.println (PIN_MOTOR);
#endif
  digitalWrite(PIN_MOTOR, HIGH);
}
void Camera::motorOFF()
{
#if SIMPLEDEBUG
  Serial.println ("motorOFF");
#endif
  digitalWrite(PIN_MOTOR, LOW);
}

void Camera::mirrorDOWN()
{
#if SIMPLEDEBUG
  Serial.println ("mirrorDOWN");
#endif

  //unsigned long imillis = millis();
  Camera::motorON ();

  while (Camera::DebouncedRead(PIN_S5) != LOW)
    //         while(digitalRead(PIN_S5) != LOW)
    ;
  motorOFF ();
  return;
}    //end of void mirrorDOWN()
//***************************************************************************************************************************************

void Camera::mirrorUP()
{
#if SIMPLEDEBUG
  Serial.println ("mirrorUP");
#endif
  motorON ();
  //while (digitalRead(PIN_S5) != HIGH)            //waiting for S5 to OPEN do NOTHING
  while (DebouncedRead(PIN_S5) != HIGH)
    ;
  //S5 OPENS
  //S1 MAY BE OPEN NOW (DON'T KNOW HOW TO DO THIS YET)
  // Motor Brake
  motorOFF ();
  ////////////////////////////Serial.println ("mU");
  return;

}   //end of mirrorUP();



void Camera::darkslideEJECT()
{
#if SIMPLEDEBUG
  Serial.println ("darkslideEJECT");
#endif
  Camera::shutterCLOSE();
  Camera::mirrorUP();
  Camera::mirrorDOWN();
  Camera::shutterOPEN();
  return;
}
void Camera::DongleFlashNormal()
{
  pinMode(PIN_S2, OUTPUT);
  digitalWrite (PIN_SOL2, LOW); //So FFA recognizes the flash as such
  digitalWrite(PIN_FFA, HIGH);   //FLASH TRIGGERING
  delay (1);                 //FLASH TRIGGERING
  digitalWrite(PIN_FFA, LOW);    //FLASH TRIGGERING
  pinMode(PIN_SOL2, INPUT_PULLUP);  //S2 back to dongle mode
}

void DongleFlashF8 ()
{ /*
    #if SIMPLEDEBUG
    Serial.println ("DONGLE FLASH F8");
    #endif

    //                 byte PictureType = 4;
    //                 CurrentPicture = EEPROM.read(4) ;
    //
    //                 eepromUpdate ();

    //  if (takePicture == true)

    {

     byte PictureType = 6;
    //    eepromUpdate ();


     //         HighSpeedPWM ();
     //         analogWrite(Solenoid2, 255);
     Camera::shutterCLOSE ();


     mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
     ///////while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
     while (DebouncedRead(S3) != HIGH)            //waiting for S3 to OPEN

       ;
     //         analogWrite (Solenoid2, 130);
    delay     (YDelay);                               //S3 is now open start Y-delay (40ms)



     shutterOPEN ();
     //                  delay (66);
     delay (80);
     Write_DS2408_PIO (7, 1); // this is for dongle (jack flash)
     //                  digitalWrite(FFA, HIGH); //this is for in-camera flash
     delay (1);
     //                  analogWrite (Solenoid2,0);
     //                  digitalWrite(FFA, LOW);
     Write_DS2408_PIO (7, 0);
     delay (10u);
     shutterCLOSE();

     delay (500);

     delay (200);                             //AGAIN is this delay necessary?
     mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
     delay (200);                             //AGAIN is this delay necessary?

     shutterOPEN();
     shots = 0;
     return;
    }*/
}

void Camera::Ydelay ()
{

  //Only one 120ms delay. Switch 2 for other purposes!!

  delay (120);
  /*                   if (switch2 ==  0) {
                      //NORMAL DELAY
                      delay (40);                                 //S3 is now open start Y-delay (40ms)

                          }
                          else
                          {
                      delay (200);                              //LONG DELAY SELECTED
                         } */
  return;

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
void Camera::HighSpeedPWM ()
{
  const byte n = 224;  // for example, 71.111 kHz
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
  OCR2A =  n;                                // from table
  OCR2B = ((n + 1) / 2) - 1;                 // 50% duty cycle
  //THIS AFFECTS OUTPUT 3 AND OUTPUT 11 (Solenoid1 and Solenoid2)
}

void Camera::BlinkTimerDelay(byte led1, byte led2, byte time) {
  // DONGLE-LED BLINKS ON COUNTDOWN (10secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.

  //unsigned long startTimer = millis();
  //*******************************************************

  //steps time*1000/4
  unsigned long steps = (time * 1000) / 4;

  // DS2408 LED
  Camera::Blink (1000, steps, led1, 2);
  //cancelButton();
  Camera::Blink (600, steps, led1, 2);
  //cancelButton();
  Camera::Blink (200, steps, led1, 2);
  //cancelButton();
  steps = steps / 2;
  Camera::Blink (80, steps, led1, 2);
  Camera::Blink (80, steps, led2, 2);
  //cancelButton();

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
      //gitalWrite(Pin, ledState);
      if (type == 1) {
        digitalWrite (Pin, ledState);
      }  else if (type == 2) {
        //        Serial.println ("TYPE 2");
        _dongle->Write_DS2408_PIO (Pin, ledState);
      }
    }
  }

} //   END OF Blink FUNCTION

void Camera::AutoExposure(int _myISO)
{
  currentPicture++;
  WritePicture(currentPicture);
  meter_set_iso(_myISO);
  Camera::shutterCLOSE ();

  Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  while (digitalRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN
    ;
  //         analogWrite (PIN_SOL2, 130);
  delay(YDelay);                               //S3 is now open start Y-delay (40ms)
  //  startCounter();
  //
  meter_init();
  meter_integrate();
  Camera::shutterOPEN ();
  //                  startMillis = millis;
  //                  shots =  ++shots;
  //if(meter_update()) camera_stop_exposure();
  while (meter_update() == false)
    ;
  Camera::ExposureFinish();
  return;
}

void Camera::ExposureFinish()
{
  Camera::shutterCLOSE();
  delay (200); //Was 20
  //switch1 = Read_DS2408_PIO(1);
  //Serial.print ("finish, switch1 =");
  //Serial.println (switch1);


  if ((_dongle->checkDongle() > 0) && (_dongle->switch1() == 1))
    //if ((_dongle->checkDongle() > 0)

  {
    shots++;
    return;

    //  } else if (_dongle->switch1()== 0)
  } //else if (_dongle->checkDongle() == 0)
  {
    delay (100);                             //AGAIN is this delay necessary?
    Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (300);                  //WAS 60           //AGAIN is this delay necessary?
    Camera::shutterOPEN();
    shots = 0;
    //tsl237_init();
    return;
  }
}
void Camera::ManualExposure() //ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure ManualExposure Manual
{
#if SIMPLEDEBUG
  Serial.println ("take Manual picture");
#endif
  currentPicture++;
  WritePicture(currentPicture);

  Camera::shutterCLOSE ();

  //                 delay (200); //added to fix bad photos
  delay (100); //added to fix bad photos WITH LESS delay

  Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  while (digitalRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN
    ;
  delay (YDelay);

  // startCounterCalibration();

  int ShutterSpeedDelay = ((ShutterSpeed[_dongle->selector()]) + ShutterConstant) ;
  if (_dongle->selector() >= 6)
  {
    ShutterSpeedDelay = (ShutterSpeedDelay - flashDelay);
  }

#if SIMPLEDEBUG

  Serial.print("ShutterSpeed[");
  Serial.print(selector);
  Serial.print("] :");
  Serial.println(ShutterSpeed[selector]);

  Serial.print("ShutterConstant:");
  Serial.println(ShutterConstant);

  Serial.print("ShutterSpeedDelay:");
  Serial.println(ShutterSpeedDelay);

#endif

  sei();
  Camera::shutterOPEN();  //SOLENOID OFF MAKES THE SHUTTER TO OPEN!
  unsigned long initialMillis = millis();
  //delay (ShutterSpeedDelay);
  while (millis() <= (initialMillis + ShutterSpeedDelay))
    ;

  if (_dongle->selector() >= 3) // changed the flash selection
  {
    Camera::FastFlash ();

#if SIMPLEDEBUG
    Serial.println("FF");
#endif
  }
  Camera::ExposureFinish();
  return;
}

void Camera::FlashBAR() //FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR FlashBAR
{

#if SIMPLEDEBUG
  Serial.println ("CAMERA FLASH");
#endif

  //  currentPicture =  ReadPicture();
  currentPicture++;
  WritePicture(currentPicture);
  // byte PictureType = 2;


  //                   byte PictureType = 2;
  //                   CurrentPicture = EEPROM.read(4) ;
  //                   eepromUpdate ();

  Camera::HighSpeedPWM ();
  analogWrite(PIN_SOL2, 255);
  Camera::shutterCLOSE ();


  Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  while (digitalRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN
    ;
  analogWrite (PIN_SOL2, 130);
  delay (YDelay);                               //S3 is now open start Y-delay (40ms)

  Camera::shutterOPEN ();
  delay (66);
  //                  delay (80);
  digitalWrite(PIN_FFA, HIGH);
  delay (2);
  analogWrite (PIN_SOL2, 0);
  digitalWrite(PIN_FFA, LOW);
  delay (20);

  Camera::shutterCLOSE();
  delay (200);                             //AGAIN is this delay necessary?
  Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
  delay (200);                             //AGAIN is this delay necessary?
  Camera::shutterOPEN();
  shots = 0;
  ++currentPicture;
  WritePicture(currentPicture);
  return;

}

void Camera::FastFlash ()
{
  pinMode(PIN_S2, OUTPUT);
  //     delay (2);
  digitalWrite (PIN_S2, LOW); //So FFA recognizes the flash as such
  //     delay (2);

  digitalWrite(PIN_FFA, HIGH);   //FLASH TRIGGERING
  delay (1);                 //FLASH TRIGGERING
  digitalWrite(PIN_FFA, LOW);    //FLASH TRIGGERING

  pinMode(PIN_S2, INPUT_PULLUP);  //S2 back to normal

}

void Camera::ShutterB()
{
#if SIMPLEDEBUG
  Serial.println ("SHUTTER B");
#endif
  currentPicture++;
  WritePicture(currentPicture);
  digitalWrite(PIN_LED2, LOW);
  digitalWrite(PIN_LED1, LOW);
  Camera::shutterCLOSE ();

  Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  while (digitalRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN˚
    //  while (openSX70.DebouncedRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN˚
    ;
  delay (40);                               //S3 is now open start Y-delay (40ms)

  if (_dongle->switch2() ==  1) //CASE DONGLE FLASH AT THE END OF B
  {
    analogWrite(PIN_SOL2, 255);
  }
  Camera::shutterOPEN ();
  if (_dongle->switch2() ==  1) //CASE DONGLE FLASH AT THE END OF B
  {
    analogWrite(PIN_SOL2, 130);
  }
  while (digitalRead(PIN_S1) == S1Logic)
    //  while (sw_S1.depressed)
    ;
//  if (_dongle->switch2() ==  1) //CASE DONGLE FLASH AT THE END OF B
    Camera::FastFlash();
  /*{
    Write_DS2408_PIO (7, 1);
    delay (1);
    analogWrite (Solenoid2, 0);
    Write_DS2408_PIO (7, 0);
    }*/
  Camera::shutterCLOSE ();

  if (_dongle->switch2() ==  1) //CASE DONGLE FLASH AT THE END OF B
  {
    analogWrite(PIN_SOL2, 0);
  }

  delay (200);                             //AGAIN is this delay necessary?
  Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
  delay (200);                             //AGAIN is this delay necessary?

  Camera::shutterOPEN();
  //shots = 0;

  return;
}

void Camera::ShutterT()
{
#if SIMPLEDEBUG
  Serial.println ("SHUTTER T");
#endif
  digitalWrite(PIN_LED2, LOW);
  digitalWrite(PIN_LED1, LOW);
  currentPicture++;
  WritePicture(currentPicture);
  //if (_dongle->switch2() ==  1) //CASE DONGLE FLASH AT THE END OF B
  //    Camera::FastFlash();

  Camera::shutterCLOSE ();


  Camera::mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  //while (digitalRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN
  while (DebouncedRead(PIN_S3) != HIGH)            //waiting for S3 to OPEN˚
    ;
  delay (40);                               //S3 is now open start Y-delay (40ms)


  Camera::shutterOPEN ();

  while (digitalRead(PIN_S1) == !S1Logic)
    //  while (DebouncedRead(PIN_S1) == !S1Logic)
    ;

  if (digitalRead(PIN_S1) == S1Logic)
  {
    Camera::FastFlash();
    Camera::shutterCLOSE ();
  }

  delay (200);                             //AGAIN is this delay necessary?
  Camera::mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
  delay (200);                             //AGAIN is this delay necessary?

  Camera::shutterOPEN();
  //shots = 0;
  return;

}
