//High speed PWM
const byte n = 224;  // for example, 71.111 kHz


const int S1 = 12;     //Red button SHUTTER RELEASE
const int Solenoid1 = 3;           // 6V High Power
const int LED = 13;
const int ARM = 8;

void setup() {
    pinMode(S1, INPUT_PULLUP);
    pinMode(Solenoid1, OUTPUT);
    pinMode(LED,OUTPUT);
    pinMode(ARM,INPUT_PULLUP);
        
    analogWrite(Solenoid1, 0);
  
    
}

void loop() {
  
  int Speed = 10;

while (digitalRead(ARM) == LOW) {
    HighSpeedPWM ();
    analogWrite (Solenoid1, 255);
    delay (10);
    analogWrite (Solenoid1,155); //OPTIONAL LOW POWER
       
      if (digitalRead(S1) == LOW) {
      //T=-1
      digitalWrite (LED, HIGH);
      delay (1);
      digitalWrite (LED, LOW);
      //T=0
      analogWrite (Solenoid1,0); //SHUTTER OPEN
      delay (Speed); //EXPOSURE
      //T=Speed-+
      analogWrite (Solenoid1,255); //SHUTTER CLOSE
      digitalWrite (LED, HIGH);
      delay (1);
      digitalWrite (LED, LOW);
      delay (10);
      analogWrite (Solenoid1,155); //OPTIONAL LOW POWER
      }

  }

}// END OF LOOP
//***************************************************************************************************************************************
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
