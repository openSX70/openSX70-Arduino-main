float voltage;
byte  level = 1;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(57600);
float voltage = 50;
int level = 1;
}

float test (int type){

if (type == 1) {
//Connect 8M
//Disconnect rest
pinMode (A2, INPUT);
pinMode (A3, INPUT);
pinMode (A1, OUTPUT);  // No se si esto es necesario…
digitalWrite (A1, LOW);
int sensorValue = analogRead(A0);
voltage = (sensorValue * (100.0 / 1023.0));
return voltage;
}
if (type == 2) {
//Connect 1M
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A3, INPUT);
pinMode (A2, OUTPUT);  // No se si esto es necesario…
digitalWrite (A2, LOW);
int sensorValue = analogRead(A0);
voltage = (sensorValue * (100.0 / 1023.0));
return voltage;
}
if (type == 3) {
//Connect 100K
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A2, INPUT);
pinMode (A3, OUTPUT);  // No se si esto es necesario…
digitalWrite (A3, LOW);
int sensorValue = analogRead(A0);
voltage = (sensorValue * (100.0 / 1023.0));
return voltage;
}


 
} // end of function test

void loop() {
  // put your main code here, to run repeatedly:
voltage = test (level);

Serial.print ("level: (");
Serial.print (level);
Serial.print (")");
Serial.println(voltage);

if ((voltage = 100) && (level <3))
  {
  level = level++;
  
} else if ((voltage <= 0) && (level < 0)) {
  level = level - 1;
 
}


}
