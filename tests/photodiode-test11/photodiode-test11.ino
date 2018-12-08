int sensorValue;
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
}
 
 
void R11()
{
//Connect 10K
//Disconnect rest
//R11 on the PCB
//pinMode (A7, INPUT);
pinMode (A4, INPUT);
pinMode (A5, OUTPUT);  // this is the one "connected"
digitalWrite (A5, LOW);
 
return ;
}

void R15()
{
//Connect 1M
//Disconnect rest
//R15 on the PCB
//pinMode (A7, INPUT);
pinMode (A5, INPUT);
pinMode (A4, OUTPUT);  // this is the one "connected"
digitalWrite (A4, LOW);
 
return ;
}

float exposureTime (int sensor) {
float t = (-0,03846154*(sensor) + 20,23077);
if (t < 1) {
  t=1;
} else if   (t > 18)
{
  //LED ROJO
  return;
}

return t;
}
/*
void R14()
{
//Connect xx DO NOT USE!!!
//Disconnect rest
//R14 on the PCB
pinMode (A4, INPUT);
pinMode (A5, INPUT);
pinMode (A7, OUTPUT);  // this is the one "connected"
digitalWrite (A7, LOW);
 
return ;
}*/

void loop() {
  // read sensor and print values
 
/*
R14();
delay (10);
sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
if (sensorValue == 0)
{
Serial.print ("  10K: null");
} 
else if (sensorValue == 1023) 
{
Serial.print ("  10K: max");
}
else 
{
Serial.print ("  10K: ");
Serial.print(sensorValue);
delay (500);
}
*/
R11();
delay (10);
//sensorValue = analogRead(A3);
//sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
if (sensorValue == 0)
{
Serial.print ("          HIGH LIGHT 10K: null");
} 
else if (sensorValue == 1023) 
{
Serial.print ("          HIGH LIGHT 10K: max");
}
else 
{
Serial.print ("          HIGH LIGHT 10K: ");
Serial.print(sensorValue);
delay (500);
}
 
 
R15();
delay (10);
sensorValue = analogRead(A3);
//sensorValue = analogRead(A3);
//sensorValue = analogRead(A3);
if (sensorValue == 0)
{
Serial.println ("          LOW LIGHT 1M: null");
Serial.println("--------------------------------------------------------");
} 
else if (sensorValue == 1023) 
{
Serial.println ("          LOW LIGHT 1M: max");
Serial.println("--------------------------------------------------------");
}
else 
{
Serial.print ("          LOW LIGHT 1M: ");
Serial.println (sensorValue);
Serial.println("--------------------------------------------------------");
delay (500);
}

}
  
