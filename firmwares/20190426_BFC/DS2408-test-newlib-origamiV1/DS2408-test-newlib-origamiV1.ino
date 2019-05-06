
//****************************************************************************************************************************
//OPTION for origamiV1 dongles

//#define origamiV1 0 //this is for "normal" uDongles

//
#define origamiV1 1 //this is for "origamiV1" dongles, will be fixed in newer origamis.

//suport for origami V1 boards.

//****************************************************************************************************************************

// OneWire and DS2408 STUFF*****************************************************************
#include <DS2408.h>

//OPTION: the pin I connect S2 might be different on reworked or future boards. I want to connect to an ANALOG input so I can have A8-dongle support.

const int S2 = 2;  //this for Flash insertion detection
//this CLOSED when there is a FLASHBAR inserted

//const int S2 = A4;  //this for Flash insertion detection for reworked board


#define ONE_WIRE_BUS_PORT S2
DS2408 ds(ONE_WIRE_BUS_PORT);
Device dongleDevice;
uint8_t device_count = 0;
//
uint8_t readDevice;

byte selector ;
bool switch1 ;
bool switch2 ;

// END OF OneWire and DS2408 STUFF*****************************************************************

void initializeDS2408();
byte Read_DS2408_PIO(int Slot);
void Write_DS2408_PIO(byte port, bool ON);


void setup() {

  Serial.begin (9600);
  Serial.println ("hello openSX70");

  void intializeDS2408();

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  int old_device_count = device_count;
  device_count = ds.findsingle(&dongleDevice);
  if (device_count != 0 && old_device_count == 0) {
    //OPTION this is only to blink the LED on the dongle if inserted late. Only the first time.
    initializeDS2408(); //Pitsie fix for non-blinking LED on dongle
    Write_DS2408_PIO (6, 1);
    delay (200);
    Write_DS2408_PIO (6, 0);
  }

  if (device_count != 0) {
    // READ DS2408

    selector = Read_DS2408_PIO(0);
    switch1 = Read_DS2408_PIO(1);
    switch2 = Read_DS2408_PIO(2);


  }

Serial.println ("                                            testing for origamiV1");

  if (Read_DS2408_PIO(0) == 100)
  {
    Serial.println ("FLASH");
    return;
  }
  if (Read_DS2408_PIO(0) == 200)
  {
    Serial.println ("NOTHING");
    return;
  }

  if (Read_DS2408_PIO(0) < 100)
  {
    byte ActualSlot = Read_DS2408_PIO(0);
    Serial.print ("Selector: ");
    Serial.println (ActualSlot);

    byte S1 = Read_DS2408_PIO(1);
    Serial.print ("S1: ");
    Serial.println (S1);

    byte S2 = Read_DS2408_PIO(2);
    Serial.print ("S2: ");
    Serial.println (S2);
    return;


  }
}
