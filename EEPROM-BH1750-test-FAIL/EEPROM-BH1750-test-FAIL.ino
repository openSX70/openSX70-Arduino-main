//Light meter software without pullup resistors using internal pullups.
#include <Wire.h>
#include <BH1750.h>
#include <EEPROM.h>

//byte eeCounter = 0;
//byte picCounter = 0;
/*
struct Picture{
  byte Index;
  int  ShutterSpeed;
  uint16_t lux;
};
*/

 
  struct Picture  {
  byte StructPicture;
  int StructSpeed;
  uint16_t StructLux;
  };
//int InitPicture;
//int TestPicture;

int eeAddress;


// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 6;
int dumpButton = 4;
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
  pinMode(dumpButton, INPUT_PULLUP);
  pinMode(led,OUTPUT);
    lightmeter.begin(luxMode); // Inicializar BH1750
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
/*
int InitPicture;

int TestPicture;
*/
/*
Picture InitPicture;
Picture TestPicture ={ 1111111111, 2222222222, 3333333333};
EEPROM.get (10,InitPicture);

if (TestPicture.StructPicture != InitPicture.StructPicture)
{

EEPROM.put(10,1);
EEPROM.put(1,10);  // where to write

}


EEPROM.get(1,eeAddress);  // where to write
//byte ActualCounter = EEPROM.read(eeCounter);


//EEPROM.get (10, ActualPicture); //the picture taken counter

*/

}


// the loop routine runs over and over again forever:
void loop() //*************************************************************************************************************************************************************************

{

int ShutterSpeed = 66;
//uint16_t lux;

uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
/* 
  struct Picture  {
  byte StructPicture;
  int StructSpeed;
  uint16_t StructLux;
  };
*/
int ActualPicture;
Picture MyPicture ={ ActualPicture, ShutterSpeed, lux};
  if (digitalRead(pushButton) == LOW) 
  {
  
//    digitalWrite (led, HIGH);  

//      Picture MyPicture ={ ActualPicture, ShutterSpeed, lux};
    
    EEPROM.put(eeAddress,MyPicture);
    
  
//eeCounter += sizeof(Picture);
  // read the input pin:
//bool buttonState = digitalRead(pushButton);
// print out the state of the button:

   
    EEPROM.get (eeAddress,MyPicture);

//    Picture MyPicture;
  Serial.println("****************************************************************");
  Serial.print("eeAddress read: ");
  Serial.println (eeAddress);
  Serial.println ("-");
  Serial.print("ActualPicture read: ");
  Serial.println(ActualPicture);
  Serial.println ("-");
  EEPROM.get( eeAddress, MyPicture );  
  Serial.print( " Picture: " );
  Serial.println( MyPicture.StructPicture );
  Serial.print( " ShutterSpeed: " );

  Serial.println( MyPicture.StructSpeed );
    Serial.print( " Lux: " );

  Serial.println( MyPicture.StructLux );
    
    
//    EEPROM.write(eeCounter, ActualCounter++);
//    EEPROM.put(0,ActualPicture);

    uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
//  Serial.print(F("Check Lux:  "));
//  Serial.print(lux);
//  Serial.println(" lx");
//  Serial.print("ActualPicture = ");
//  Serial.println(ActualPicture);
//  delay(1000);

  eeAddress += sizeof(MyPicture);  //Obtener la siguiente posicion para escribir
  if(eeAddress+sizeof(MyPicture) >= EEPROM.length()) eeAddress = 0;  //Comprobar que no hay desbordamiento

 
 EEPROM.update (1,eeAddress);

 ActualPicture = ActualPicture+1;

 EEPROM.update (10,ActualPicture);

delay (500);
  }

  if (digitalRead(dumpButton) == LOW) 
  {

EEPROM.get(1,eeAddress);
int ReadAddress = (eeAddress - (sizeof(MyPicture)*8));

Serial.print("======================= Entering loop =======================");
Serial.print("eeAddress before loop: ");
Serial.println (eeAddress);
  
Serial.print("ReadAddress before loop: ");
Serial.println (ReadAddress);
  
for (int i = 0; i < 8; i++)
{
  int thisRecordAddress = ReadAddress + (i * sizeof(MyPicture));
  EEPROM.get(thisRecordAddress, MyPicture);
  Serial.println("=======================================================");
  Serial.print("eeAddress read: ");
  Serial.println (thisRecordAddress);
  Serial.println ("-");
  Serial.print("ActualPicture read: ");
  Serial.println(ActualPicture);
  Serial.println ("-");
  Serial.print( " Picture: " );
  Serial.println( MyPicture.StructPicture );
  Serial.print( " ShutterSpeed: " );

  Serial.println( MyPicture.StructSpeed );
  Serial.print( " Lux: " );

  Serial.println( MyPicture.StructLux );
  
  delay(500);  
  
}
Serial.print("======================= After loop =======================");
Serial.print ("Read: ");
Serial.println (ReadAddress);

    digitalWrite (led, HIGH);  
    delay (1000);
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
  
