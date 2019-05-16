
void setup() {
  // put your setup code here, to run once:
  Serial.begin (57600);

}

int Auto600 (int inputPin)
{
 
int sensorValue = analogRead(inputPin);
// sensorValue = analogRead(inputPin);
Serial.print ("               SensorValue = ");
Serial.println (sensorValue);

float t = (-0.03846154*(sensorValue) + 20.23077);
int t2 = (int) t;
//float t = (-0.0385*(sensor) + 20.2307);

if (t2 < 1) {
  t2 = 1;
  return;
}
return t2; 
}


 
void R11()
{
//Connect 1M5 LOW
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
//Connect 750K HIGH
//Disconnect rest
//R15 on the PCB
//pinMode (A7, INPUT);
pinMode (A5, INPUT);
pinMode (A4, OUTPUT);  // this is the one "connected"digitalWrite (A4, LOW);
 
return ;
}
void loop() {
  // put your main code here, to run repeatedly:
  R11();
  int exposureTime = Auto600 (A3);
  Serial.print ("exposure time: ");
  Serial.println (exposureTime);
  

}
