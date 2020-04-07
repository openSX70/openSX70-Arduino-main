void FlashBAR()
{

#if SIMPLEDEBUG
  Serial.println ("CAMERA FLASH");
#endif

  CurrentPicturePack = EEPROM.read(4) ;
  byte PictureType = 2;
  eepromUpdate ();


  //                   byte PictureType = 2;
  //                   CurrentPicture = EEPROM.read(4) ;
  //                   eepromUpdate ();
  HighSpeedPWM ();
  analogWrite(Solenoid2, 255);
  shutterCLOSE ();


  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
    ;
  analogWrite (Solenoid2, 130);
  Ydelay ();                               //S3 is now open start Y-delay (40ms)

  shutterOPEN ();
  delay (66);
  //                  delay (80);
  digitalWrite(FFA, HIGH);
  delay (2);
  analogWrite (Solenoid2, 0);
  digitalWrite(FFA, LOW);
  delay (20);
  shutterCLOSE();
  delay (200);                             //AGAIN is this delay necessary?
  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
  delay (200);                             //AGAIN is this delay necessary?
  shutterOPEN();
  shots = 0;
  return;

}

void DongleFlashF8 ()
{


#if SIMPLEDEBUG
  Serial.println ("DONGLE FLASH F8");
#endif

  //                 byte PictureType = 4;
  //                 CurrentPicture = EEPROM.read(4) ;
  //
  //                 eepromUpdate ();

  if (takePicture == true)

  {

    byte PictureType = 6;
    eepromUpdate ();


    //         HighSpeedPWM ();
    //         analogWrite(Solenoid2, 255);
    shutterCLOSE ();


    mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
    while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
      ;
    //         analogWrite (Solenoid2, 130);
    Ydelay ();                               //S3 is now open start Y-delay (40ms)



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
  }
}

void DongleFlashNormal ()
{


#if SIMPLEDEBUG
  Serial.println ("DONGLE FLASH NORMAL");
#endif

  if (takePicture == true)
    /* {

                digitalWrite(FFA, HIGH);
                Serial.println ("FFA, HIGH");
                delay (45);
      //               analogWrite (Solenoid2,0);
                digitalWrite(FFA, LOW);
                Serial.println ("FFA, LOW");

      }*/
  {

    //               byte PictureType = 5;
    eepromUpdate ();

    HighSpeedPWM ();
    analogWrite(Solenoid2, 255);
    shutterCLOSE ();

    mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
    while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
      ;
    analogWrite (Solenoid2, 130);
    Ydelay ();                               //S3 is now open start Y-delay (40ms)


    shutterOPEN ();
    //                  delay (66);
    delay (80);
    Write_DS2408_PIO (7, 1);
    //                  digitalWrite(FFA, HIGH);
    delay (1);
    analogWrite (Solenoid2, 0);
    //                  digitalWrite(FFA, LOW);
    Write_DS2408_PIO (7, 0);
    delay (10);
    shutterCLOSE();

    delay (200);                             //AGAIN is this delay necessary?
    mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (200);                             //AGAIN is this delay necessary?

    shutterOPEN();
    shots = 0;
    return;
  }
}
void FastFlash ()
{
  Write_DS2408_PIO (7, 1);
  delay (1);
  //Serial.println ("Flash!");
  Write_DS2408_PIO (7, 0);
  return;
}
