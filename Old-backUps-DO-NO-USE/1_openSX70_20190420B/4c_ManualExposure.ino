void ManualExposure()
{
                          #if SIMPLEDEBUG
                          Serial.println ("take Manual picture");
                          #endif
                    byte PictureType = 0;                    
//                    eepromUpdate ();
    
                  shutterCLOSE (); 

//                  delay (200); //added to fix bad photos
                  delay (100); //added to fix bad photos WITH LESS delay
                                    
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
                  Ydelay();
                  

                  Click (0);  
                  

//                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (50);                             //AGAIN is this delay necessary?
                  //CHANGED 200->50

                 shutterOPEN();
                  eepromUpdate ();
                 shots = 0;  

                 return;  
}
