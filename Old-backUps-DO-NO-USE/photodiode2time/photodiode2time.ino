
int sensorValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (57600);

}

int Auto600 (int inputPin, int numReadings)
{
int readings[numReadings];      // the readings from the analog input
int readIndex ;              // the index of the current reading
int total ;                  // the running total
int average;                // the average

 // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++)   
  
{  readings[thisReading] = 0;}
 
int sensorValue = analogRead(inputPin);
        Serial.print ("sensorValue: ");
        Serial.println (sensorValue);
// subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  //delay (10);
    // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  // calculate the average:
  average = total / numReadings;
        Serial.print ("average: ");

        Serial.println (average);

float t = (-0.03846154*(average) + 20.23077);
int Auto600 = (int) t;
//float t = (-0.0385*(sensor) + 20.2307);
/*
if (Auto600 < 1) {
  Auto600 = 1;
} else if   (sensorValue >= 500)
{
  //LED ROJO
  Auto600 = 0;
  return;
}*/

return Auto600;
}
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
pinMode (A4, OUTPUT);  // this is the one "connected"
digitalWrite (A4, LOW);
 
return ;
}
void loop() {
  // put your main code here, to run repeatedly:
  R15();
  int exposureTime = Auto600 (A3,10);
  Serial.print ("exposure time: ");
  Serial.println (exposureTime);
  

}
