#define origamiV1 0

//#include <OneWire.h>
#include <DS2408.h>

//#define BIT(x, y) (x & (1 << y))
#define ONE_WIRE_BUS_PORT S2
#define uint8_t device_count = ds.find(&devices);

const int buttonPin = 12;     // the number of the pushbutton pin
const int flashPin =  11;      // firing the flash through the Mosfet
const int S2 = 2;    // detects if a flash is inserted (FFC mode "1")

//const int S2 = A4;    // detects if a flash is inserted (FFC mode "1")

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
//uint8_t device_count;

#ifdef CXA_PURE_VIRTUAL_FIX
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) { while(1); }
#endif
//******************************************************************************************************

void setup() {


// initialize the LED pin as an output:

  //pinMode(flashPin, OUTPUT);           // make sure I don't fire the flash!
  //digitalWrite(flashPin, LOW);
  pinMode(S2, INPUT_PULLUP);  // initialize the pushbutton pin as an input:
  //pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX70!");

// device_count = ds.find(&devices);
// Serial.print ("device_count = ");
// Serial.print (device_count);
}






//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                DS2408 FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************

byte Read_DS2408_PIO(int Slot) {

   // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
   // Slot = 1 S1
   // Slot = 2 S2

uint8_t device_count = ds.find(&devices);

//Serial.print ("                                                                               device_count=");
//Serial.println(device_count);

  if (digitalRead(S2) == LOW)  {  //////////////////////////////////////////////////////////// CASE FLASH
//    Serial.println ("FLASH INSERTED");

    return 100;
        
  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH


  
//  device_count = ds.find(&devices);

    
  byte selector = B0000;
  uint8_t readDevice = ds.get_state(devices[0]);
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

  if  ((device_count == 0) && (digitalRead(S2) == HIGH))   ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED
    {
    Serial.println("NOTHING CONNECTED!!!");
   return 200;
    }

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
