/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
int led1 = 13;
int led2 = 5;
unsigned int cycle = 1;
int sensorValue;
int lightMeter = A3;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led1, OUTPUT);     
  pinMode(led2, OUTPUT);     
Serial.begin (9600);
Serial.println ("Welcome to the openSX70 project!");
Serial.println ("this Blink2 sketch");
Serial.println ();
Serial.println ("modified from original blink by Joaquín de Prada");
Serial.println ();
Serial.println ("it should blink in two different colours");
Serial.println ();
Serial.println ("LED1 is port 13 and LED2 is port 5 on Arduino");
Serial.println ();
Serial.println ();
}

// the loop routine runs over and over again forever:

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
  Serial.print ("Cycle:  ");
  Serial.println (cycle);
  cycle = (cycle+ 1);
  
  
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 
Serial.println ("LED1 is ON     LED2 is OFF");
  digitalWrite(led2, LOW
  );   // turn the LED on (HIGH is the voltage level)

Serial.println("---------------------------------------------------");

  Serial.println ("now I wait 1000ms");
  delay(1000);               // wait for a second

Serial.println("---------------------------------------------------");

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

  
  digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
Serial.println ("LED1 is OFF     LED2 is ON");
  digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)

Serial.println("---------------------------------------------------");

  Serial.println ("now I wait 1000ms again");
  delay(1000);               // wait for a second

Serial.println("---------------------------------------------------");


//delay (500);
}

}
