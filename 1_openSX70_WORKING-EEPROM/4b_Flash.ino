void Flash () 
    {

 
       if (takePicture == true)
      /* {
        
                  digitalWrite(FFA, HIGH);
                  Serial.println ("FFA, HIGH");
                  delay (45);
   //               analogWrite (Solenoid2,0);
                  digitalWrite(FFA, LOW);
                  Serial.println ("FFA, LOW");

       }*/
       {
 
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
                  Ydelay ();                               //S3 is now open start Y-delay (40ms)
                  #endif
 
                  #if !MOTOR
                  delay (500);
                  #endif

                  #if SHUTTER
                  shutterOPEN (); 
//                  delay (66);
                  delay (80);
                  digitalWrite(FFA, HIGH);
                  delay (15);
                  analogWrite (Solenoid2,0);
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

void DongleFlash () 
    {

 
       if (takePicture == true)
      /* {
        
                  digitalWrite(FFA, HIGH);
                  Serial.println ("FFA, HIGH");
                  delay (45);
   //               analogWrite (Solenoid2,0);
                  digitalWrite(FFA, LOW);
                  Serial.println ("FFA, LOW");

       }*/
       {
 
             #if SHUTTER
                   HighSpeedPWM ();
     //             analogWrite(Solenoid2, 255);
                  shutterCLOSE (); 
                  #endif
                  
                  #if MOTOR 
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
      //           analogWrite (Solenoid2, 130);
                  Ydelay ();                               //S3 is now open start Y-delay (40ms)
                  #endif
 
                  #if !MOTOR
                  delay (500);
                  #endif

                  #if SHUTTER
                  shutterOPEN (); 
//                  delay (66);
                  delay (80);
                  Write_DS2408_PIO (7,1);
//                  digitalWrite(FFA, HIGH);
                  delay (15);
//                  analogWrite (Solenoid2,0);
//                  digitalWrite(FFA, LOW);
                  Write_DS2408_PIO (7,0);
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

