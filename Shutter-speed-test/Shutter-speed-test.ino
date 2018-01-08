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
