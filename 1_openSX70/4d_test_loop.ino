void loop() {
  
              takePicture= false;                                  
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
         byte ActualSlot = (TimeSlot(0));
                                      
          if (TimeSlot(0) < 100)
                              {
          //byte ActualSlot = (TimeSlot(0));
                                      #if ISDEBUG 
                                      Serial.print ("Selector: ");
                                      Serial.println (ActualSlot);
                                      #endif



          Dongle (ActualSlot);
/*
    byte S1 = TimeSlot(1);
                                      #if ISDEBUG 
                                      Serial.print ("------------------------>S1: ");
                                      Serial.println (S1);
                                      #endif

    byte S2 = TimeSlot(2);
                                      #if ISDEBUG 
                                      Serial.print ("------------------------>S2: ");
                                      Serial.println (S2);
                                      #endif
    return;
  }
  if (TimeSlot(0) == 100)
  {
                                      #if ISDEBUG 
                                      Serial.println ("FLASH");
                                      #endif
    return;
  }
  if (TimeSlot(0) == 200)
  {
                                    #if ISDEBUG 
                                    Serial.println ("NOTHING");
                                    #endif
    return;
  }
  else
  {
                                  #if ISDEBUG 
                                  Serial.println ("DONT KNOW ");
                                  #endif
                                  */
  }

}

