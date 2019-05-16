int sensorValue;
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
}
 

void R14()
{
//Connect 100R
//Disconnect rest
//R14 on the PCB
pinMode (A4, INPUT);
pinMode (A5, INPUT);
pinMode (A7, OUTPUT);  // this is the one "connected"
digitalWrite (A7, LOW);
 
return ;
}
 
void loop() {
  // read sensor and print values
 
R14();
delay (10);
sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
if (sensorValue == 0)
{
Serial.print ("  100R: null");
} 
else if (sensorValue == 1023) 
{
Serial.print ("  100R: max");
}
else 
{
Serial.print ("  100R: ");
Serial.println(sensorValue);
delay (500);
}
 
}
 
