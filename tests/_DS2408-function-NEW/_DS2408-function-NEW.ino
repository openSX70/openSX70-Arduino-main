#define origamiV1 0

//
#include <OneWire.h>
#include <DS2408.h>

#define BIT(x, y) (x & (1 << y))
#define ONE_WIRE_BUS_PORT S2
//#define uint8_t device_count = ds.find(&devices);

//const int buttonPin = 12;     // the number of the pushbutton pin
//const int flashPin =  11;      // firing the flash through the Mosfet
const int S2 = 2;    // detects if a flash is inserted (FFC mode "1")

//const int S2 = A4;    // detects if a flash is inserted (FFC mode "1")

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
uint8_t device_count;


//******************************************************************************************************

void setup() {


// initialize the LED pin as an output:

  //pinMode(flashPin, OUTPUT);           // make sure I don't fire the flash!
  //digitalWrite(flashPin, LOW);
  pinMode(S2, INPUT_PULLUP);  // initialize the pushbutton pin as an input:
  //pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX70!");

 device_count = ds.find(&devices);
Serial.print ("device_count = ");
Serial.print (device_count);
}







byte Read_DS2408_PIO(int Slot) {

uint8_t device_count = ds.find(&devices);

  if (digitalRead(S2) == LOW)  {  //////////////////////////////////////////////////////////// CASE FLASH
    Serial.println ("FLASH INSERTED");

    return 100;
        
  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH
   
  device_count = ds.find(&devices);

if ((digitalRead(S2) == HIGH) && (device_count > 0)){  ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

Serial.println("openSX70 Dongle connected!");

// BIT: 7 6 5 4 3 2 1 0

byte S1 = B0000; 
byte S2 = B0000; 
byte selector = B0000;
/*

#if !origamiV1      

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
//    } //END OF Slot=0

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
//    } else
//      return 0;
#endif

#if origamiV1

        uint8_t readDevice = ds.get_state(devices[0]);


      if (readDevice & 0b00000001) {
        bitSet(selector, 3);
      } else bitClear(selector, 3);
      if (readDevice & 0b00000010) {
        bitSet(selector, 2);
      } else bitClear(selector, 2);
      if (readDevice & 0b00000100) {
        bitSet(selector, 1);
      } else bitClear(selector, 1);
      if (readDevice & 0b000001000) {
        bitSet(selector, 0);
      } else bitClear(selector, 0);
      return selector;

    byte Switch;
    if (Slot == 2) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00010000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    }
    if (Slot == 1) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00100000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    } else
      return 0;
#endif
*/


#if origamiV1
 for(int index=0; index < device_count; index++) {
      uint8_t readDevice = ds.get_state(devices[index]);
      if (readDevice & 0b00000001) {
                bitSet(selector, 3);
                  }
      
      if (readDevice & 0b00000010) {
                bitSet(selector, 2);
                  }
      if (readDevice & 0b00000100) {
                bitSet(selector, 1);
                  }
      if (readDevice & 0b000001000) {
                bitSet(selector, 0);
                  }
#endif
#if !origamiV1
 for(int index=0; index < device_count; index++) {
      uint8_t readDevice = ds.get_state(devices[index]);
      if (readDevice & 0b00000001) {
                bitSet(selector, 0);
                  }
      
      if (readDevice & 0b00000010) {
                bitSet(selector, 1);
                  }
      if (readDevice & 0b00000100) {
                bitSet(selector, 2);
                  }
      if (readDevice & 0b000001000) {
                bitSet(selector, 3);
                  }
                        
      if (readDevice & 0b00010000) {
                bitSet(S1, 0);
               //ds.set_state(devices[index], ~0b01000000); 
                ds.set_state(devices[index], ~0b01000000); 
                } else {
                ds.set_state(devices[index], ~0b00000000);
                //ds.set_state(devices[index], ~0b11111111);  
                 }

      if (readDevice & 0b00100000) {
                bitSet(S2, 0);
                }
                }

#endif


#if origamiV1
     if (readDevice & 0b00010000) {
                bitSet(S2, 0);
               //ds.set_state(devices[index], ~0b01000000); 
                ds.set_state(devices[index], ~0b01000000); 
                } else {
                ds.set_state(devices[index], ~0b00000000);
                //ds.set_state(devices[index], ~0b11111111);  
                 }

      if (readDevice & 0b00100000) {
                bitSet(S1, 0);
                }
                }
#endif

  return;
  } // END OF if ((digitalRead(S2) == HIGH) && (device_count > 0)){  //CASE DONGLE INSERTED


    device_count = ds.find(&devices);
  
  if  ((device_count == 0) && (digitalRead(S2) == HIGH)) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED

   Serial.println("NOTHING CONNECTED");
   return 200;
  } // END OF if  ((device_count == 0) && (digitalRead(S2) == HIGH)) {  //CASE NOTHING CONNECTED

}
