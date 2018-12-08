

void setup() {
  // put your setup code here, to run once:
  Serial.begin (57600);

}

int Auto600 (int inputPin, int numReadings)
{
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

 // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;}

int sensorValue = analogRead(inputPin);
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


float t = (-0.03846154*(sensorValue) + 20.23077);
int Auto600 = (int) t;
//float t = (-0.0385*(sensor) + 20.2307);

if (Auto600 < 1) {
  Auto600 = 1;
} else if   (Auto600 > 18)
{
  //LED ROJO
  Auto600 = 0;
  return;
}

return Auto600;

}
}

void loop() {
  // put your main code here, to run repeatedly:

  int exposureTime = Auto600 (A3,5);
  Serial.print (exposureTime);
  

}
