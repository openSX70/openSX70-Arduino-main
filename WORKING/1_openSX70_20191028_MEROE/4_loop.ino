void loop()
{
  Serial.println (
  int old_device_count = device_count;
  device_count = ds.findsingle(&dongleDevice);
  //  Serial.print ("device_count: ");
  //  Serial.println (device_count);

  if (device_count != 0 && old_device_count == 0)
  {
    //OPTION this is only to blink the LED on the dongle if inserted late. Only the first time.
    initializeDS2408(); //Pitsie fix for non-blinking LED on dongle
    /*    Write_DS2408_PIO (6, 1);
        delay (200);
        Write_DS2408_PIO (6, 0);
    */

#if LIGHTMETER
    byte cISO = EEPROM.read(20);
    if (cISO == 60)
    {
      simpleBlink(2, 6);
      output_compare = A600;
    }
    else if (cISO == 10)
    {
      simpleBlink(1, 6);

      output_compare = A100;
    }
#endif

#if !LIGHTMETER
    simpleBlink(3, 6);
#endif
  }
  //
  if (device_count == 0)
    //if (device_count == 0 && old_device_count == 0)
  {
    if (digitalRead(S2) == LOW)
      selector = 100;   //Flashbar
    else
      selector = 200;   //dongleless

    //    switch1 = 0;
    //    switch2 = 0;

output_compare = ISO();
  }
  //else
  if (device_count != 0) {
    // READ DS2408

    selector = Read_DS2408_PIO(0);
    switch1 = Read_DS2408_PIO(1);
    switch2 = Read_DS2408_PIO(2);

    //	Serial.println(ShutterSpeed[selector]);

  }

  if (((ShutterSpeed[selector]) == AUTO600))
    output_compare = A600;

  if (((ShutterSpeed[selector]) == AUTO100))
    output_compare = A100;

  //Serial.print ("predicted exposure: ");
  //Serial.println (PredictedExposure());
  /////////////////////////////////////////////////////////////////
  /*
    int exposure;
    //byte
    //cISO = EEPROM.read(20);

    #if LIGHTMETER

    #if VFled

    //byte cISO = EEPROM.read(20);
  */
  //THIS IS **JUST** FOR dongleless auto setting

  if  ((switch2 == 1) && (switch1 == 1) && ((ShutterSpeed[selector]) == AUTO600))
    // 	  cISO = EEPROM.read(20);
  {

    if (cISO != 60)
    {
      EEPROM.update(20, 60);
      //				  Serial.print("cISO changed to 100");
      output_compare = A600;
      cISO = 60;
      simpleBlink(2, 6);
    }
  }
  if ((switch2 == 1) && (switch1 == 1) && ((ShutterSpeed[selector]) == AUTO100)) //this is to Set dongleless ISO
    //  	  cISO = EEPROM.read(20);
  {
    output_compare = A100;

    if (cISO != 10)
    {
      EEPROM.update(20, 10);
      //				  Serial.print("cISO changed to 100");
      output_compare = A100;
      cISO = 10;
      simpleBlink(1, 6);
    }
  }

  //TEST CODE VFl

#if VFled

  if ((switch2 == 1) && (selector >= 0) && (selector < 12))  // MANUAL LM "helper" function (Manual helper)
  {

    // int nearest(int x, int myArray[], int elements, bool sorted)
    int slot = nearest(PredictedExposure(), ShutterSpeed, 11, 1);


    if (selector < slot)
    {
      digitalWrite(led2, HIGH);
      digitalWrite(led1, LOW);
      //return;
    }
    else if (selector > slot)
    {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      //return;
    }
    else if (selector == slot)
    {

      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      //return;
    }


    /*  {
        Serial.print ("(MANUAL (ISO ");
        Serial.print (ISO);
        Serial.print (") Predicted exposure: ");
        Serial.println (PredictedExposure());
      }
    */

  }

  if ((selector == 12) || (selector == 13) || (selector == 200)) //AUTO: LOW LIGHT INDICATOR
  {
/*
       {
        Serial.print ("(AUTO (ISO ");
        Serial.print (ISO);
        Serial.print (") Predicted exposure: ");
        Serial.println (PredictedExposure());
        Serial.print ("counter: ");
        Serial.println ( counter);

       }
    
*/

    if (PredictedExposure() > 60) // IF EXPOSURE IS SLOWER THAT 50ms I turn ON LED indicating LOW Light
    {
      digitalWrite(led2, HIGH);
    } else if (PredictedExposure() <= 60)
    {
      digitalWrite(led2, LOW);
    }


  }

#endif
  //=======================================================================================




  //=======================================================================================


  //WHAT TO DO WHEN POWER-UP:
  //  S8     S9
  //closed  open  --> EJECT DARKSLIDE (DEFAULT)
  // open  closed --> FILM REACH 0 (NO FLASH)
  // open   open  --> NORMAL OPERATION 10 TO 1


//Serial.println ("antes");

  // STATE 1: EJECT DARKSLIDE:*************************************************************************************************************************************************

  if (digitalRead(S8) == HIGH && digitalRead(S9) == LOW)
    //EJECT DARK SLIDE
  {
    CurrentPicturePack = 0;
    EEPROM.write(4, CurrentPicturePack);
    //OPTION TURN ON AND OFF LED WHILE DARKSLIDE EJECT
    Write_DS2408_PIO (6, 0); //DONGLE LED OFF
    Write_DS2408_PIO (6, 1); //DONGLE LED ON
    darkslideEJECT();
    Write_DS2408_PIO (6, 0);  //DONGLE LED OFF

#if SIMPLEDEBUG
    Serial.println("STATE1: EJECT DARK SLIDE");
#endif
    return;
  }
  //  CurrentPicture = EEPROM.read(4) ;

  //STATE 2: PACK IS EMPTY--> NO WASTE OF FLASH *********************************************************************************************************************************
  //    if ((digitalRead(S8) == LOW && digitalRead(S9) == HIGH) || (CurrentPicture >= 8))

  // changed this to allow shooting until counter is actually 0, in case "something" happens and I loose count!

  if ((digitalRead(S8) == LOW && digitalRead(S9) == HIGH) && (CurrentPicturePack >= 8))


    //CurrentPicture = 0;
    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
  {
#if SIMPLEDEBUG
    Serial.println("STATE2: PACK IS EMPTY");
#endif

    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
    // KEEP IN MIND THAT THIS **THE CAMERA** SAYING IT HAS ALREADY MADE 10 SHOTS.
    // I COULD SET MY OWN COUNTER (UP TO 8) AND MAKE IT MORE IMPOSSIBLE-8-SHOTS-FRIENDLY
    Write_DS2408_PIO (7, 1);
    //digitalWrite(led2, HIGH);
    //      Serial.begin (9600);
    //      Serial.println ("Write f^*ng PIO");



    if ((digitalRead(S1) == S1Logic) && (switch2 ==  0))  // DUMP EEPROM INFO "NORMAL" Read_DS2408_PIO(2) ==  0

    {

      eepromDump ();

      //Serial.print("======================= After loop =======================");
      //Serial.print ("Read: ");
      //Serial.println (ReadAddress);
      //    delay (1000);

      //added return

    }
    return;

    //======================================================================================================
    // S1 = ON dump CSV and ask how many

    if ((digitalRead(S1) == S1Logic) && (switch2 ==  1))
    {
      eepromDumpCSV();

    }
    //======================================================================================================

    //added return
    return;

  }


  //STATE 3: NORMAL OPERATION *************************************************************************************************************************************************
  if (digitalRead(S8) == LOW && digitalRead(S9) == LOW) //S8 and S9 are OPEN
  {
    CurrentPicturePack = EEPROM.read(4);
#if SIMPLEDEBUG

#endif


    // ///////////////////////////////////PICTURE TAKING OPERATION//////////////////////////////////////////////////
    //    FOUR CASES:
    //   *   CASE 1 NORMAL OPERATION: FULL CYCLE
    //   *  SELECTOR = NORMAL (LOW)
    //   *  SHOTS = 0
    //   *  S1 = LOW (RED BUTTON PRESSED)
    //   *
    //   *  CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
    //   *  SELECTOR = DOUBLE (HIGH)
    //   *  SHOTS = 0
    //   *  S1 = LOW (RED BUTTON PRESSED)
    //   *
    //   *  CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
    //   *  SELECTOR = DOUBLE (HIGH)
    //   *  SHOTS >= 1
    //   *  S1 = LOW (RED BUTTON PRESSED)
    //   *  CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
    //   *
    //   *  SELECTOR = NORMAL (LOW)
    //   *  SHOTS >= 1

    //              takePicture= false;
    //              byte ActualSlot = (Read_DS2408_PIO(0));


    //              Serial.println (ShutterSpeed[Read_DS2408_PIO(0)]);

    //=================================================================================================================================================================

    if ((digitalRead(S1) == S1Logic) && ((ShutterSpeed[selector] == (POSB)))) //////////////POSITION B

    {

      PictureType = 7;
      eepromUpdate();
      ShutterB();

    }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))
    //=================================================================================================================================================================
    if ((digitalRead(S1) == S1Logic) && ((ShutterSpeed[selector] == (POST)))) //////////////POSITION T

    {
#if SIMPLEDEBUG
      Serial.println("POS T");
#endif
      PictureType = 8;
      eepromUpdate();
      ShutterT();

    }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POST))))


    //==================================================================================================================================================================


    takePicture = false;
    int RedButton = checkButton();
