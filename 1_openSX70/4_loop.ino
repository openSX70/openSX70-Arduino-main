/*
void loop() {
 
  //running code from 3d_test_loop
  
  //WHAT TO DO WHEN POWER-UP:
  //  S8     S9
  //closed  open  --> EJECT DARKSLIDE (DEFAULT)
  // open  closed --> FILM REACH 0 (NO FLASH)
  // open   open  --> NORMAL OPERATION 10 TO 1

 
   // STATE 1: EJECT DARKSLIDE:
  #if MOTOR 
  if (digitalRead(S8) == HIGH && digitalRead(S9) == LOW)
    //EJECT DARK SLIDE
  {
     darkslideEJECT();
                                   #if ISDEBUG 
                                  Serial.println("STATE1: EJECT DARK SLIDE");      
                                  #endif
  }
  #endif
  #if MOTOR 
  //STATE 2: PACK IS EMPTY--> NO WASTE OF FLASH
  if (digitalRead(S8) == LOW && digitalRead(S9) == HIGH)
    // FOR THE MOMENT I JUST TURN ON THE GREEN LED
  {
                                   #if ISDEBUG 
                                  Serial.println("STATE2: PACK IS EMPTY");      
                                  #endif

}
#endif
#if MOTOR 
  //STATE 3: NORMAL OPERATION
  if (digitalRead(S8) == LOW && digitalRead(S9) == LOW)
  {
                                 #if ISDEBUG 
                                Serial.println("STATE3: NORMAL OPERATION (BIG LOOP)");      
                                #endif
    
#endif
   
// ///////////////////////////////////PICTURE TAKING OPERATION//////////////////////////////////////////////////
//    FOUR CASES:
//   *   CASE 1 NORMAL OPERATION: FULL CYCLE
//   *  SELECTOR = NORMAL (LOW)
//   *  SHOTS = 0
//   *  S1 = LOW (RED BUTTON PRESSED)
//   *  
//   *  CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
//   *  SELECTOR = DOUBLE (HIGH)
//   *  SHOTS = 0
//   *  S1 = LOW (RED BUTTON PRESSED)
//   *  
//   *  CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
//   *  SELECTOR = DOUBLE (HIGH)
//   *  SHOTS >= 1
//   *  S1 = LOW (RED BUTTON PRESSED)
//   *  CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
//   *  
//   *  SELECTOR = NORMAL (LOW)
//   *  SHOTS >= 1
     

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

            }   // END Of else if (pressTime > longPress) {

          if (DS2408(0) < 100)
  {
    byte ActualSlot = (DS2408(0));
                                      #if ISDEBUG 
                                      Serial.print ("Selector: ");
                                      Serial.println (ActualSlot);
                                      #endif
    Dongle (ActualSlot);

    byte S1 = DS2408(1);
                                      #if ISDEBUG 
                                      Serial.print ("S1: ");
                                      Serial.println (S1);
                                      #endif

    byte S2 = DS2408(2);
                                      #if ISDEBUG 
                                      Serial.print ("S2: ");
                                      Serial.println (S2);
                                      #endif
    return;
  }
  if (DS2408(0) == 100)
  {
                                      #if ISDEBUG 
                                      Serial.println ("FLASH");
                                      #endif
    return;
  }
  if (DS2408(0) == 200)
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
  }
  //  int DS2408();
  return;



            //---------------------------------------------------------------------------
#if MOTOR           
  }
#endif

} //END OF loop      

//***************************************************************************************************************************************
*/
