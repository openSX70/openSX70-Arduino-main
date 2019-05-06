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
                  
                 // startCounterCalibration();

                  Click(0);
                  

               if (switch1 == 1)
                  {
                  shots = ++shots;
                  return;
                  } else if (switch1 == 0)
                  {
                    delay (200);                             //AGAIN is this delay necessary?
                    mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                    delay (60);                             //AGAIN is this delay necessary?
                    shutterOPEN();
                    shots = 0;  
                    return;   
                  }

                 return;  
}
