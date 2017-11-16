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
     
void motorON()
{
                                        #if ISDEBUG 
                                      Serial.println ("motorON");
                                      #endif
  digitalWrite(Motor, HIGH);
}

void motorOFF()
{
                                      #if ISDEBUG 
                                    Serial.println ("motorOFF");
                                    #endif
  digitalWrite(Motor, LOW);
}

void shutterCLOSE()
{
                                      #if ISDEBUG 
                                    Serial.println ("shutterCLOSE");
                                    #endif
  analogWrite(Solenoid1, 255);
                                      #if ISDEBUG 
                                    Serial.println ("Delay 15ms");
                                    #endif
  delay (15);                                        //Wait for the SOL#1 in BC position, more tests perhaps need
                                    #if ISDEBUG 
                                    Serial.println ("shutter in power down mode (130)");
                                    #endif
  analogWrite (Solenoid1, 130);
  
  }   //end of void motorON()

//***************************************************************************************************************************************

void shutterOPEN()
{
                            #if ISDEBUG 
                          Serial.println ("shutterOPEN");
                          #endif
analogWrite (Solenoid1,0);

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

}   //end of mirrorUP();
//***************************************************************************************************************************************

void darkslideEJECT()
{
                              #if ISDEBUG 
                            Serial.println ("darkslideEJECT");
                            #endif
  
  mirrorUP();
  mirrorDOWN();
}   // end of void darkslideEJECT()

//***************************************************************************************************************************************

void Click()
{
                            #if ISDEBUG 
                            Serial.println ("Click");
                            #endif
  
  shutterOPEN ();  //SOLENOID OFF MAKES THE SHUTTER TO OPEN!

  ActualSlot = TimeSlot(0);

  if (ActualSlot == 0)                                        //THIS IS THE B "BULB" OPTION WHILE S1 (Red SHUTTER button is pressed) the shutter is OPEN
  { ;
    while (digitalRead(S1) == LOW);
    shutterCLOSE ();
    return;
  }
  // TimeSlot = 9

  if (ActualSlot == 9)
  { ;
    (digitalRead(S1) == HIGH );                        //THIS IS THE T "TIME" OPTION PRESS ONCE S1 (Red SHUTTER button) the shutter is OPEN and AGAIN to close
    shutterCLOSE ();
    return;
  }
  delay (ShutterSpeed[ActualSlot]);                        // NOW COMES THE DELAY THAT IS THE PHOTO!

  //CLICK!

  shutterCLOSE ();                                         //close the shutter

} //en of void Click()
void HighSpeedPWM ()
 {
  //PWM high speed
  //one N_Mosfet powerdown
  //taken from: https://www.gammon.com.au/forum/?id=11504

  TCCR2A = bit (WGM20) | bit (WGM21) | bit (COM2B1); // fast PWM, clear OC2A on compare
  TCCR2B = bit (WGM22) | bit (CS20);         // fast PWM, no prescaler
  OCR2A =  n;                                // from table  
  OCR2B = ((n + 1) / 2) - 1;                 // 50% duty cycle
  //THIS AFFECTS OUTPUT 3 AND OUTPUT 11 (Solenoid1 and Solenoid2) 
 }
//***************************************************************************************************************************************
