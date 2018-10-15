float voltage;
byte  level = 1;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(57600);
float voltage = 50;
int level = 1;
}

void RES1()
{
//Connect 8M
//Disconnect rest
pinMode (A2, INPUT);
pinMode (A3, INPUT);
pinMode (A1, OUTPUT);  // No se si esto es necesario…
digitalWrite (A1, LOW);
return;
}
void RES2()
{ 
//Connect 1M
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A3, INPUT);
pinMode (A2, OUTPUT);  // No se si esto es necesario…
digitalWrite (A2, LOW);
return;
}
void RES3()
{
//Connect 100K
//Disconnect rest
pinMode (A1, INPUT);
pinMode (A2, INPUT);
pinMode (A3, OUTPUT);  // No se si esto es necesario…
digitalWrite (A3, LOW);
return;
}

 void loop() {
  // put your main code here, to run repeatedly:

RES1();
int sensorValue = analogRead(A0);

if (sensorValue >= 1023)
{
RES2();
int sensorValue = analogRead(A0);
if (sensorValue >= 1023)
{
RES3();
}
}
sensorValue = analogRead(A0);

//Serial.print ("level: (");
//Serial.print (level);
//Serial.print (")");
Serial.println(sensorValue);


}
