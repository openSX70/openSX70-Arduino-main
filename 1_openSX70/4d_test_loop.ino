void loop() {
  
              takePicture= false;                                  
              byte ActualSlot = (DS2408(0));
              /*
              if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))
              {
                Serial.println ("POSITION B");
              }
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
                  if (ShutterSpeed[ActualSlot] == POSB)
                       { 
                        while (digitalRead(S1) == LOW)
                        ;
                        shutterCLOSE (); Click (); 
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
              return;
              };*/ 
             /*
              if (ShutterSpeed[ActualSlot] == POST)
               {
               if (digitalRead(S1) == HIGH)                         //THIS IS THE T "TIME" OPTION PRESS ONCE S1 (Red SHUTTER button) the shutter is OPEN and AGAIN to close
              {
              shutterCLOSE ();
              }
              return;
              } ; 
              }// END OF if ((digitalRead(S1 == LOW))  && ((ShutterSpeed[ActualSlot] == (POSB)) || (ShutterSpeed[ActualSlot] ==(POST))))
              */
              
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

            timerDelay();   
            takePicture = true;

            }   // END Of else if (pressTime > longPress) 
 
                                      
          if (DS2408(0) < 100)
                              {
          //byte ActualSlot = (DS2408(0));
                                      #if ISDEBUG 
                                      byte S1 = DS2408(1);
                                      byte S2 = DS2408(2);
                                      Serial.print ("Selector: ");
                                      Serial.print (ActualSlot);
                                      Serial.print (" / ");
                                      Serial.print (S1);
                                      Serial.print (" / ");
                                      Serial.print (S2);
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[DS2408(0)]));
                                      #endif



          Dongle (ActualSlot);
          return;
                              }

  if (DS2408(0) == 100)
  {
                                      #if ISDEBUG 
                                      Serial.print (DS2408(0));
                                      Serial.println (":  FLASH");
                                      #endif
                                      Flash();
    return;
  }
  if (DS2408(0) == 200)
  {
                                    #if ISDEBUG 
                                    Serial.print (DS2408(0));
                                    Serial.println (": NOTHING: WILL USE LIGHT METER?");
                                    #endif
    return;
  }
  else
  {
                                  #if ISDEBUG 
                                  Serial.print (DS2408(0));
                                  Serial.println (":   DONT KNOW, SHOULD NOT BE HERE! ");
                                  #endif
  }
                              
}

