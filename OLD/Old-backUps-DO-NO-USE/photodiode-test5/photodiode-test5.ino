float voltage;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(57600);
pinMode (A0, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
int sensorValue = analogRead(A0);

//Serial.print ("level: (");
//Serial.print (level);
//Serial.print (")");
Serial.print ("Sensor Value: ");
Serial.println(sensorValue);

}
