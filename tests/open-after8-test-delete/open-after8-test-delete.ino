//#include <OneWire.h>
#include <DS2408.h>

#define BIT(x, y) (x & (1 << y))
#define ONE_WIRE_BUS_PORT selectorPin
//#define uint8_t device_count = ds.find(&devices);

const int buttonPin = 12;     // the number of the pushbutton pin
const int flashPin =  11;      // firing the flash through the Mosfet
const int selectorPin = A0;    // detects if a flash is inserted (FFC mode "1")
byte SelectorAfter8();

DS2408 ds(ONE_WIRE_BUS_PORT);

Devices devices;
uint8_t device_count;


//******************************************************************************************************

void setup() {


// initialize the LED pin as an output:

  pinMode(flashPin, OUTPUT);           // make sure I don't fire the flash!
  digitalWrite(flashPin, LOW);
//  pinMode(selectorPin, INPUT_PULLUP);  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
//    pinMode(selectorPin, OUTPUT); 
//    pinMode(selectorPin,LOW);

  Serial.begin(9600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX70!");

 device_count = ds.find(&devices);

                                                                                                 
} //        end of void setup()
//****************************************************************************************************** 

void loop() {

//    pinMode(selectorPin, OUTPUT); 
//    pinMode(selectorPin,LOW);
//    pinMode(selectorPin, INPUT); 
//    pinMode(selectorPin, INPUT); 
    
    
    Serial.print ("digitalRead: ");
    byte dRead = digitalRead(selectorPin);
    Serial.println (dRead);

    Serial.print ("                       analogRead: ");
    byte aRead = analogRead(selectorPin);
    Serial.println (aRead);

  device_count = ds.find(&devices);

    Serial.print ("                                               device_count: ");
    Serial.println (device_count);
  return;

    if (  (device_count == 0) && (aRead < 350) && (dRead == 0)  )
    {
    Serial.println ("----------->NOTHING INSERTED");     
    }
    if (  (device_count == 1) && (aRead < 150) && (dRead == 1)  )
    {
    Serial.println ("----------->openSX70 dongle");     
    }
    
    

   pinMode(selectorPin, INPUT_PULLUP);  // initialize the pushbutton pin as an input with pullup
   
  if (digitalRead(selectorPin) == LOW)  {  //////////////////////////////////////////////////////////// CASE FLASH
    Serial.println ("FLASH INSERTED");

                  if (digitalRead(buttonPin) == LOW) {
                      Serial.println("Button pressed: firing flash");
                      digitalWrite(flashPin, HIGH);
                      delay (15);
                      digitalWrite(flashPin, LOW);
                                                      }
    delay (1000);
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

  Serial.print ("openSX70 selector: ");
  Serial.println (selector);

  Serial.print("switch 1: ");
  Serial.println(S1, BIN);
 
  Serial.print("switch 2: ");
  Serial.println(S2, BIN);

  return;
  } // END OF if ((digitalRead(selectorPin) == HIGH) && (device_count > 0)){  //CASE DONGLE INSERTED


    device_count = ds.find(&devices);
    // pinMode(selectorPin, OUTPUT); 
    //

    
  //pinMode(selectorPin, INPUT);  // initialize the pushbutton pin as an input:  

//  if  ((device_count == 0) && (digitalRead(selectorPin) == HIGH)) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED

  if  ((device_count == 0) != ((analogRead(selectorPin) <= 1023)|(analogRead(selectorPin) >= 900)))
  {
    Serial.print ("after8 selector: ");
    Serial.println (SelectorAfter8());
    return;
    
  } else {
   
   Serial.println("NOTHING CONNECTED");
   delay (1000);
   return;

  } // END OF if  ((device_count == 0) && (digitalRead(selectorPin) == HIGH)) {  //CASE NOTHING CONNECTED


 

} // END OF loop
//******************************************************************************************************
byte SelectorAfter8()
{
  int j = 1; // integer used in scanning the array designating column number
//2-dimensional array for asigning the buttons and there high and low values
/*
 * TEST for After8 dongle
 * R100 = 50K  (2) --> S2-
 * R101 = 120K (8) --> S2-
 * R102 = 100K (1) --> S2-
 * R103 = 196K (4) --> S2-
 * 
 * Common R105 = 10K (S2+ --> VCC)
 */
int SelectorPos[16][3] = {{0, 990,1023},  // Pos 0  --> 2000
                          {1, 940,950},   // Pos 1  --> OFF
                          {2, 970,980},   // Pos 2  --> A100
                          {3, 900,910},   // Pos 3  --> A600
                          {4, 850,860},   // Pos 4  --> T
                          {5, 795,810},   // Pos 5  --> B
                          {6, 815,830},   // Pos 6  --> 2
                          {7, 768,780},   // Pos 7  --> 4
                          {8, 925,935},   // Pos 8  --> 8
                          {9, 860,870},   // Pos 9  --> 15
                          {10, 885,895},  // Pos A  --> 30
                          {11, 825,835},  // Pos B  --> 60
                          {12, 785,795},  // Pos C  --> 125
                          {13, 740,750},  // Pos D  --> 250
                          {14, 755,767},  // Pos E  --> 500
                          {15, 710,720}}; // Pos F  --> 1000


 // loop for scanning the button array.
 for(int i = 0; i <= 15; i++)
 {
      int j = 1;
   // checks the ButtonVal against the high and low vales in the array
   if( (analogRead (selectorPin)) >= SelectorPos[i][j] &&  (analogRead (selectorPin)) <= SelectorPos[i][j+1])
   {
     byte label = SelectorPos[i][0];
     return label;
   }
 }

}
