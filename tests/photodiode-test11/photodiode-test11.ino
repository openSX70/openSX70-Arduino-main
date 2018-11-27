int sensorValue;
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
}
 
 
void R11()
{
//Connect 50K
//Disconnect rest
//R11 on the PCB
pinMode (A7, INPUT);
pinMode (A4, INPUT);
pinMode (A5, OUTPUT);  // this is the one "connected"
digitalWrite (A5, LOW);
 
return ;
}
void R15()
{
//Connect 500K
//Disconnect rest
//R15 on the PCB
pinMode (A7, INPUT);
pinMode (A5, INPUT);
pinMode (A4, OUTPUT);  // this is the one "connected"
digitalWrite (A4, LOW);
 
return ;
}
 /*
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
 */
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
Serial.print ("  100R: null");
} 
else if (sensorValue == 1023) 
{
Serial.print ("  100R: max");
}
else 
{
Serial.print ("  100R: ");
Serial.print(sensorValue);
delay (500);
}
 */
R11();
delay (10);
//sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
if (sensorValue == 0)
{
Serial.print ("          50K: null");
} 
else if (sensorValue == 1023) 
{
Serial.print ("          50K: max");
}
else 
{
Serial.print ("          50K: ");
Serial.print(sensorValue);
delay (500);
}
 
 
R15();
delay (10);
sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
sensorValue = analogRead(A3);
if (sensorValue == 0)
{
Serial.println ("          500K: null");
Serial.println("--------------------------------------------------------");
} 
else if (sensorValue == 1023) 
{
Serial.println ("          500K: max");
Serial.println("--------------------------------------------------------");
}
else 
{
Serial.print ("          500K: ");
Serial.println (sensorValue);
Serial.println("--------------------------------------------------------");
delay (500);
}

}
  
