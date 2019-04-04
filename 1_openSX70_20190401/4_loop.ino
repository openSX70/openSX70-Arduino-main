
void loop() {
//takePicture = false;
if (device_count == 0)
{
device_count = ds.find(&devices);
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
  
  //WHAT TO DO WHEN POWER-UP:
  //  S8     S9
  //closed  open  --> EJECT DARKSLIDE (DEFAULT)
  // open  closed --> FILM REACH 0 (NO FLASH)
  // open   open  --> NORMAL OPERATION 10 TO 1

 
   // STATE 1: EJECT DARKSLIDE:*************************************************************************************************************************************************
  #if MOTOR 
  if (digitalRead(S8) == HIGH && digitalRead(S9) == LOW)
    //EJECT DARK SLIDE
  {
    CurrentPicturePack = 0;
    EEPROM.write(4,CurrentPicturePack);
    Write_DS2408_PIO (6, 0);
            Write_DS2408_PIO (6, 1);
     darkslideEJECT();
            Write_DS2408_PIO (6, 0);
            
                                   #if ISDEBUG 
                                  Serial.println("STATE1: EJECT DARK SLIDE");      
                                  #endif
  }
  //  CurrentPicture = EEPROM.read(4) ; 
  #endif
  #if MOTOR 
  //STATE 2: PACK IS EMPTY--> NO WASTE OF FLASH *********************************************************************************************************************************
//    if ((digitalRead(S8) == LOW && digitalRead(S9) == HIGH) || (CurrentPicture >= 8))

// changed this to allow shooting until counter is actually 0, in case "something" happens and I loose count!
    
    if ((digitalRead(S8) == LOW && digitalRead(S9) == HIGH) && (CurrentPicturePack >= 8))
 
    
    //CurrentPicture = 0;
    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
     {
                                   #if ISDEBUG 
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
#endif

  //STATE 3: NORMAL OPERATION *************************************************************************************************************************************************
  if (digitalRead(S8) == LOW && digitalRead(S9) == LOW)
  {
                                 CurrentPicturePack = EEPROM.read(4) ; 
                                 #if ISDEBUG 
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

                 //Serial.println ("POS T");
                  PictureType = 8;
                  eepromUpdate ();                
                  ShutterT();
                  
               }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POST))))

  
  //==================================================================================================================================================================                

//            int pressTime = REDbutton(S1);

       takePicture = false;
      int RedButton = checkButton();
       
//       if ((ShutterSpeed[Read_DS2408_PIO(0)]) != (POSB))
// //       if (ShutterSpeed[Read_DS2408_PIO(0)])
  
 // //            {
              PictureType = 0;

/*              
            if ((pressTime > shortPress) && (pressTime < longPress)) {
                                       #if ISDEBUG 
                                      Serial.println("---------------------------");
                                      Serial.print ("SHORT:");
                                      Serial.println (pressTime);
                                      Serial.println("---------------------------");
                                      #endif  
*/
            if ((RedButton == 1) or (RedButton == 3) )
            {
             takePicture = true;
            } //else takePicture = false;
            

            if (RedButton == 2) 
            {
            BlinkTimerDelay();  //Dongle LED blinks
            takePicture = true;

            }
            
//            }  // END OF if ((pressTime > shortPress) && (pressTime < longPress)) {
            
          
/*            else if (pressTime > longPress) {
                                       #if ISDEBUG 
                                      Serial.println("---------------------------");
                                      Serial.print ("LONG: ");
                                      Serial.println (pressTime);
                                      Serial.println("---------------------------");
                                      #endif  
                                        
//            BeepTimerDelay();   //Piezo beeps
          BlinkTimerDelay();  //Dongle LED blinks
//          LEDTimerDelay();    //Built-in LED blinks          
            takePicture = true;
             
            }   // END Of else if (pressTime > longPress) */
 // //            }
                                      
                            if (Read_DS2408_PIO(0) < 100)  //THIS CASE WE HAVE A PROPER SHUTTER SPEED
                              {
                                if ((ShutterSpeed[Read_DS2408_PIO(0)] == (POSFLASH)) && (Read_DS2408_PIO(2) ==  1))
                              {
                                DongleFlashNormal();
                                return;
                              
                              } else if ((ShutterSpeed[Read_DS2408_PIO(0)] == (POSFLASH)) && (Read_DS2408_PIO(2) ==  0))
                              {
                                DongleFlashF8();
                                return;
                                                              
                              }
                            if ((ShutterSpeed[Read_DS2408_PIO(0)] == (AUTO100)))
                            {
                              Auto100();
                              return;
                            }
          //byte ActualSlot = (Read_DS2408_PIO(0));

                                      #if ISDEBUG 
                                      byte SD1 = Read_DS2408_PIO(1);
                                      byte SD2 = Read_DS2408_PIO(2);
                                      Serial.print ("Selector: ");
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.print (" / ");
                                      Serial.print (SD1);
                                      Serial.print (" / ");
                                      Serial.print (SD2);
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[Read_DS2408_PIO(0)]));
                                      //delay(500);
                                      #endif



//          Dongle (Read_DS2408_PIO(0));
            Dongle (0);   //MANUAL MODE
          return;
                              }

  if (Read_DS2408_PIO(0) == 100)  //THIS CASE ITS A FLASH PICTURE (FLASH INSERTED IN SX70, NO DONGLE)
  {
   
                                      #if ISDEBUG 
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.println (":  FLASH");
                                      #endif
                                      BuiltInFlash();
    return;
  
  }
  if (Read_DS2408_PIO(0) == 200)  //THIS CASE WILL BE AUTO PROBABLY AT 600ASA
  {
   /*                                 #if ISDEBUG 
                                    Serial.print (Read_DS2408_PIO(0));
                                    #endif
                                    #if LIGHTMETER
                                    //here goes light metering
                                      #endif

                                       if (takePicture == true )    //NORMAL AUTO OPERATION
    {
                         
                    byte PictureType = 6;                    
                    eepromUpdate ();
    
                  shutterCLOSE (); 
                  // delay (200); //added to fix bad photos
                  delay (100); //added to fix bad photos WITH LESS delay
                                    
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
//                  Ydelay();
                    delay(40); //Just "normal" "Y" delay

                  shutterOPEN();
                  // Click (time); 
  //                delay (time);  // this set by lux! NEW
                  shutterCLOSE();
//                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (50);                             //AGAIN is this delay necessary?
                  //CHANGED 200->50
                 shutterOPEN();
                 shots = shots++;  

                 return;  
    
     } // END of  if (takePicture == true  && Read_DS2408_PIO(1) ==  0 && shots == 0)    //NORMAL OPERATION
//    #endif

           
                                      
                                    
//if (takePicture == true  && Read_DS2408_PIO(1) ==  0 && Read_DS2408_PIO(0) ==  200 && shots == 0)    //NORMAL AUTO OPERATION
 
                                    
                                      
    return;
*/  }
  else
  {
                                  // re check if dongle has been re-connected!

                                  //#if ISDEBUG 
                                  Serial.print (Read_DS2408_PIO(0));
                                  Serial.println (":   DONT KNOW, SHOULD NOT BE HERE! ");
                                  //#endif
  }


  }
} //END OF loop      

//***************************************************************************************************************************************
 
