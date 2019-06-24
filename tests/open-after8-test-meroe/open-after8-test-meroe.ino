//#include <OneWire.h>
#include <DS2408.h>

#define BIT(x, y) (x & (1 << y))
#define ONE_WIRE_BUS_PORT selectorPin
//#define uint8_t device_count = ds.find(&devices);

const int buttonPin = 12;     // the number of the pushbutton pin
const int flashPin =  11;      // firing the flash through the Mosfet
const int selectorPin = A4;    // detects if a flash is inserted (FFC mode "1")
byte SelectorAfter8();
int previousRead = 0;
    
    DS2408 ds(ONE_WIRE_BUS_PORT);
    
    Devices devices;
    uint8_t device_count;
    
    
    //******************************************************************************************************
    
    void setup() {
    
    
    // initialize the LED pin as an output:
    
      pinMode(flashPin, OUTPUT);           // make sure I don't fire the flash!
      digitalWrite(flashPin, LOW);
        Serial.print ("                       analogRead: ");
        byte aRead = analogRead(selectorPin);
        Serial.println (aRead);
    
      pinMode(buttonPin, INPUT_PULLUP);
      //pinMode(buttonPin, INPUT);

  Serial.begin(9600);                  // set up Serial library at 9600 bps
  Serial.println("HELLO openSX70!");

 device_count = ds.find(&devices);

                                                                                                 
} //        end of void setup()
//****************************************************************************************************** 

void loop() {

 //   pinMode(selectorPin,LOW);

 //   pinMode(selectorPin, INPUT); 
    
//    Serial.print ("AFTER8 INSERTED "); 
    Serial.println (analogRead(selectorPin));

  device_count = ds.find(&devices);

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

  
  if  ((device_count == 0) && (analogRead(selectorPin) >= 700))
  {
    Serial.print ("previousRead: ");
    Serial.println (previousRead);    
    Serial.print ("after8 selector: ");
    Serial.println (SelectorAfter8());
    
  } else {
   
   Serial.println("NOTHING CONNECTED");
   delay (1000);
   return;

  } // END OF if  ((device_count == 0) && (digitalRead(selectorPin) == HIGH)) {  //CASE NOTHING CONNECTED


 

} // END OF loop
//******************************************************************************************************
byte SelectorAfter8()
{
  byte label;
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
int SelectorPos[16][3] = {{0, 1022,1023},  // Pos 0  --> 2000
                          {1, 980,995},   // Pos 1  --> OFF
                          {2, 1010,1021},   // Pos 2  --> A100
                          {3, 940,950},   //  Pos 3  --> A600
                          {4, 890,904},   //  Pos 4  --> T
                          {5, 835,850},   //  Pos 5  --> B
                          {6, 855,870},   //  Pos 6  --> 2
                          {7, 808,820},   //  Pos 7  --> 4
                          {8, 965,975},   //  Pos 8  --> 8
                          {9, 905,915},   //  Pos 9  --> 15
                          {10, 925,935},  //  Pos A  --> 30
                          {11, 865,885},  //  Pos B  --> 60
                          {12, 825,835},  //  Pos C  --> 125
                          {13, 780,785},  //  Pos D  --> 250
                          {14, 795,805},  //  Pos E  --> 500
                          {15, 750,770}}; //  Pos F  --> 1000 


 // loop for scanning the button array.
 for(int i = 0; i <= 15; i++)
 {
      int j = 1;
   // checks the ButtonVal against the high and low vales in the array
   if( (analogRead (selectorPin)) >= SelectorPos[i][j] &&  (analogRead (selectorPin)) <= SelectorPos[i][j+1])
   {
    label = SelectorPos[i][0];

   }
 }
 /*
 Serial.print ("label: ");
 Serial.println (label);
 Serial.print ("previousRead: ");
 Serial.println(previousRead);
 */
 if ((label == 0) && ((previousRead != 1) || (previousRead != 15)))
    {
    label = 200;
       
    } else
          {
          
          previousRead = label ;
          //label = 200;
          }

          return label;    

}
