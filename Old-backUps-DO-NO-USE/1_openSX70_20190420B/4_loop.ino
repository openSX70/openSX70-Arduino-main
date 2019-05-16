
void loop() {
//takePicture = false;

//initializeDS2408();
device_count = ds.find(&devices);
/*
uint8_t prev_count = ds.find(&devices); //this is an F****ing workaround to read the DS2408 count.
if  ((prev_count || device_count) == 1) 
{
  device_count = 1;
} else  

{
  device_count = 0;
}

*/
Serial.print("---------------->");
//Serial.println(prev_count);
Serial.println(device_count);


//=======================================================================================
//OPTION this is only to blink the LED on the dongle if inserted late. Only the first time.
if (device_count == 0)
{
#if SIMPLEDEBUG
Serial.print ("LOOP: device_count = ds.find(&devices);--->");
Serial.println (device_count);
#endif
          Write_DS2408_PIO (6, 1);
          delay (200);
         Write_DS2408_PIO (6, 0);
#if SIMPLEDEBUG
Serial.println ("BLINK after DS2408 init in loop");
#endif
}
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
    EEPROM.write(4,CurrentPicturePack);
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
        digitalWrite(led2, HIGH);
//      Serial.begin (9600);
//      Serial.println ("Write f^*ng PIO");



if ((digitalRead(S1) == LOW) && (Read_DS2408_PIO(2) ==  0))  // DUMP EEPROM INFO "NORMAL" Read_DS2408_PIO(2) ==  0
  
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

if ((digitalRead(S1) == LOW) && (Read_DS2408_PIO(2) ==  1))
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
                                Serial.println("STATE3: NORMAL OPERATION (BIG LOOP)");      
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
              
             if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[Read_DS2408_PIO(0)] == (POSB)))) //////////////POSITION B

                  {
                    
                 //Serial.println ("POS B");
                  PictureType = 7;
                  eepromUpdate ();
                  ShutterB();
                  
               }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))
//=================================================================================================================================================================
             if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[Read_DS2408_PIO(0)] == (POST)))) //////////////POSITION T
  
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
             Serial.println ("takePicture = TRUE");
            } //else takePicture = false;
            

            if (RedButton == 2)                         //this the 2 short press that enable the self-timer delay. Can be disabled of course
            {
            BlinkTimerDelay();  //Dongle LED blinks
            takePicture = true;
            Serial.println ("takePicture = TRUE seft timer");

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

            if (takePicture == true)
                            {
                            if (Read_DS2408_PIO(0) < 100)  //THIS CASE WE HAVE A PROPER SHUTTER SPEED
                              {
                                Serial.println ("MANUAL SPEED");
                                return;
                                //Dongle(0);
                              }


                            if (Read_DS2408_PIO(0) == 100)  //THIS CASE ITS A FLASH PICTURE (FLASH INSERTED IN SX70, NO DONGLE)
                                  {
                                Serial.println ("FLASHBAR");
                                
                                  #if SIMPLEDEBUG 
                                  //Serial.print (Read_DS2408_PIO(0));
                                  Serial.println (":  FLASH");
                                  #endif
                                //  FlashBAR();
                                  return;
                                   }
  
                             if (Read_DS2408_PIO(0) == 200)  //THIS CASE WILL BE AUTO PROBABLY AT 600ASA
                                   {
                                  Serial.println ("200");
                                  Serial.println ("DONGLELESS AUTO");
                                  return;
                                   }

                            } else 
                            return;
                                   
  }
  /*
                                  #if LIGHTMETER
                                    //here goes light metering
                                    output_compare = 200; //OPTION magic number for ISO 600
                                   Serial.println ("A6DL");
                                      AutoExposure();
                                      byte PictureType = 6;                    
                                      eepromUpdate ();
                                      shots = shots++;  
                                     #endif
                                                                        Serial.println ("A6DL");
                                     return;
                                   }*/
                                     
/* ////////////////
            if (takePicture == true )    //NORMAL AUTO OPERATION
                  {
                  void AutoExposure();                       
                  byte PictureType = 6;                    
                  eepromUpdate ();
                  shots = shots++;  
                  #endif
                 return;  
                                          
    return;
 }*/
 /*
                                    else
                                  {
                                  // re check if dongle has been re-connected!
                                  #if SIMPLEDEBUG 
                                  Serial.print (Read_DS2408_PIO(0));
                                  Serial.println (":   DONT KNOW, SHOULD NOT BE HERE! ");
                                  #endif
                                  return;
                                  }
                                  return;
 
 */
} //END OF loop      

//***************************************************************************************************************************************
 
