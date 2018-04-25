void Dongle(int DongleSlot) 
    {
                                        #if ISDEBUG 
                                        Serial.print ("                                                 Dongle:");
                                        Serial.println (DongleSlot);
                                        #endif


//=========================================================================================================================================================                   
     // CASE 0 FORCED EJECTION TO SAFEGUARD SOLENOID#1 IN MULTIPLE EXPOSURES AFTER 1 MINUTE (60000ms)       
//=========================================================================================================================================================

unsigned long currentMillisTimer = millis();
           
           if ((currentMillisTimer-DoubleExposureTimer) >= 60000 && shots >= 1)
         
         {
           eepromUpdate ();

            shots = 0;
 
             #if MOTOR
            mirrorDOWN ();

            delay (50);                             //100-->50 AGAIN is this delay necessary?
            #endif
                  #if !MOTOR
                  delay (500);
                  #endif

            #if SHUTTER
            shutterOPEN();    
           #endif
           Write_DS2408_PIO (6, 0); 
         }  else if ((currentMillisTimer-DoubleExposureTimer) >= 30000 && shots >= 1)

         {
          Write_DS2408_PIO (6, 1); 
          
         } 


//=========================================================================================================================================================                   
     // CASE 1 NORMAL OPERATION NORMAL OPERATION NORMAL OPERATION NORMAL OPERATION NORMAL OPERATION NORMAL OPERATION NORMAL OPERATION NORMAL OPERATION            
//=========================================================================================================================================================

    
    if (takePicture == true  && Read_DS2408_PIO(1) ==  0 && shots == 0)    //NORMAL OPERATION
    {
                          Serial.println ("take picture");

                    byte PictureType = 0;                    
                    eepromUpdate ();
    
                  #if SHUTTER
                  shutterCLOSE (); 
                  #endif

                  #if MOTOR
//                  delay (200); //added to fix bad photos
                  delay (100); //added to fix bad photos WITH LESS delay
                                    
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
                  Ydelay();
                  #endif
                  
                  #if !MOTOR
                  delay (500);
                  #endif

                  #if SHUTTER
                   Click (); 
                  #endif
                  
                  #if !MOTOR
                  delay (1000);
                  #endif

                  #if MOTOR
//                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (50);                             //AGAIN is this delay necessary?
                  //CHANGED 200->50
                  #endif

                 #if SHUTTER
                 shutterOPEN();
                 #endif   
                 shots = 0;  

                 return;  
    
     } // END of  if (takePicture == true  && Read_DS2408_PIO(1) ==  0 && shots == 0)    //NORMAL OPERATION
    
//=========================================================================================================================================================                   
     // CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)           
//=========================================================================================================================================================

    
     if (takePicture == true && Read_DS2408_PIO(1) ==  1 && shots == 0)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) FIRST SHOT!!!!
      {   
/*
     EEPROM.get (13,ActualPicture);
          //CurrentPicture = (CurrentPicture+1);
*/    
          byte PictureType = 0;
         
          DoubleExposureTimer = millis();
    
          #if SHUTTER
          shutterCLOSE ();  // I CLOSE THE SHUTTER
          #endif
          
          #if MOTOR                                            
          mirrorUP();                                 //Motor Starts: MIRROR COMES UP!!!

          while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
            ;
          Ydelay();                         //S3 is now open start Y-delay (40ms)
          #endif
          #if !MOTOR
          delay (500);
          #endif
                  
          #if SHUTTER
          Click ();                 // NOW I am going to take the picture: SMILE!

          delay (500);            //debounce will go here
          shots++;
          #endif
          return;

          } // end of  if (takePicture == true && Read_DS2408_PIO(1) ==  1 && shots == 0)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) FIRST SHOT!!!!

//=========================================================================================================================================================        
         // CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED) 
//=========================================================================================================================================================


         
         if (takePicture == true && Read_DS2408_PIO(1) ==  1 && shots >= 1)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) ULTERIOR SHOTS!!!!
          {
 
          #if SHUTTER    
          Click();

          delay (500); //debounce will go here
          
          shots++;

          #endif
          return;
          
          } // end of  if (digitalRead(S1) == LOW && Read_DS2408_PIO(1) ==  1 && shots >= 1)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) ULTERIOR SHOTS!!!!

          
//=========================================================================================================================================================          
          // CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
//=========================================================================================================================================================
          
         if (takePicture == true && Read_DS2408_PIO(1) ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
          {
            
            eepromUpdate ();

            shots = 0;

            Write_DS2408_PIO (6, 0);
 
             #if MOTOR
            mirrorDOWN ();

            delay (50);                             //AGAIN is this delay necessary? 100-->50
            #endif
                  #if !MOTOR
                  delay (500);
                  #endif

            #if SHUTTER
            shutterOPEN();    
            #endif

            return;
            
           }// end of if (takePicture == true && Read_DS2408_PIO(1) ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL

    }
     
    
 
