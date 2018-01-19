void Dongle(int DongleSlot) 
    {
                                        #if ISDEBUG 
                                        Serial.print ("                                                 Dongle:");
                                        Serial.println (DongleSlot);
                                        #endif

    
    if (takePicture == true  && DS2408(1) ==  0 && shots == 0)    //NORMAL OPERATION
    {
 
                  #if SHUTTER
                  shutterCLOSE (); 
                  #endif
                  
                  #if MOTOR 
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
                  delay (40);                               //S3 is now open start Y-delay (40ms)
                  #endif
                      pinMode(13,OUTPUT);
  
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
                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (200);                             //AGAIN is this delay necessary?
                  #endif

                 #if SHUTTER
                 shutterOPEN();
                 #endif   
                 shots = 0;  
                 return;             
     } // END of  if (takePicture == true  && DS2408(1) ==  0 && shots == 0)    //NORMAL OPERATION
    
                   
     // CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)           
     if (takePicture == true && DS2408(1) ==  1 && shots == 0)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) FIRST SHOT!!!!
      {
 
          #if SHUTTER
          shutterCLOSE ();  // I CLOSE THE SHUTTER
          #endif
          
          #if MOTOR                                            
          mirrorUP();                                 //Motor Starts: MIRROR COMES UP!!!

          while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
            ;
          delay (40);                               //S3 is now open start Y-delay (40ms)
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

          } // end of  if (takePicture == true && DS2408(1) ==  1 && shots == 0)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) FIRST SHOT!!!!

        
         // CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED) 

          
         if (takePicture == true && DS2408(1) ==  1 && shots >= 1)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) ULTERIOR SHOTS!!!!
          {
 
          #if SHUTTER    
          Click();
          
          delay (1000); //debounce will go here
          
          shots++;
          #endif
          return;
          
          } // end of  if (digitalRead(S1) == LOW && DS2408(1) ==  1 && shots >= 1)    //DOUBLE EXPOSURE and RED BUTTON PRESSED (S1) ULTERIOR SHOTS!!!!

          
          
          // CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
         if (takePicture == true && DS2408(1) ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
          {
        
            shots = 0;
 
             #if MOTOR
            mirrorDOWN ();

            delay (200);                             //AGAIN is this delay necessary?
            #endif
                  #if !MOTOR
                  delay (500);
                  #endif

            #if SHUTTER
            shutterOPEN();    
            #endif
            return;
            
           }// end of if (takePicture == true && DS2408(1) ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL

    }
     
    
 
