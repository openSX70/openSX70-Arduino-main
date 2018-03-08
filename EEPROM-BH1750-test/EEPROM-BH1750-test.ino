//Light meter software without pullup resistors using internal pullups.
#include <Wire.h>
#include <BH1750.h>
#include <EEPROM.h>

//EEPROM fixed positions
// 0 -->J
// 1 -->P
//10 --> pointer int eeAddress

int eeAddress;

//13 --> Picture counter int ActualPicture

int ActualPicture;

//15-100 --> empty for future settings auto exposure etc...
//101 --> intial record

// record struct structure
// Picture sequence
// Type (manual, a100,a600, flash etc...)
// ShutterSpeed actual for auto reference/record
// lux measured
 
  struct Picture  {
  int StructPicture;
  byte StructType;
  int StructSpeed;
  uint16_t StructLux;
  };


// testing/simulation buttons / LED
int pushButton = 6;
int dumpButton = 4;
int led = 5;

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
  // make the pushbuttons pin an input:
  pinMode(pushButton, INPUT);
  pinMode(dumpButton, INPUT_PULLUP);
  
  pinMode(led,OUTPUT);

    lightmeter.begin(luxMode); // Inicializar BH1750
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);


    char initJP [2];
    initJP[0] = EEPROM.read(0);
    initJP[1] = EEPROM.read(1);

    if (initJP[0] != 'S' || initJP[1] != 'X')
        {
  Serial.println ("Initializing....");
  EEPROM.write(0,'S');
  EEPROM.write(1,'X');
  int eeAddress = 101;
  EEPROM.put (10,eeAddress);
  int ActualPicture= 1;
  EEPROM.put (13,ActualPicture);
  }
      else 
  {
Serial.println ("NO NEED TO INITIALIZE");
EEPROM.get(10,eeAddress);  // where to write
EEPROM.get (13, ActualPicture); //the picture taken counter
  }

Serial.println ("Running main code...");
}


// the loop routine runs over and over again forever:
void loop() //*************************************************************************************************************************************************************************

{

byte PictureType = 0;
// PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600

int ShutterSpeed = 66;
//uint16_t lux;

uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
/* 

  struct Picture  {
  int StructPicture;
  byte StructType;
  int StructSpeed;
  uint16_t StructLux;
  };*/
Picture MyPicture ={ ActualPicture, PictureType, ShutterSpeed, lux};
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
  //Serial.print("ActualPicture read: ");
  //Serial.println(ActualPicture);
  Serial.println ("-");
  EEPROM.get( eeAddress, MyPicture );  
  Serial.print( " Picture: " );
  Serial.println( MyPicture.StructPicture );
  Serial.print( " Type: " );
  Serial.println( MyPicture.StructType );

  Serial.print( " ShutterSpeed: " );

  Serial.println( MyPicture.StructSpeed );
    Serial.print( " Lux: " );

  Serial.println( MyPicture.StructLux );
Serial.println("****************************************************************");
    
    

    uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
//  Serial.print(F("Check Lux:  "));
//  Serial.print(lux);
//  Serial.println(" lx");
//  Serial.print("ActualPicture = ");
//  Serial.println(ActualPicture);
//  delay(1000);

  eeAddress += sizeof(MyPicture);  //Obtener la siguiente posicion para escribir
  if(eeAddress+sizeof(MyPicture) >= EEPROM.length()) eeAddress = 0;  //Comprobar que no hay desbordamiento

 
 EEPROM.update (10,eeAddress);

 ActualPicture = ActualPicture+1;

 EEPROM.update (13,ActualPicture);

delay (500);
  }

//*************************DUMPING ROUTINE****************************************************************  
  if (digitalRead(dumpButton) == LOW) 
  {

EEPROM.get(10,eeAddress);
int ReadAddress = (eeAddress - (sizeof(MyPicture)*8));

Serial.print("======================= Entering loop =======================");
Serial.print("eeAddress before loop: ");
Serial.println (eeAddress);
  
Serial.print("ReadAddress before loop: ");
Serial.println (ReadAddress);
  
for (int i = 0; i < 8; i++)
{
  int thisRecordAddress = ReadAddress + (i * sizeof(MyPicture));
  int sequence = i+1;
  EEPROM.get(thisRecordAddress, MyPicture);
  Serial.println("=======================================================");
  Serial.print("eeAddress read: ");
  Serial.println (thisRecordAddress);
  Serial.println ("-");
  Serial.print ("Pack order: ");
  Serial.println (sequence);
  Serial.print( " Picture: " );
  Serial.println( MyPicture.StructPicture );
  Serial.print( " Type raw: " );
  Serial.println( MyPicture.StructType );
  // PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600

  Serial.print( " Type: " );
  if (MyPicture.StructType == 0){
  Serial.println( "MANUAL" );  }  
  if (MyPicture.StructType == 1){
  Serial.println( "AUTO 100" ) ; }
  if (MyPicture.StructType == 2){
  Serial.println( "FLASH DONGLELESS" );}
  if (MyPicture.StructType == 4){
  Serial.println( "FLASH F8 DONGLE" ); }
  if (MyPicture.StructType == 6){
  Serial.println( "AUTO 600" );  }
  
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
  
