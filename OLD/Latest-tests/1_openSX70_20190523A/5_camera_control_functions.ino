// --------------------------------------------------------------------------------------------------------------------------------------

//                                CAMERA CONTROL

// --------------------------------------------------------------------------------------------------------------------------------------

//***************************************************************************************************************************************
void ShutterT()
{
                 #if SIMPLEDEBUG 
                 Serial.println ("SHUTTER T");    
                 #endif

                  shutterCLOSE (); 
                  
                  
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN˚
                   ;
                  delay (40);                               //S3 is now open start Y-delay (40ms)
                   
                  
                  shutterOPEN ();
                   
                        while (digitalRead(S1) == HIGH)
                        ;

                        if (digitalRead(S1) == LOW)
                        {
                        shutterCLOSE ();
                        }
                  
                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (200);                             //AGAIN is this delay necessary?

                 shutterOPEN();
                 //shots = 0;  
                 return;       

}
void ShutterB()
{
               #if SIMPLEDEBUG 
               Serial.println ("SHUTTER B");    
               #endif

                  shutterCLOSE (); 
                  
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN˚
                   ;
                  delay (40);                               //S3 is now open start Y-delay (40ms)
 
                  if (switch2 ==  1) //CASE DONGLE FLASH AT THE END OF B
                  {
                  analogWrite(Solenoid2, 255);
                  }
                  shutterOPEN ();
                   if (switch2 ==  1) //CASE DONGLE FLASH AT THE END OF B
                  {
                  analogWrite(Solenoid2, 130);
                  }
                        while (digitalRead(S1) == LOW)
                        ;
                   if (switch2 ==  1) //CASE DONGLE FLASH AT THE END OF B
                  {
                  Write_DS2408_PIO (7,1);
                  delay (1);
                  analogWrite (Solenoid2,0);
                  Write_DS2408_PIO (7,0);                  
                  }
                        shutterCLOSE ();


                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (200);                             //AGAIN is this delay necessary?

                 shutterOPEN();
                 //shots = 0;  

              return;    

}
//***************************************************************************************************************************************
void motorON()
{
                                        #if SIMPLEDEBUG 
                                      Serial.println ("motorON");
                                      #endif
  digitalWrite(Motor, HIGH);
}

//***************************************************************************************************************************************
void motorOFF()
{
                                      #if SIMPLEDEBUG 
                                    Serial.println ("motorOFF");
                                    #endif
  digitalWrite(Motor, LOW);
}

//***************************************************************************************************************************************
void shutterCLOSE()
{
                                      #if SIMPLEDEBUG 
                                    Serial.println ("shutterCLOSE");
                                    #endif
                                    HighSpeedPWM ();
                                    analogWrite(Solenoid1, 255);
  
                                    delay (PowerDownDelay);                                        //Wait for the SOL#1 in BC position, more tests perhaps need changed to 25
                                    analogWrite (Solenoid1, PowerDown);

                                    return;
  //return;
  }   //end of void motorON()

//***************************************************************************************************************************************
void shutterOPEN()
{
                            #if SIMPLEDEBUG 
                          Serial.println ("shutterOPEN");
                          #endif
analogWrite (Solenoid1,0);
return;

}     //end of void shutterOPEN()
//***************************************************************************************************************************************

void mirrorDOWN()
{
                            #if SIMPLEDEBUG 
                          Serial.println ("mirrorDOWN");
                          #endif
  motorON ();

  while (digitalRead(S5) != LOW)  //While S5 open keep the motor running and lower the mirror
    ;
  motorOFF ();
  return;
}    //end of void mirrorDOWN()
//***************************************************************************************************************************************

void mirrorUP()
{
                            #if SIMPLEDEBUG 
                          Serial.println ("mirrorUP");
                          #endif
  motorON ();

  while (digitalRead(S5) != HIGH)            //waiting for S5 to OPEN do NOTHING
    ;
  //S5 OPENS
  //S1 MAY BE OPEN NOW (DON'T KNOW HOW TO DO THIS YET)
  // Motor Brake
  motorOFF ();
  return;

}   //end of mirrorUP();
//***************************************************************************************************************************************

void darkslideEJECT()
{
                              #if SIMPLEDEBUG 
                            Serial.println ("darkslideEJECT");
                            #endif
  shutterCLOSE();
  mirrorUP();
  mirrorDOWN();
  shutterOPEN();
  return;
}   // end of void darkslideEJECT()

