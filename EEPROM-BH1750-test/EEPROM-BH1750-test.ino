//Light meter software without pullup resistors using internal pullups.
#include <Wire.h>
#include <BH1750.h>
#include <EEPROM.h>

byte eeCounter = 0;
int val = 1;

byte ActualCounter = EEPROM.read(eeCounter);

//byte value = EEPROM.read(eeCounter);

/*
if ((eeCounter) == 1) {
   EEPROM.write (eeCounter, 1);
}
*/

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 6;
int led = 5;

// the setup routine runs once when you press reset:

BH1750 lightmeter;

const byte luxMode = BH1750_CONTINUOUS_HIGH_RES_MODE;
// BH1750_CONTINUOUS_HIGH_RES_MODE
// BH1750_CONTINUOUS_HIGH_RES_MODE_2
// BH1750_CONTINUOUS_LOW_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE
// BH1750_ONE_TIME_HIGH_RES_MODE_2
// BH1750_ONE_TIME_LOW_RES_MODE

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  pinMode(led,OUTPUT);
    lightmeter.begin(luxMode); // Inicializar BH1750
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
//bool buttonState = digitalRead(pushButton);
// print out the state of the button:

  while (digitalRead(pushButton) == LOW) 
  {
  
    digitalWrite (led, HIGH);
 
    uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
  Serial.print(F("Lux:  "));
  Serial.print(lux);
  Serial.println(" lx");
  delay(500);

  }
  
  digitalWrite (led, LOW);
//Serial.println ( ActualCounter);

//    EEPROM.write(eeCounter, val);

/*
   uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
  Serial.print(F("Lux:  "));
  Serial.print(lux);
  Serial.println(" lx");
  delay(500);
*/
  }
  
