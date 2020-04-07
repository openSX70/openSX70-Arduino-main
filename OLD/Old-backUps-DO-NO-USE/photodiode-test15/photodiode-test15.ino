int sensorValue;
int inputPin = A3;
const int numReadings0 = 10;
const int numReadings1 = 10;
const int numReadings2 = 10;

int readings0[numReadings0];      // the readings from the analog input
int readIndex0 = 0;              // the index of the current reading
int total0 = 0;                  // the running total
int average0 = 0;                // the average

int readings1[numReadings1];      // the readings from the analog input
int readIndex1 = 0;              // the index of the current reading
int total1 = 1;                  // the running total
int average1 = 0;                // the average

int readings2[numReadings2];      // the readings from the analog input
int readIndex2 = 0;              // the index of the current reading
int total2 = 0;                  // the running total
int average2 = 0;                // the average

  
void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);

  // initialize all the readings to 0:
  for (int thisReading0 = 0; thisReading0 < numReadings0; thisReading0++) {
    readings0[thisReading0] = 0;}

      for (int thisReading1 = 0; thisReading1 < numReadings1; thisReading1++) {
    readings1[thisReading1] = 0;}

      for (int thisReading2 = 0; thisReading2 < numReadings2; thisReading2++) {
    readings2[thisReading2] = 0;}
}
 
 /*
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
}*/
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
}*/
 
void loop() {
  // read sensor and print values
/* 
R14();

//sensorValue = analogRead(A3);
// subtract the last reading:
  total0 = total0 - readings0[readIndex0];
  // read from the sensor:
  readings0[readIndex0] = analogRead(inputPin);
  delay (10);
    // add the reading to the total:
  total0 = total0 + readings0[readIndex0];
  // advance to the next position in the array:
  readIndex0 = readIndex0 + 1;

  // if we're at the end of the array...
  if (readIndex0 >= numReadings0) {
    // ...wrap around to the beginning:
    readIndex0 = 0;
  // calculate the average:
  average0 = total0 / numReadings0;

  Serial.print ("  100R: ");
  Serial.print(average0);

  }
 
R11();

//sensorValue = analogRead(A3);
// subtract the last reading:
  total1 = total1 - readings1[readIndex1];
  // read from the sensor:
  readings1[readIndex1] = analogRead(inputPin);
  delay (10);
  // add the reading to the total:
  total1 = total1 + readings1[readIndex1];
  // advance to the next position in the array:
  readIndex1 = readIndex1 + 1;

  // if we're at the end of the array...
  if (readIndex1 >= numReadings1) {
    // ...wrap around to the beginning:
        // calculate the average:
        average1 = total1 / numReadings1;

        Serial.print ("          1K: ");
        Serial.print(average1);

    readIndex1 = 0;
  }

// delay (500);

 */
 
R15();

//sensorValue = analogRead(A3);
// subtract the last reading:
  total2 = total2 - readings2[readIndex2];
  // read from the sensor:
  readings2[readIndex2] = analogRead(inputPin);
  delay (10);
  // add the reading to the total:
  total2 = total2 + readings2[readIndex2];
  // advance to the next position in the array:
  readIndex2 = readIndex2 + 1;

  // if we're at the end of the array...
  if (readIndex2 >= numReadings2) {
    // ...wrap around to the beginning:
    readIndex2 = 0;
  // calculate the average:
      average2 = total2 / numReadings2;

      Serial.print ("          10K: ");
      Serial.println (average2);
      Serial.println("-----------------------------------------------------");
  }


delay (200);
}
