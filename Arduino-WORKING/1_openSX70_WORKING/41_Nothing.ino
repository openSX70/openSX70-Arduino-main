/*     if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POST))))
              {
                Serial.println ("POSITION T");
              
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
 
                  #if !MOTOR
                  delay (500);
                  #endif

                  #if SHUTTER
                  shutterOPEN ();
                  //delay (1000); //minimun 1 second
                        
                        
                                       
                                     if  (digitalRead(S1) != LOW )                        //THIS IS THE T "TIME" OPTION PRESS ONCE S1 (Red SHUTTER button) the shutter is OPEN and AGAIN to close
                                      {
                                      ;   // DO NOTHING
                                      shutterCLOSE ();
                                      }
                                      
                                      
                        
                        
                   #endif
                  
                  #if !MOTOR
                  delay (500);
                                  Serial.println ("POSITION T delay");
                  #endif

                  #if MOTOR
                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (200);                             //AGAIN is this delay necessary?
                  #endif
 
                  #if !MOTOR
                  delay (1000);
                  #endif

                 #if SHUTTER
                 shutterOPEN();
                 #endif   
                 //shots = 0;  
                 };
              //return;
              }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POST))))
              */
