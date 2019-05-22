void loop() {

  int old_device_count = device_count;
  device_count = ds.findsingle(&dongleDevice);
  if (device_count != 0 && old_device_count == 0) {
    //OPTION this is only to blink the LED on the dongle if inserted late. Only the first time.
    initializeDS2408(); //Pitsie fix for non-blinking LED on dongle
    Write_DS2408_PIO (6, 1);
    delay (200);
    Write_DS2408_PIO (6, 0);
  }

  if (device_count != 0) {
    // READ DS2408

    selector = Read_DS2408_PIO(0);
    switch1 = Read_DS2408_PIO(1);
    switch2 = Read_DS2408_PIO(2);


  }

  int exposure;
  int ISO;

#if LIGHTMETER

#if VFled

  
  if (((ShutterSpeed[selector]) == AUTO600) || ((ShutterSpeed[selector]) == AUTO100) || (Read_DS2408_PIO(0) == 200))
  {
	  if ((ShutterSpeed[selector]) == AUTO100)
		  ISO = A100;
	  else
		  ISO = A600;
		
	  exposure = PredictedExposure(ISO);

#if SIMPLEDEBUG
		Serial.print(" Predicted Exposure: ");
		Serial.println(exposure);

		Serial.print("ShutterSpeed[selector]: ");
		Serial.println((ShutterSpeed[selector]));
	   

	        digitalWrite(led1, LOW);
	   	  Serial.println("AUTO MODE ON");
#endif
	  if (exposure >= ShutterSpeed[7])
	  {
			digitalWrite(led2, HIGH);
		  //		Serial.print(ISO);
		  //		Serial.println("  Low light!!!");
	  }
	  else
	  {
		  //		Serial.println("  else light!!!");

		  digitalWrite(led2, LOW);
		  digitalWrite(led1, LOW);
	  }
  }
  
  //int nearest(int x, int myArray[], int elements, bool sorted)

  if (switch2 == 1) {

	  if ((selector >= 0) && (selector < 12))
		  //ISO = A600;
		  exposure = PredictedExposure(A600);
	  {

	  int slot = nearest(exposure, ShutterSpeed, 11, 1);
	  /*
		Serial.print ("Slot/selector/Pred Exp: ");
		Serial.print (slot);
		Serial.print (" / ");
		Serial.print (selector);
		Serial.print(" / ");
		Serial.println(exposure);
	  	*/ 


	  if (selector < slot)
		  {
			  digitalWrite(led2, HIGH);
			  digitalWrite(led1, LOW);
		  }
		  else if (selector > slot)
		  {
			  digitalWrite(led1, HIGH);
			  digitalWrite(led2, LOW);
		  }
		  else if (selector == slot)
		  {
			
			  digitalWrite(led1, LOW);
			  digitalWrite(led2, LOW);
		  }


	  }
  }
  else
  {
	  digitalWrite(led1, LOW);
	  digitalWrite(led2, LOW);
  }
  
#endif
#endif



  /*
    long frequency = frequencyCounter();
    Serial.print ("frequency : ");
    Serial.println (frequency);
  */
#if SIMPLEDEBUG

  //DEBUG DS2408
  /*
    Serial.print ("selector: ");
    Serial.println (selector);
    Serial.print ("switch1: ");
    Serial.println (switch1);
    Serial.print ("switch2: ");
    Serial.println (switch2);
  */
#endif



  /*
    if (Read_DS2408_PIO(0) == 100)
    {
      Serial.println ("FLASH");
      return;
    }
    if (Read_DS2408_PIO(0) == 200)
    {
      Serial.println ("NOTHING");
      return;
    }

    if (Read_DS2408_PIO(0) < 100)
    {
      byte ActualSlot = Read_DS2408_PIO(0);
      Serial.print ("Selector: ");
      Serial.println (ActualSlot);

      byte S1 = Read_DS2408_PIO(1);
      Serial.print ("S1: ");
      Serial.println (S1);

      byte S2 = Read_DS2408_PIO(2);
      Serial.print ("S2: ");
      Serial.println (S2);
      return;
    } */
  /*
    if (selector == 15) {
    Serial.println ("workaround");
    uint8_t readDevice = ds.get_state(devices[0]);
    //workaround
    selector = Read_DS2408_PIO(0);
    switch1 = Read_DS2408_PIO(1);
    switch2 = Read_DS2408_PIO(2);
    }*/
  // READ DS2408
  //=======================================================================================




  //=======================================================================================


  //WHAT TO DO WHEN POWER-UP:
  //  S8     S9
  //closed  open  --> EJECT DARKSLIDE (DEFAULT)
  // open  closed --> FILM REACH 0 (NO FLASH)
  // open   open  --> NORMAL OPERATION 10 TO 1




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
    Write_DS2408_PIO (6, 1);
    //digitalWrite(led2, HIGH);
    //      Serial.begin (9600);
    //      Serial.println ("Write f^*ng PIO");



    if ((digitalRead(S1) == LOW) && (switch2 ==  0))  // DUMP EEPROM INFO "NORMAL" Read_DS2408_PIO(2) ==  0

    {

      eepromDump ();

      //Serial.print("======================= After loop =======================");
      //Serial.print ("Read: ");
      //Serial.println (ReadAddress);
      //    delay (1000);

      //added return
      //  return;
    }

    //======================================================================================================
    // S1 = ON dump CSV and ask how many

    if ((digitalRead(S1) == LOW) && (switch2 ==  1))
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
    CurrentPicturePack = EEPROM.read(4) ;
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

    if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[selector] == (POSB)))) //////////////POSITION B

    {

      PictureType = 7;
      eepromUpdate ();
      ShutterB();

    }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))
    //=================================================================================================================================================================
    if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[selector] == (POST)))) //////////////POSITION T

    {
#if SIMPLEDEBUG
      Serial.println ("POS T");
#endif
      PictureType = 8;
      eepromUpdate ();
      ShutterT();

    }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POST))))


    //==================================================================================================================================================================


    takePicture = false;
    int RedButton = checkButton();

    PictureType = 0;

    if ((RedButton == 1) or (RedButton == 3) )  //this is either one press and release or one long sustained press with no release (to "emulate" the original camera)
    {
      takePicture = true;
#if SIMPLEDEBUG
      Serial.println ("takePicture = TRUE");
#endif
    } //else takePicture = false;


    if (RedButton == 2)                         //this the 2 short press that enable the self-timer delay. Can be disabled of course
    {
      BlinkTimerDelay();  //Dongle LED blinks
      takePicture = true;
#if SIMPLEDEBUG
      Serial.println ("takePicture = TRUE seft timer");
#endif

    }

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

    if (takePicture == true && switch1 ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
    {
#if SIMPLEDEBUG
      Serial.println ("last pict");
#endif

      eepromUpdate ();
      shots = 0;
      Write_DS2408_PIO (6, 0);
      mirrorDOWN ();
      delay (50);                             //AGAIN is this delay necessary? 100-->50
      shutterOPEN();
      return;

    }// end of if (takePicture == true && Read_DS2408_PIO(1) ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
    //=========================================================================================================================================================
    if (takePicture == true)

    {
#if SIMPLEDEBUG
      Serial.print ("                                    Selector: ");
      Serial.println (selector);
#endif

      if (Read_DS2408_PIO(0) == 100)  //THIS CASE ITS A FLASH PICTURE (FLASH INSERTED IN SX70, NO DONGLE)
      {
#if SIMPLEDEBUG
        Serial.println ("FLASHBAR");
        Serial.print (Read_DS2408_PIO(0));
        Serial.println (":  FLASH");
#endif
        FlashBAR();
        return;
      }

      //      if (selector == 200)  //DONGLELESS!!!! THIS CASE WILL BE AUTO PROBABLY AT 600ASA



      if (Read_DS2408_PIO(0) == 200)
      {


        output_compare = A600;
        byte PictureType = 6;
        eepromUpdate ();
#if SIMPLEDEBUG
        Serial.println ("DONGLELESS AUTO600");
#endif
        shots = 0;
        AutoExposure();
        //        return;
        return;
#if SIMPLEDEBUG
        Serial.println ("200");
        Serial.println ("DONGLELESS AUTO");
#endif

      }

      if (selector < 100)  //THIS CASE WE HAVE A PROPER SHUTTER SPEED ON THE SELECTOR (WE HAVE A DONGLE)
      {
        //Serial.println ("MANUAL SELECTOR SPEED");

        if ((ShutterSpeed[selector]) == AUTO600)
        {
          output_compare = A600;
          byte PictureType = 6;
          eepromUpdate ();
#if SIMPLEDEBUG
          Serial.println ("SELECTOR AUTO600");
#endif
          AutoExposure();
          return;
        }
        else if  ((ShutterSpeed[selector]) == AUTO100)
        {
          output_compare = A100;
          byte PictureType = 1;
          eepromUpdate ();
#if SIMPLEDEBUG
          Serial.println ("SELECTOR AUTO100");
#endif
          AutoExposure();
          return;
        }
        //                            Serial.print ("                           Read_DS2408_PIO NOW selector = ");
        //                            Serial.println (selector);
#if SIMPLEDEBUG
        Serial.println ("MANUAL SELECTOR SPEED");
#endif
        ManualExposure();
        eepromUpdate();
        return;
      }

    }
  }

} //END OF loop

//***************************************************************************************************************************************
