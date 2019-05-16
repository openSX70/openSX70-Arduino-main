/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
  
This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
//float voltage ;

//=====================================================================================================================================

void Res8M()
{

  //Connect 8M
//Disconnect rest
pinMode (A2, INPUT);
pinMode (A3, INPUT);
pinMode (A1, OUTPUT);  // No se si esto es necesario…
digitalWrite (A1, LOW);
//String res = "resistor 8M:  ";
//voltage = (int sensorValue * (100.0 / 1023.0));
return;
}

void Res100K()
{
  //Connect 1M
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A3, INPUT);
pinMode (A2, OUTPUT);  // No se si esto es necesario…
digitalWrite (A2, LOW);
return;
}

void Res10K(){
  //Connect 100K
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A2, INPUT);
pinMode (A3, OUTPUT);  
digitalWrite (A3, LOW);
return;
}
//=====================================================================================================================================

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(57600);
String res;

Res100K();


}
void loop() {

   
   
 //  int sensorValue = analogRead(A0);
  // put your main code here, to run repeatedly:

String res = "resistor:100KK read value: ";
int sensorValue = analogRead (A0);


if (sensorValue > 99) {
  Res10K();
int sensorValue = analogRead (A0);
res = "resistor:10K read value: ";

} else if (sensorValue <= 0) {
  Res8M();
int sensorValue = analogRead (A0);
res = "resistor:8MM read value: ";
}
if (sensorValue >= 100) {
  Res10K;
  int sensorValue = analogRead (A0);
res = "resistor:10K read value: ";
}


float voltage = ( sensorValue * (100.0 / 1023.0));
Serial.print (res);
Serial.println(voltage);

//  Serial.println(sensorValue);
  delay(1000);






}
