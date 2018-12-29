int sensorValue;
int lightMeter = A3;
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
}
 
 
void R11()
{
//Connect 1M5
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
//Connect 750K
//Disconnect rest
//R15 on the PCB
//pinMode (A7, INPUT);
pinMode (A5, INPUT);
pinMode (A4, OUTPUT);  // this is the one "connected"
digitalWrite (A4, LOW);
return ;
}

void loop() {
  // read sensor and print values

R11();
delay (10);
sensorValue = analogRead(lightMeter);
if (sensorValue == 0)
{
Serial.print ("LOW LIGHTS 1M5: too low");
} 
else if (sensorValue == 1023) 
{
Serial.print ("LOW LIGHTS 1M5: too much");
}
else 
{
Serial.print ("LOW LIGHTS 1M5: ");
Serial.print(sensorValue);
delay (500);
}

R15();
delay (10);
sensorValue = analogRead(lightMeter);
//sensorValue = analogRead(A3);
//sensorValue = analogRead(A3);
if (sensorValue == 0)
{
Serial.println ("   HIGH LIGHTS 750K: too low");
Serial.println("---------------------------------------------------");
} 
else if (sensorValue == 1023) 
{
Serial.println ("   HIGH LIGHTS 750K: too much");
Serial.println("---------------------------------------------------");
}
else 
{
Serial.print ("   HIGH LIGHTS 750K: ");
Serial.println (sensorValue);

Serial.println("---------------------------------------------------");
delay (500);
}
}
  
