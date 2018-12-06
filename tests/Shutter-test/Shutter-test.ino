//High speed PWM
const byte n = 224;  // for example, 71.111 kHz


const int S1 = 12;     //Red button SHUTTER RELEASE
const int Solenoid1 = 3;           // 6V High Power
const int LED = 13;
//const int ARM = 8;
//const int ARMLED = 11;

void setup() {
    pinMode(S1, INPUT_PULLUP);
    pinMode(Solenoid1, OUTPUT);
    pinMode(LED,OUTPUT);
//    pinMode(ARMLED,OUTPUT);
//    pinMode(ARM,INPUT_PULLUP);
        
    analogWrite(Solenoid1, 0);
  
    
}

void loop() {

    //digitalWrite (ARMLED,LOW);
    //analogWrite(Solenoid1, 0);
  
  int Speed = 50;

while (digitalRead(S1) == LOW) {
    digitalWrite (LED,HIGH);
    HighSpeedPWM ();
    analogWrite (Solenoid1, 235);
    delay (5);
    analogWrite (Solenoid1,255); //OPTIONAL LOW POWER
      digitalWrite (LED,LOW);         
      analogWrite (Solenoid1,0);
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
