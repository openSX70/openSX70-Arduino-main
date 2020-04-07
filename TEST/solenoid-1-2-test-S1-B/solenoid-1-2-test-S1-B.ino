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

  Serial.begin (9600);
  Serial.println ("Sol1 & Sol2 running test");
}

void loop() {
  // put your main code here, to run repeatedly:

    Serial.println ("STARTING CYCLE");
    delay (1000);
  Serial.println ("engaging Solenoid1");
      
     analogWrite (Solenoid1, 255);
     delay (400);
  Serial.println ("engaging Solenoid2");
    analogWrite(Solenoid2, 255);
    delay (400);
    Serial.println ("disengaging Solenoid1");
    analogWrite (Solenoid1, 0);
    Serial.println ("Operation, test NOW");
    delay (5000);
    Serial.println ("disengaging Solenoid2");    
    analogWrite(Solenoid2, 0);
    delay (400);
    Serial.println ("briefly engaging Solenoid1");
    analogWrite (Solenoid1, 255);
    delay (40);
    Serial.println ("disengaging Solenoid1");
    analogWrite (Solenoid1, 0);
    Serial.println ("resting FULLY OPEN: END OF CYCLE");
      
}
