//Light meter software without pullup resistors using internal pullups.
#include <Wire.h>
#include <BH1750.h>
#include <EEPROM.h>

//byte eeCounter = 0;
//byte picCounter = 0;

struct Picture{
  byte Index;
  int  ShutterSpeed;
  uint16_t lux;
};

byte eeAddress = EEPROM.read(1);  // where to write
//byte ActualCounter = EEPROM.read(eeCounter);
byte ActualPicture = EEPROM.read(10); //the pictures taken serial

//byte value = EEPROM.read(eeCounter);

//EEPROM.write(eeCounter, val

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

/*
if ((ActualCounter) == 255) 
{
  EEPROM.write (eeCounter, 1);
}
*/

}





// the loop routine runs over and over again forever:
void loop() {

int ShutterSpeed = 66;
    uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
  
  struct Picture = {
    ActualPicture,
    ShutterSpeed,
    lux
  };


  if (digitalRead(pushButton) == LOW) 
  {
  
    digitalWrite (led, HIGH);    
    
 
  EEPROM.put (eeAddress,current); 

  eeAddress += sizeof(float);  //Obtener la siguiente posicion para escribir
  if(eeAddress >= EEPROM.length()) eeAddress = 0;  //Comprobar que no hay desbordamiento

 EEPROM.update (0,eeAddress);

  ActualPicture = ActualPicture+1;

  EEPROM.update (10,ActualPicture);

  
//eeCounter += sizeof(Picture);
  // read the input pin:
//bool buttonState = digitalRead(pushButton);
// print out the state of the button:

    
    
    EEPROM.get (eeAddress,ActualPicture);


    Picture current;
  EEPROM.get( eeAddress, Picture );  
  Serial.println( "Estructura leida: " );
  Serial.println( Picture.ActualPicture );
  Serial.println( Picture.ShutterSpeed );
  Serial.println( Picture.lux );
    
    
//    EEPROM.write(eeCounter, ActualCounter++);
//    EEPROM.put(0,ActualPicture);
 
    uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
  Serial.print(F("Lux:  "));
  Serial.print(lux);
  Serial.println(" lx");
  Serial.println(ActualPicture);
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
  
