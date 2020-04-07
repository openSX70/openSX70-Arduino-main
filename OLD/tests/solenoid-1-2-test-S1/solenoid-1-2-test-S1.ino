const int Solenoid1 = 3;           // 6V High Power
const int Solenoid2 = 11;          // 6V High Power
const int S1 = 12;     //Red button SHUTTER RELEASE


void setup() {
  // put your setup code here, to run once:

  pinMode(Solenoid1, OUTPUT);
  pinMode(Solenoid2, OUTPUT);
  pinMode(S1, INPUT_PULLUP);
  
  analogWrite(Solenoid1, 0);
  analogWrite(Solenoid2, 0);


}

void loop() {
  // put your main code here, to run repeatedly:

   if ((digitalRead(S1) == LOW) ) 
      {
     analogWrite (Solenoid1, 255);
     delay (400);
    analogWrite(Solenoid2, 255);
    analogWrite (Solenoid1, 0);
      } else
    analogWrite(Solenoid2, 0);
    analogWrite (Solenoid1, 255);
    delay (40);
    analogWrite (Solenoid1, 0);
      
}
