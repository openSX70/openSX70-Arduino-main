// --------------------------------------------------------------------------------------------------------------------------------------

//                                CAMERA CONTROL

// --------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************

int REDbutton(int button) {

               #if ISDEBUG 
              //Serial.println("REDbutton");      
              #endif

    
    // REDbutton (button pin)
    // REDbutton is a standalone function
    // by Joaquín de Prada
  
// REDbutton variables  
static boolean buttonActive = false;
static unsigned long buttonTimer = 0;
static unsigned long STARTbuttonTimer = 0 ;
// END of REDbutton variables
    
    pinMode(button, INPUT_PULLUP);
 
    
int result= 0 ;

                 #if ISDEBUG 
                 //Serial.print ("SHUTTERSPEED:  ");    
                 //Serial.println (ShutterSpeed[DS2408(0)]);
                 #endif

if (digitalRead(button) == LOW) {
            if (buttonActive == false) {
               #if ISDEBUG 
              Serial.println("button pressed");      
              #endif
            STARTbuttonTimer = millis(); 
            buttonActive = true ;
                                        }
     
     } else  {


      if (buttonActive == true) {
            buttonTimer = millis();
               #if ISDEBUG 
            Serial.println("button just released");
            Serial.print("pressed time inside function:  ");
              #endif
            result = (buttonTimer - STARTbuttonTimer);
               #if ISDEBUG 
            Serial.println(result);
              #endif
            buttonActive = false ;
     }
     }
                      return result;

     } // END OF REDbutton function
//***************************************************************************************************************************************
void ShutterT()
{
                 #if ISDEBUG 
                 Serial.println ("SHUTTER T");    
                 #endif
}

void ShutterB()
{
               #if ISDEBUG 
               Serial.println ("SHUTTER B");    
               #endif
}
//***************************************************************************************************************************************
void motorON()
{
                                        #if ISDEBUG 
                                      Serial.println ("motorON");
                                      #endif
  digitalWrite(Motor, HIGH);
}

//***************************************************************************************************************************************
void motorOFF()
{
                                      #if ISDEBUG 
                                    Serial.println ("motorOFF");
                                    #endif
  digitalWrite(Motor, LOW);
}

//***************************************************************************************************************************************
void shutterCLOSE()
{
                                      #if ISDEBUG 
                                    Serial.println ("shutterCLOSE");
                                    #endif
                                    HighSpeedPWM ();
                                    analogWrite(Solenoid1, 255);
                                      #if ISDEBUG 
                                    Serial.println ("Delay 25ms");
                                    #endif
                                    delay (25);                                        //Wait for the SOL#1 in BC position, more tests perhaps need changed to 25
                                    analogWrite (Solenoid1, 195);
  return;
  }   //end of void motorON()

//***************************************************************************************************************************************
void shutterOPEN()
{
                            #if ISDEBUG 
                          Serial.println ("shutterOPEN");
                          #endif
analogWrite (Solenoid1,0);
return;

}     //end of void shutterOPEN()
//***************************************************************************************************************************************

void mirrorDOWN()
{
                            #if ISDEBUG 
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
                            #if ISDEBUG 
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
                              #if ISDEBUG 
                            Serial.println ("darkslideEJECT");
                            #endif
  
  mirrorUP();
  mirrorDOWN();
  return;
}   // end of void darkslideEJECT()

//***************************************************************************************************************************************

void Click()
{
                            #if ISDEBUG 
                            ActualSlot = DS2408(0);
//                            Serial.print ("ClickClickClickClickClickClickClickClickClickClickClickClickClickClickClick: ");
//                            Serial.println (ShutterSpeed[ActualSlot]);
                            #endif
  

  ActualSlot = DS2408(0);

  if ((ShutterSpeed[ActualSlot]) == POST)
  {
    // case T
     pressTime = 0;
          
      #if ISDEBUG
      Serial.println ("MODE T");
      delay (1000);
      #endif
      
      #if SHUTTER
     shutterOPEN (); 
     
     while (pressTime > shortPress)
     int pressTime = REDbutton(S1);
      shutterCLOSE();
      #endif    

     
          
  }

  if ((ShutterSpeed[ActualSlot]) == POSFLASH)
  {
    // case FLASH DONGLE
                                  #if ISDEBUG
                                  Serial.println ("MODE FLASH IN DONGLE POSITION");
                                  #endif
//delay (10000);
                            #if ISDEBUG 
                            ActualSlot = DS2408(0);
                            Serial.println ("Click: FLASH ");
                            #endif
                  #if SHUTTER
                  shutterOPEN (); 
                  delay (51);
                 WritePIO (7,1);
                  //                        digitalWrite(FFA, HIGH);
                  delay (25);
                  //                      digitalWrite(FFA, LOW);
                  WritePIO (7,0);
                  delay (26);
                  shutterCLOSE();
                  #endif
                  return;
  }

  if ((ShutterSpeed[ActualSlot]) > 0)

  {
        #if ISDEBUG
    Serial.print ("--------------------------------------------------CLICK:  ");
    Serial.println (ShutterSpeed[ActualSlot]);
    #endif
    
    shutterOPEN ();  //SOLENOID OFF MAKES THE SHUTTER TO OPEN!
        delay (ShutterSpeed[ActualSlot]);                        // NOW COMES THE DELAY THAT IS THE PHOTO!
        ////////CLICK!
    shutterCLOSE ();                                         //close the shutter
    return;
            } else {

                    return;
                  }

} //end of void Click()

void Ydelay ()
{
                    if (DS2408(2) ==  1) {
                    //NORMAL DELAY
                    delay (40);                                 //S3 is now open start Y-delay (40ms)
                   Serial.println ("NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY NORMAL Y-DELAY");     

                        } 
                        else 
                        {
                    delay (1000);                              //LONG DELAY SELECTED 
                    Serial.println ("LONG DELAY**************************************************************************************************************************************");     
                        }
}
//***************************************************************************************************************************************
void HighSpeedPWM ()
 {
  //PWM high speed
  //one N_Mosfet powerdown
  //taken from: https://www.gammon.com.au/forum/?id=11504
/*
 * 
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
