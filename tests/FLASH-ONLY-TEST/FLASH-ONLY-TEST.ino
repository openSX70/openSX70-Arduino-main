const int S1 = 12;     //Red button SHUTTER RELEASE
const int FFA = 4;
void setup() {
  // put your setup code here, to run once:
pinMode(S1, INPUT_PULLUP);
pinMode(FFA, OUTPUT);
 Serial.begin(9600);
Serial.println ("Flash test");
}

void loop() {
  // put your main code here, to run repeatedly:

if (digitalRead(S1) == LOW) {
       
                  digitalWrite(FFA, HIGH);
                  Serial.println ("FFA, HIGH");
                  delay (55);
   //               analogWrite (Solenoid2,0);
                  digitalWrite(FFA, LOW);
                  Serial.println ("FFA, LOW");

}
delay (500);
}
