int sensorValue;
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
}


void R11()
{
//Connect 1K 
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
//Connect 10K
//Disconnect rest
//R15 on the PCB
pinMode (A7, INPUT);
pinMode (A5, INPUT);
pinMode (A4, OUTPUT);  // this is the one "connected"
digitalWrite (A4, LOW);

return ;
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
  // put your main code here, to run repeatedly:

R14();
sensorValue = analogRead(A3);
if ((sensorValue > 0) && (sensorValue < 1024)){
Serial.print (" 100R: ");
Serial.print(sensorValue);

delay (500);
} else {
Serial.print (" 100R: null");
  
}

R11();
sensorValue = analogRead(A3);
if ((sensorValue > 0) && (sensorValue < 1024)){
Serial.print ("          1K: ");
Serial.print(sensorValue);
delay (500);
} else {
Serial.print ("          1K: null");
  
}




R15();
sensorValue = analogRead(A3);
if ((sensorValue > 0) && (sensorValue < 1024)) {
Serial.print ("          10K: ");
Serial.println(sensorValue);
Serial.println("-----------------------------------------------");
delay (500);
} else {
Serial.println ("          10K: null");
Serial.println("-----------------------------------------------");  
}
}
