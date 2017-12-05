void Flash () 
    {

       if (takePicture == true)
       {
      Serial.println ("FLASH FUNCTION!");
            #if SHUTTER
                   HighSpeedPWM ();
                  analogWrite(Solenoid2, 255);
                  shutterCLOSE (); 
                  #endif
                  
                  #if MOTOR 
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
                  analogWrite (Solenoid2, 130);
                  delay (40);                               //S3 is now open start Y-delay (40ms)
                  #endif
 
                  #if !MOTOR
                  delay (500);
                  #endif

                  #if SHUTTER
                  shutterOPEN (); 
                  delay (51);
                  digitalWrite(FFA, HIGH);
                  delay (25);
                  analogWrite (Solenoid1,0);
                  digitalWrite(FFA, LOW);
                  delay (26);
                  shutterCLOSE();
                  #endif
                  
                  #if !MOTOR
                  delay (500);
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
       }
    }

