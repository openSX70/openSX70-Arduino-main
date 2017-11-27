  // self timer code
  // by Joaquín de Prada
  // based in lots of stuff, like blink without delay
  // has lots of debug messages
#include <DS2408.h>

const int S2 = 10;


#define ONE_WIRE_BUS_PORT S2
DS2408 ds(ONE_WIRE_BUS_PORT);
Devices devices;
uint8_t device_count;


  // first I define what is considered a longPress and a shortPress
  
  // shortPress is when you want to take a "regular" picture
  // it needs to be timed for debounce purposes, that is, you need a "solid" press to take a picture
  const int shortPress = 200;
  //longPress is when you want to "something else", in my case delay the taking of the picture for x (10) seconds.
  //since 1000ms = 1 seconds, this is just a bit more than 1 second.
  const int  longPress = 1200;

  long LOOPresult;


void setup() {
  // put your setup code here, to run once:
  // this is for debug messages
    Serial.begin(57600) ;
    pinMode(S2, INPUT_PULLUP);
    device_count = ds.find(&devices);
    WritePIO(6,0);

}

void loop() {
    
  // put your main code here, to run repeatedly:
  // I try loop to be very simple and that looking at it you can understand what is happening
         LOOPresult = REDbutton(6);
            if ((LOOPresult > shortPress) && (LOOPresult < longPress)) {
            Serial.println("---------------------------");
            Serial.println ("CLICK");
            Serial.println("---------------------------");
            //blink (1000,10,5);
            } else if (LOOPresult > longPress) {
            BeepTest();
            Serial.println("---------------------------");
            Serial.println ("...CLICK");
            Serial.println("---------------------------");
            
            } 
}

void BeepTest() {
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay() 
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.
unsigned long startTimer = millis();
Serial.println ("START 500,2600,5,0");

//*******************************************************
//Piezo
//BlinkBeep (500,2600,5,0);

// arduino LED
//BlinkBeep (1000,2500,5,1);

//Serial.println ("START 300,2400,5,0");
//Piezo
//BlinkBeep (300,2400,5,0);

// DS2408 LED
BlinkBeep (1000,2500,6,2);

//********************************************************
// arduino LED
//BlinkBeep (600,2500,5,1);

//Serial.println ("START 200,2400,5,0");
//Piezo
//BlinkBeep (200,2400,5,0);

// DS2408 LED
BlinkBeep (600,2500,6,2);


// arduino LED
//BlinkBeep (200,2500,5,1);

//Serial.println ("START 70,1400,5,0");
//Piezo
//BlinkBeep (70,1400,5,0);

// DS2408 LED
BlinkBeep (200,2500,6,2);

// arduino LED
//BlinkBeep (80,2500,5,1);

// DS2408 LED
BlinkBeep (80,2500,6,2);


//Serial.println ("START 1200,1200,5,0");
//Piezo
//BlinkBeep (1200,1200,5,0);
;
Serial.println ("END");

unsigned long endTimer = millis();

unsigned long time = ((endTimer-startTimer)/1000);
;
Serial.println ("===========================");
Serial.print ("TIME:   ");
Serial.print(time);
Serial.println ("  seconds  ");
Serial.println ("===========================");

}

//************************************************************************************************************

int BlinkBeep (int interval, int timer, int Pin, byte type)
// blink (blink interval=blinking speed, timer=duration blinking, Pin=pin of LED 
//type 1 = LED 
//type 0 = Piezo 
//type 2 = DS2480
// blink is a standalone function
{

int ledState = LOW;             // ledState used to set the LED
pinMode(Pin, OUTPUT);
  
unsigned long previousMillis = 0;        // will store last time LED was updated

unsigned long currentMillisTimer = millis();
  
  while (millis() < (currentMillisTimer + timer)) 
  
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == 0) {
      ledState = 1;
    } else {
      ledState = 0;
    }


    // set the LED with the ledState of the variable:
    //gitalWrite(Pin, ledState);
  if (type == 1) {
        digitalWrite (Pin, ledState);
  } else if (type == 2){
    Serial.println ("TYPE 2");
    WritePIO(Pin, ledState);
  } else 
  {
   Serial.println ("TYPE 0");
  beep(ledState,Pin);
  }

  }
  }
    beep (0,Pin);
} //   END OF BlinkBeep FUNCTION


int REDbutton(int button) {
    
    // REDbutton (button pin)
    // REDbutton is a standalone function
    // by Joaquín de Prada
  
// REDbutton variables  
static boolean buttonActive = false;
static unsigned long buttonTimer = 0;
static unsigned long STARTbuttonTimer = 0 ;
// END of REDbutton variables
    
    pinMode(button, INPUT);
 
    
int result= 0 ;

if (digitalRead(button) == LOW) {
            if (buttonActive == false) {
           Serial.println("button pressed");
            STARTbuttonTimer = millis(); 
            buttonActive = true ;
                                        }
     
     } else  {


      if (buttonActive == true) {
            buttonTimer = millis();
            Serial.println("button just released");
            Serial.print("pressed time inside function:  ");
            result = (buttonTimer - STARTbuttonTimer);
            Serial.println(result);
            buttonActive = false ;
     }
     }
                      return result;

     } // END OF REDbutton function




bool beep (bool state,int Pin) {
//int PiezoPin = 5;
pinMode(Pin, OUTPUT);
while (state == true) {
tone (Pin, 1300);
return;
};
noTone(Pin);
Serial.println ("NO TONE");
return;
}

byte WritePIO(byte port, bool ON) {
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
} //END OF WritePIO
