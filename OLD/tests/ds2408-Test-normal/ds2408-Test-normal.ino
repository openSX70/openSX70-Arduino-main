#define origamiV1 0

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

uint8_t devices;

//******************************************************************************************************


#if !origamiV1
//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                DS2408 FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------


byte Read_DS2408_PIO(int Slot) {


  // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
  // Slot = 1 S1          //Return switch 1 on or off
  // Slot = 2 S2         //Return switch 2 on or off

  byte selector = B0000;
  //Serial.print ("readDevice = ");
  //Serial.println (readDevice, HEX);


  if  (((device_count == 0)) && (digitalRead(S2) == HIGH))   ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED
  {
    return 200;
  }


  if ((digitalRead(S2) == HIGH) && ((device_count > 0))  ) { ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

    // BIT: 7 6 5 4 3 2 1 0


    if (Slot == 0) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00000001) {
        bitSet(selector, 0);
      } else bitClear(selector, 0);
      if (readDevice & 0b00000010) {
        bitSet(selector, 1);
      } else bitClear(selector, 1);
      if (readDevice & 0b00000100) {
        bitSet(selector, 2);
      } else bitClear(selector, 2);
      if (readDevice & 0b000001000) {
        bitSet(selector, 3);
      } else bitClear(selector, 3);
      return selector;
    } //END OF Slot=0

    byte Switch = 0;
    if (Slot == 1) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00010000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    }
    if (Slot == 2) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00100000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    } else
      return 0;

  } // END OF if ((digitalRead(S2) == HIGH) && (ds.find(&devices); > 0)){  //CASE DONGLE INSERTED

  //pinMode(S2, INPUT_PULLUP); // perhaps not such a good idea internal pullup??

  //   readDevice = ds.get_state(devices[0]);
  if (digitalRead(S2) == LOW)  {
    //////////////////////////////////////////////////////////// CASE FLASH
    return 100; // FLASH

  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH

  if  ((device_count == 0) && (digitalRead(S2) == HIGH)) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED

    //   Serial.println("NOTHING CONNECTED");
    //delay (1000);
    return 200; //NOTHING

  }

  return 200;
} //////////////////////////////////END OF Read_DS2408_PIO() function




//******************************************************************************************************
void Write_DS2408_PIO(byte port, bool ON) {
  // This is to turn on LED P6 or PC-FLASH socket (opto) P7
  // port 6 = P6 = LED
  // port 7 = P7 = PC-FLASHM
  byte OutPIO = B00000000;

  if (ON == true )
  {
    uint8_t readDevice = ds.get_state(dongleDevice);
    bitSet(OutPIO, port);
    ds.set_state(dongleDevice, ~OutPIO);
    return;
  } else {
    uint8_t readDevice = ds.get_state(dongleDevice);
    bitClear(OutPIO, port);
    ds.set_state(dongleDevice, ~OutPIO);
    return;
  }
} //END OF Write_DS2408_PIO
//******************************************************************************************************




void initializeDS2408()
{
  //INTITIALIZE DS2408
  //ds.find(&devices); = ds.find(&devices);
  ds.reset();
  ds.write(0x96);
  for (int i = 0; i < 8; i++)
    ds.write(dongleDevice[i]);
  ds.write(0x3C);
  ds.reset();
  // THIS IS FUNDAMENTAL
}
#endif



void setup() {


  // initialize the LED pin as an output:

  pinMode(S2, INPUT_PULLUP);  // initialize the pushbutton pin as an input:

  Serial.begin(9600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX70!");


} //        end of void setup()
//******************************************************************************************************

void loop() {

  Serial.println ("testing for uDongle");
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
/*
  if (selector == 15) {
  Serial.println ("workaround");
  uint8_t readDevice = ds.get_state(devices[0]);
  //workaround
  selector = Read_DS2408_PIO(0);
  switch1 = Read_DS2408_PIO(1);
  switch2 = Read_DS2408_PIO(2);
  }
*/
