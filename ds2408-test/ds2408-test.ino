//Based on DS2408/Arduino/lightLed/lightLed.ino
#include <DS2408.h>

//#define BIT(x, y) (x & (1 << y))
#define ONE_WIRE_BUS_PORT selectorPin
//#define uint8_t device_count = ds.find(&devices);

#define ISDEBUG 1

const int buttonPin = 2;     // the number of the pushbutton pin
const int flashPin =  9;      // firing the flash through the Mosfet
const int selectorPin = 12;    // detects if a flash is inserted (FFC mode "1")

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
uint8_t device_count;

byte OutPIO = B00000000;
byte S1 = B0; 
byte S2 = B0; 
byte selector = B0000;

#if ISDEBUG

long cycle;

#endif


//******************************************************************************************************

void setup() {


// initialize the LED pin as an output:

  pinMode(flashPin, OUTPUT);           // make sure I don't fire the flash!
  digitalWrite(flashPin, LOW);
  pinMode(selectorPin, INPUT_PULLUP);  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  #if ISDEBUG 
  Serial.begin(57600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX70!");
  #endif

 device_count = ds.find(&devices);
 Serial.begin(57600);                  // set up Serial library at 9600 bps

Serial.println ("hello");                                                                                                 
} //        end of void setup()


//****************************************************************************************************** 

void loop() {
Serial.println ("HELLO");                                                                                                 
  
  
/*
 //***********************************************************************
 while (digitalRead(buttonPin) == LOW) {
      uint8_t readDevice = ds.get_state(devices[0]);
       bitSet(OutPIO, 6);
       ds.set_state(devices[0], ~OutPIO); 
        }
        {
      uint8_t readDevice = ds.get_state(devices[0]);
       bitClear(OutPIO, 6);
       ds.set_state(devices[0], ~OutPIO); 
        }
 //***********************************************************************

 #if ISDEBUG 
    cycle = cycle+1;
    Serial.print ("                        CYCLE:");
    Serial.println(cycle);
  #endif
 
  uint8_t readDevice = ds.get_state(devices[0]);
  #if ISDEBUG 
   Serial.print ("---------------------------------------------->");
   Serial.println (readDevice, BIN);
  #endif

// Was fLASH
    
if ((digitalRead(selectorPin) == HIGH) && (readDevice > 0)){  ////////////////////////////////////////////////////////////CASE DONGLE INSERTED
  #if ISDEBUG 
  Serial.println("openSX70 Dongle connected!");
  #endif

// BIT: 7 6 5 4 3 2 1 0

     uint8_t readDevice = ds.get_state(devices[0]);
      
      if (readDevice & 0b00000001) {
                bitSet(selector, 0);
                  } else bitClear(selector, 0);
      if (readDevice & 0b00000010) {
                bitSet(selector, 1);
                  }else bitClear(selector, 1);
      if (readDevice & 0b00000100) {
                bitSet(selector, 2);
                  }else bitClear(selector, 2);
      if (readDevice & 0b000001000) {
                bitSet(selector, 3);
                  }else bitClear(selector, 3);
                        
      if (readDevice & 0b00010000) {
                bitSet(S1, 0);/*
                ds.set_state(devices[0], ~0b01000000); 
                } else {
                ds.set_state(devices[0], ~0b00000000);
                 *//*
                 }else bitClear(S1, 0);
                 
      if (readDevice & 0b00100000) {
                bitSet(S2, 0);
                }else bitClear(S2, 0);


/*
if (S1 && S2) {
  bitSet(OutPIO, 7);
  bitSet(OutPIO, 6);
  ds.set_state(devices[0], ~OutPIO); 
    #if ISDEBUG 
  Serial.print ("                  P7-P6: ");
  Serial.println(OutPIO,BIN);
  #endif
  //return;
}
// P7 = PC-FLASH
// P7 ON = B10000000
// P6 = RED LED
// P6 ON = B01000000

if (S1) {
  bitSet(OutPIO, 7);
  bitClear(OutPIO,6);
  ds.set_state(devices[0], ~OutPIO); 
   #if ISDEBUG 
  Serial.print ("                     P7: ");
  Serial.println(OutPIO,BIN);
  #endif
  //return;
}
if (S2) {
  bitSet(OutPIO, 6);
  bitClear(OutPIO,7);
  ds.set_state(devices[0], ~OutPIO); 
   #if ISDEBUG 
  Serial.print ("                     P6: ");
  Serial.println(OutPIO,BIN);
  #endif
  //return;
}
//LED ON: ds.set_state(devices[0], ~0b01000000); 
//PC FLASH ON: ds.set_state(devices[0], ~0b10000000); 
/*   if (BIT(pos, 3)){
  bitSet(selector, 3);
                  }
                 
*/               
/* ////////////////////////////////////////////////////// 
   
//                } // END OF for(int index=0; index < device_count; index++) {


    #if ISDEBUG 
  Serial.print ("selector: ");
  Serial.println (selector);

  Serial.print("switch 1: ");
  Serial.println(S1, BIN);
 
  Serial.print("switch 2: ");
  Serial.println(S2, BIN);
  #endif
  return;
  } // END OF if ((digitalRead(selectorPin) == HIGH) && (device_count > 0)){  //CASE DONGLE INSERTED

  
readDevice = ds.get_state(devices[0]);

pinMode(selectorPin, INPUT_PULLUP); // perhaps not such a good idea internal pullup??

 
  if  ((digitalRead(selectorPin) == HIGH) && (readDevice == 0)  ) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED 
      #if ISDEBUG 
      Serial.println("NOTHING CONNECTED");
      delay (1000);
      #endif
   return;
  } // END OF if  ((digitalRead(selectorPin) == HIGH) && (readDevice == 0)  ) { //CASE NOTHING CONNECTED


  if (digitalRead(selectorPin) == LOW)  {  //////////////////////////////////////////////////////////// CASE FLASH
      #if ISDEBUG 
    Serial.println ("FLASH INSERTED");
    #endif
                  if (digitalRead(buttonPin) == LOW) {
                        #if ISDEBUG 
                      Serial.println("Button pressed: firing flash");
                      #endif
                      digitalWrite(flashPin, HIGH);
                      delay (15);
                      digitalWrite(flashPin, LOW);
                                                      }
     #if ISDEBUG 
     delay (500);
     #endif
    return;
        
  } // END OF if (digitalRead(selectorPin) == LOW)  {  // CASE FLASH

*/  

} // END OF loop
//******************************************************************************************************

