/*
 void loop() {
  
              takePicture= false;                                  
              byte ActualSlot = (Read_DS2408_PIO(0));
              
              if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))
                  {
                      Serial.print ("POSITION B:  ");
                      Serial.println( ActualSlot);
                                    
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
                   
                        while (digitalRead(S1) == LOW)
                        ;
                        
                        shutterCLOSE ();
                  #endif
                  
                  #if !MOTOR
                  delay (500);
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
                 //shots = 0;  
 
                        

                 return;       
                  
               }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))

  
            int pressTime = REDbutton(S1);
        
       if ((ShutterSpeed[ActualSlot]) != (POSB))
             {
              
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
             
            }   // END Of else if (pressTime > longPress) 
             }
                                      
          if (Read_DS2408_PIO(0) < 100)
                              {
          //byte ActualSlot = (Read_DS2408_PIO(0));
                                      #if ISDEBUG 
                                      byte S1 = Read_DS2408_PIO(1);
                                      byte S2 = Read_DS2408_PIO(2);
                                      Serial.print ("Selector: ");
                                      Serial.print (ActualSlot);
                                      Serial.print (" / ");
                                      Serial.print (S1);
                                      Serial.print (" / ");
                                      Serial.print (S2);
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[Read_DS2408_PIO(0)]));
                                      #endif



          Dongle (ActualSlot);
          return;
                              }

  if (Read_DS2408_PIO(0) == 100)
  {
                                      #if ISDEBUG 
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.println (":  FLASH");
                                      #endif
                                      Flash();
    return;
  }
  if (Read_DS2408_PIO(0) == 200)
  {
                                    #if ISDEBUG 
                                    Serial.print (Read_DS2408_PIO(0));
                                    Serial.println (": NOTHING: WILL USE LIGHT METER?");
                                    #endif
    return;
  }
  else
  {
                                  #if ISDEBUG 
                                  Serial.print (Read_DS2408_PIO(0));
                                  Serial.println (":   DONT KNOW, SHOULD NOT BE HERE! ");
                                  #endif
  }
                              
}
*/
