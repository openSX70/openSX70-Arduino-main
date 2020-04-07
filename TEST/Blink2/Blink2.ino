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
int led2 = A3;
unsigned int cycle = 1;

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
void loop() {
  Serial.print ("Cycle:  ");
  Serial.println (cycle);
  cycle = (cycle+ 1);
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 
  digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
  digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
    digitalWrite(led1, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
}
