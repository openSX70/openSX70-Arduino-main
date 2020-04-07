void ManualExposure()
{
                          #if SIMPLEDEBUG
                          Serial.println ("take Manual picture");
                          #endif
                    byte PictureType = 0;                    
//                    eepromUpdate ();
    
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
                  Click (0);  
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
                  eepromUpdate ();
                 shots = 0;  

                 return;  
}
