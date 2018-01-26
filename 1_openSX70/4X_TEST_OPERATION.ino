/*void loop()
{
/*  if ((digitalRead(S1) == LOW))  
                  {
                      Serial.print ("RED BUTTON PRESSED");
                                    
                  
                  shutterCLOSE (); 
                  
                  
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
                  delay (40);                               //S3 is now open start Y-delay (40ms)
                  
                  
                  delay (500);

                  shutterOPEN ();
                   
delay (500);                      
                        shutterCLOSE ();
                  
            
            
                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (200);                             //AGAIN is this delay necessary?
            
                 shutterOPEN();
            
              
}
mirrorUP();
delay (10000);
mirrorDOWN();
delay (10000);

            takePicture = false;
            int pressTime = REDbutton(S1);
            
  if ((pressTime > shortPress) && (pressTime < longPress)) {
                                       #if ISDEBUG 
                                      Serial.println("---------------------------");
                                      Serial.print ("SHORT:");
                                      Serial.println (pressTime);
                                      Serial.println("---------------------------");
                                      #endif  

            takePicture = true;
              
            }  // END OF if ((pressTime > shortPress) && (pressTime < longPress)) {
            
            else if (pressTime > longPress) {
              #if ISDEBUG 
                                      Serial.println("---------------------------");
                                      Serial.print ("LONG: ");
                                      Serial.println (pressTime);
                                      Serial.println("---------------------------");
                                      #endif  

            BeepTimerDelay();   
            takePicture = true;
            }
             
 if (takePicture == true)
 {
                  shutterCLOSE (); 
                  
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
                  delay (40);                               //S3 is now open start Y-delay (40ms)
  
 
                   Click (); 
                  
                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (200);                             //AGAIN is this delay necessary?
 
                 shutterOPEN();
                 shots = 0;  
     }

//START LOOP HERE FOR ROLLING TESTS

 Serial.println ("TEST SHUTTER ONLY");
 beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
delay (5000);
shutterCLOSE();
delay (1000);
shutterOPEN();
delay (66);
shutterCLOSE();
delay (500);
shutterOPEN();
                                       #if ISDEBUG 
                                      Serial.print ("Selector: ");
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(1));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(2));
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[Read_DS2408_PIO(0)]));
                                      #endif
delay (2000);

Serial.println ("TEST MOTOR ONLY");
 
beep (1,6);
 delay (500);
 beep (0,6);
 delay (100);
 beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
mirrorUP();
delay (1000);
mirrorDOWN();

delay (2000);
                                       #if ISDEBUG 
                                      Serial.print ("Selector: ");
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(1));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(2));
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[Read_DS2408_PIO(0)]));
                                      #endif
Serial.println ("TEST PICTURE-TAKING ONLY");
 
beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
 beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
 beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
shutterOPEN();
delay (2000);
shutterCLOSE();
delay (500);
mirrorUP();
delay (500);
shutterOPEN();
delay (66);
shutterCLOSE();
delay (100);
mirrorDOWN();
delay (100);
shutterOPEN();
                                       #if ISDEBUG 
                                      Serial.print ("Selector: ");
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(1));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(2));
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[Read_DS2408_PIO(0)]));
                                      #endif
delay (2000);

beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
 beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
 beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
 beep (1,6);
 delay (100);
 beep (0,6);
 delay (100);
                                       #if ISDEBUG 
                                      Serial.print ("Selector: ");
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(1));
                                      Serial.print (" / ");
                                      Serial.print (Read_DS2408_PIO(2));
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[Read_DS2408_PIO(0)]));
                                      #endif
 delay (2000);    
}


