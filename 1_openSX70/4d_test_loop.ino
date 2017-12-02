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
         byte ActualSlot = (DS2408(0));
                                      
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
                                      Serial.println (S2);
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