//***************************************************************************************************************************************
/*
void Click(int ExpDelay)
{
                            #if SIMPLEDEBUG 
//                            ActualSlot = Read_DS2408_PIO(0);
//                            Serial.print ("ClickClickClickClickClickClickClickClickClickClickClickClickClickClickClick: ");
//                            Serial.println (ShutterSpeed[ActualSlot]);
                            #endif
  

//  ActualSlot = Read_DS2408_PIO(0);
/*
  if ((ShutterSpeed[Read_DS2408_PIO(0)]) == POST)
  {
    // case T
     pressTime = 0;
          
      #if SIMPLEDEBUG
      Serial.println ("MODE T");
      delay (1000);
      #endif
      
           shutterOPEN (); 
     
     while (pressTime > shortPress)
     int pressTime = REDbutton(S1);
      shutterCLOSE();

     
          
  }

  if ((ShutterSpeed[Read_DS2408_PIO(0)]) == POSFLASH)
  {
    // case FLASH DONGLE
                                  #if SIMPLEDEBUG
                                  Serial.println ("MODE FLASH IN DONGLE POSITION");
                                  #endif
//delay (10000);
                            #if SIMPLEDEBUG 
                            //ActualSlot = Read_DS2408_PIO(0);
                            Serial.println ("Click: FLASH ");
                            #endif

                  shutterOPEN (); 
                  delay (51);
                 Write_DS2408_PIO (7,1);
                  //                        digitalWrite(FFA, HIGH);
                  delay (25);
                  //                      digitalWrite(FFA, LOW);
                  Write_DS2408_PIO (7,0);
                  delay (26);
                  shutterCLOSE();
                  return;
  }

  int ShutterSpeedDelay;
  if ((ShutterSpeed[selector]) > 0)

  {
        #if SIMPLEDEBUG
    Serial.print ("--------------------------------------------------CLICK:  ");
    Serial.println (ShutterSpeed[selector]);
    #endif
   startCounterCalibration();
    shutterOPEN ();  //SOLENOID OFF MAKES THE SHUTTER TO OPEN!
        if (ExpDelay == 0){
        ShutterSpeedDelay = (ShutterSpeed[selector]+ShutterConstant) ;
//        delay (ShutterSpeed[Read_DS2408_PIO(0)]);                        // NOW COMES THE DELAY THAT IS THE PHOTO!
//Serial.println (ShutterSpeedDelay);
        
        delay (ShutterSpeedDelay);                        // NOW COMES THE DELAY THAT IS THE PHOTO!
          
        }
        else {

//        delay (ExpDelay);                        // NOW COMES THE DELAY THAT IS THE PHOTO!
//        Serial.print("        actual delay: ");
//        Serial.println (ExpDelay);
          
        }

        
        ////////CLICK!
if (ShutterSpeedDelay >= FastestFlashSpeed)
     {
      FastFlash ();
     }
    shutterCLOSE ();                                         //close the shutter
    //delay (10);
    cli();
    counter = TCNT1;
    sei();
//    Serial.begin(9600);
//    Serial.print ("                       counter: ");
//    Serial.print (counter);

    return;
            } 

} //end of void Click()

*/




void Ydelay ()
{

//Only one 120ms delay. Switch 2 for other purposes!!
  
  delay (120);
/*
                    if (switch2 ==  0) {
                    //NORMAL DELAY
                    delay (40);                                 //S3 is now open start Y-delay (40ms)
                   
                        } 
                        else 
                        {
                    delay (200);                              //LONG DELAY SELECTED 
                    //Serial.println ("LONG DELAY**************************************************************************************************************************************");     
                       }
*/ 
return;

}
//***************************************************************************************************************************************
void HighSpeedPWM ()
{

  

  //PWM high speed
  //one N_Mosfet powerdown
  //taken from: https://www.gammon.com.au/forum/?id=11504
/* 
  Timer 0
  input     T0     pin  6  (D4)
  output    OC0A   pin 12  (D6)
  output    OC0B   pin 11  (D5)

  Timer 1
  input     T1     pin 11  (D5)
  output    OC1A   pin 15  (D9)
  output    OC1B   pin 16  (D10)

  Timer 2
  output    OC2A   pin 17  (D11)
  output    OC2B   pin  5  (D3)
 
*/

  TCCR2A = bit (WGM20) | bit (WGM21) | bit (COM2B1); // fast PWM, clear OC2A on compare
  TCCR2B = bit (WGM22) | bit (CS20);         // fast PWM, no prescaler
  OCR2A =  n;                                // from table  
  OCR2B = ((n + 1) / 2) - 1;                 // 50% duty cycle
  
  //THIS AFFECTS OUTPUT 3 AND OUTPUT 11 (Solenoid1 and Solenoid2)

 }
//***************************************************************************************************************************************
