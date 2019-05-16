/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(57600);

}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
//Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
   

//Connect 8M
//Disconnect rest
pinMode (A2, INPUT);
pinMode (A3, INPUT);
pinMode (A1, OUTPUT);  // No se si esto es necesario…
digitalWrite (A1, LOW);
String res = "resistor 8M:  ";
float voltage ;
voltage = (sensorValue * (100.0 / 1023.0));

if ( voltage == 100);
{
//Connect 1M
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A3, INPUT);
pinMode (A2, OUTPUT);  // No se si esto es necesario…
digitalWrite (A1, LOW);
sensorValue = analogRead(A0);
res = "resistor 1M:  ";
Serial.print (res);
Serial.println(voltage);

return;
  if (voltage == 100);
{
//Connect 100K
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A3, INPUT);
pinMode (A2, OUTPUT);  // No se si esto es necesario…
digitalWrite (A1, LOW);

sensorValue = analogRead(A0);

voltage = (sensorValue * (100.0 / 1023.0));

res = "resistor 100K:  ";
Serial.print (res);
Serial.println(voltage);

return;
}
Serial.print (res);
Serial.println(voltage);
  
}


//Disconnect 8M
//pinMode (A1, INPUT);



//  float voltage = 1023 - (sensorValue) ;
  // print out the value you read:
//
voltage = (sensorValue * (100.0 / 1023.0));
Serial.print (res);
Serial.println(voltage);

//  Serial.println(sensorValue);
  delay(1000);
}
