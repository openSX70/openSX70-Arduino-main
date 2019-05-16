//#include <OneWire.h>
#include <DS2408.h>

#define BIT(x, y) (x & (1 << y))
#define ONE_WIRE_BUS_PORT selectorPin
//#define uint8_t device_count = ds.find(&devices);

const int buttonPin = 12;     // the number of the pushbutton pin
const int flashPin =  11;      // firing the flash through the Mosfet
const int selectorPin = 2;    // detects if a flash is inserted (FFC mode "1")

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
uint8_t device_count;


//******************************************************************************************************

void setup() {


// initialize the LED pin as an output:

  pinMode(flashPin, OUTPUT);           // make sure I don't fire the flash!
  digitalWrite(flashPin, LOW);
  pinMode(selectorPin, INPUT_PULLUP);  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX79!");

 //device_count = ds.find(&devices);

                                                                                                 
} //        end of void setup()
//****************************************************************************************************** 

void loop() {

  device_count = ds.find(&devices);
 // Serial.print ("------------------------->");
 // Serial.println (device_count);

  if (digitalRead(selectorPin) == LOW)  {  //////////////////////////////////////////////////////////// CASE FLASH
    Serial.println ("FLASH INSERTED");

                  if (digitalRead(buttonPin) == LOW) {
                      Serial.println("Button pressed: firing flash");
                      digitalWrite(flashPin, HIGH);
                      delay (15);
                      digitalWrite(flashPin, LOW);
                                                      }
    //delay (1000);
    return;
        
  } // END OF if (digitalRead(selectorPin) == LOW)  {  // CASE FLASH
   
  device_count = ds.find(&devices);

if ((digitalRead(selectorPin) == HIGH) && (device_count > 0)){  ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

Serial.println("openSX70 Dongle connected!");

// BIT: 7 6 5 4 3 2 1 0

byte S1 = B0000; 
byte S2 = B0000; 
byte selector = B0000;


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

  Serial.print ("selector: ");
  Serial.println (selector);

  Serial.print("switch 1: ");
  Serial.println(S1, BIN);
 
  Serial.print("switch 2: ");
  Serial.println(S2, BIN);

  return;
  } // END OF if ((digitalRead(selectorPin) == HIGH) && (device_count > 0)){  //CASE DONGLE INSERTED


    device_count = ds.find(&devices);
  
  if  ((device_count == 0) && (digitalRead(selectorPin) == HIGH)) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED

   Serial.println("NOTHING CONNECTED");
   //delay (1000);
   return;
  } // END OF if  ((device_count == 0) && (digitalRead(selectorPin) == HIGH)) {  //CASE NOTHING CONNECTED

} // END OF loop
//******************************************************************************************************