/////Serial.println (RedButton);
    PictureType = 0;

    if ((RedButton == 1) or (RedButton == 3))  //this is either one press and release or one long sustained press with no release (to "emulate" the original camera)
    {
      takePicture = true;
#if SIMPLEDEBUG
      Serial.println("takePicture = TRUE");
#endif
    } //else takePicture = false;


    if (RedButton == 2)                         //this the 2 short press that enable the self-timer delay. Can be disabled of course
    {
 //     Serial.println ("2");
#if !SONAR
////
Serial.println ("b");
      BlinkTimerDelay();  //Dongle LED blinks
      takePicture = true;
#if SIMPLEDEBUG
      Serial.println("takePicture = TRUE seft timer");
#endif
#endif
#if SONAR
    return;
#endif    
    }

#if SONAR
    while (S1F == S1Logic)
    int WaitForFocus();
    delay (100);
#endif

    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************
    // END STATE 3: NORMAL OPERATION CASE BOTH S8 AND S9 ARE OPEN*******************************************************************************************************************

    //            Serial.println (Read_DS2408_PIO(0));

    //=========================================================================================================================================================
    // CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
    //=========================================================================================================================================================

    if (takePicture == true && switch1 == 0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
    {
#if SIMPLEDEBUG
      Serial.println("last pict");
#endif

      eepromUpdate();
      shots = 0;
      Write_DS2408_PIO(6, 0);
      mirrorDOWN();
      delay(50);                             //AGAIN is this delay necessary? 100-->50
      shutterOPEN();
      return;

    }// end of if (takePicture == true && Read_DS2408_PIO(1) ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
    //=========================================================================================================================================================
    if (takePicture == true)
    {

      //   if (Read_DS2408_PIO(0) == 200)//DONGLELESS!!!! THIS CASE WILL BE AUTO AT THE cISO from EEPROM film speed


      if (selector == 200)//DONGLELESS!!!! THIS CASE WILL BE AUTO AT THE cISO from EEPROM film speed

      {
#if LIGHTMETER
        output_compare = ISO();
        Serial.print ("DL AUTO output_compare: ");
        Serial.println (output_compare);
        //byte PictureType = 6;
        eepromUpdate();
        AutoExposure();
        return;
#endif
#if !LIGHTMETER
        return;
#endif

      }


#if SIMPLEDEBUG
      Serial.print("                                    Selector: ");
      Serial.println(selector);
#endif

      if (Read_DS2408_PIO(0) == 100)  //THIS CASE ITS A FLASH PICTURE (FLASH INSERTED IN SX70, NO DONGLE)
      {
#if SIMPLEDEBUG
        Serial.println("FLASHBAR");
        //      Serial.print(Read_DS2408_PIO(0));
        //     Serial.println(":  FLASH");
#endif
        FlashBAR();
        return;
      }

      // OLD DONGLELESS POSITION


      if (selector < 100)  //THIS CASE WE HAVE A PROPER SHUTTER SPEED ON THE SELECTOR (WE HAVE A DONGLE)
      {

        if ((selector >= 0) && (selector < 12))
        {
          ////Serial.println ("MANUAL SELECTOR SPEED");
          ManualExposure();
          eepromUpdate();
          return;
        }

      }


      if ((ShutterSpeed[selector]) == AUTO600)
      {
        output_compare = A600;
 //       ISO = A600;
        byte PictureType = 6;
                Serial.print ("S-A600 output_compare: ");
        Serial.println (output_compare);


#if SIMPLEDEBUG
        Serial.println("SELECTOR AUTO600");
#endif
#if LIGHTMETER
        AutoExposure();
        eepromUpdate();
#endif
#if !LIGHTMETER
        return;
#endif
      }
      else if ((ShutterSpeed[selector]) == AUTO100)
      {
        output_compare = A100;
//        ISO = A100;
        byte PictureType = 1;
                Serial.print ("S-A600 output_compare: ");
        Serial.println (output_compare);


#if SIMPLEDEBUG
        Serial.println("SELECTOR AUTO100");
#endif
#if LIGHTMETER
        AutoExposure();
        eepromUpdate();
#endif
#if !LIGHTMETER
        return;
#endif
      }

    }

  }


} //END OF loop

//***************************************************************************************************************************************
