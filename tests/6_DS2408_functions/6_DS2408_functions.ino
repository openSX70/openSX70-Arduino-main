#define origamiV1 0

#include <DS2408.h>
//Based on DS2408/Arduino/lightLed/lightLed.ino

// END OF OneWire and DS2408 STUFF*****************************************************************


const int S2 = 2;  //this for Flash insertion detection
                      //this CLOSED when there is a FLASHBAR inserted

//const int S2 = A4;  //this for Flash insertion detection for reworked board

#define ONE_WIRE_BUS_PORT S2


//DS2408 ds(ONE_WIRE_BUS_PORT);
//Read_DS2408_PIO ds(ONE_WIRE_BUS_PORT);

//Devices devices;
//uint8_t device_count;

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
uint8_t device_count;

/*
#ifdef CXA_PURE_VIRTUAL_FIX
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) { while(1); }
#endif
*/
//******************************************************************************************************

void setup() {


// initialize the LED pin as an output:

  pinMode(S2, INPUT_PULLUP);  // initialize the pushbutton pin as an input:

  Serial.begin(9600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX70!");

// device_count = ds.find(&devices);
// Serial.print ("device_count = ");
// Serial.print (device_count);

intializeDS2408();
}

void loop() {

device_count = ds.find(&devices);

Serial.print ("LOOP: device_count = ds.find(&devices);--->");
Serial.println (device_count);
if (device_count == 0)
{
//device_count = ds.find(&devices);
//#if SIMPLEDEBUG

delay (1000);
//#endif
          Write_DS2408_PIO (6, 1);
          delay (200);
         Write_DS2408_PIO (6, 0);
#if SIMPLEDEBUG
Serial.println ("BLINK after DS2408 init in loop");
#endif
}


  // TEST Write_DS2408_PIO function!
  //***********************************************************************

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
  else
  {
    Serial.println ("DONT KNOW ");
  }
  //  int Read_DS2408_PIO();
  return;

} // END OF loop


//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                DS2408 FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************
/*

//******************************************************************************************************
*/

byte Read_DS2408_PIO(int Slot) {
   // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
   // Slot = 1 S1
   // Slot = 2 S2
     
  byte selector = B0000;
  uint8_t readDevice = ds.get_state(devices[0]);
  
//  intializeDS2408();
  
  if  ((device_count == 0) && (digitalRead(S2) == HIGH))   ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED
    {
    Serial.println("NOTHING CONNECTED!!!");
   return 200;
    }
  
  if ((digitalRead(S2) == HIGH) && (readDevice > 0)) { ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

    // BIT: 7 6 5 4 3 2 1 0
    if (Slot == 0) {

      uint8_t readDevice = ds.get_state(devices[0]);

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

    byte Switch;
    if (Slot == 1) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00010000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    }
    if (Slot == 2) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00100000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    } else
      return 0;

  } // END OF if ((digitalRead(S2) == HIGH) && (device_count > 0)){  //CASE DONGLE INSERTED

pinMode(S2, INPUT_PULLUP); // perhaps not such a good idea internal pullup??

  if (digitalRead(S2) == LOW)  { 
    //////////////////////////////////////////////////////////// CASE FLASH
    return 100; // FLASH

  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH

  return 200; //NOTHING

} //END OF Read_DS2408_PIO() function

//******************************************************************************************************
byte Write_DS2408_PIO(byte port, bool ON) {
// This is to turn on LED P6 or PC-FLASH socket (opto) P7
// port 6 = P6 = LED
// port 7 = P7 = PC-FLASHM
  byte OutPIO = B00000000;
  
  if (ON == true )
  {
    uint8_t readDevice = ds.get_state(devices[0]);
    bitSet(OutPIO, port);
    ds.set_state(devices[0], ~OutPIO);
    return;
  } else {
    uint8_t readDevice = ds.get_state(devices[0]);
    bitClear(OutPIO, port);
    ds.set_state(devices[0], ~OutPIO);
    return;
  }
} //END OF Write_DS2408_PIO
//******************************************************************************************************

void intializeDS2408()
{
  //INTITIALIZE DS2408
//device_count = ds.find(&devices);
ds.reset();
ds.write(0x96);
for (int i = 0; i < 8; i++)
    ds.write(devices[0][i]);
ds.write(0x3C);
ds.reset(); 
#if SIMPLEDEBUG
Serial.print ("SETUP: device_count = ds.find(&devices);--->");
Serial.println (device_count);
#endif
// THIS IS FUNDAMENTAL
}
