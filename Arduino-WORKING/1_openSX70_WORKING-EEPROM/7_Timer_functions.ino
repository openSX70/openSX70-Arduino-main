//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                TIMER FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************

void BeepTimerDelay () {
  // PIEZO BUZZER BEEPS
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay() 
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.

unsigned long startTimer = millis();

BlinkBeep (500,2600,6,0);

BlinkBeep (300,2400,6,0);

BlinkBeep (200,2400,6,0);

BlinkBeep (70,1400,6,0);

BlinkBeep (1200,1200,6,0);

#if ISDEBUG
unsigned long endTimer = millis();
unsigned long time = ((endTimer-startTimer)/1000);
Serial.print(time);
Serial.println ("  seconds  ");
#endif

}


void LEDTimerDelay() {
  //MAIN PCB LED (D13) BLINKS (10 secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay() 
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.
unsigned long startTimer = millis();
Serial.println ("START 500,2600,5,0");

//BUILT-IN LED
//*******************************************************
BlinkBeep (1000,2500,5,1);

BlinkBeep (600,2500,5,1);

BlinkBeep (200,2500,5,1);

BlinkBeep (80,2500,5,1);

#if ISDEBUG
unsigned long endTimer = millis();
unsigned long time = ((endTimer-startTimer)/1000);
Serial.print(time);
Serial.println ("  seconds  ");
#endif

}

void BlinkTimerDelay() {
  // DONGLE-LED BLINKS ON COUNTDOWN (10secs)
  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay() 
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.

unsigned long startTimer = millis();
Serial.println ("START 500,2600,5,0");

//*******************************************************

// DS2408 LED
BlinkBeep (1000,2500,6,2);

BlinkBeep (600,2500,6,2);

BlinkBeep (200,2500,6,2);

BlinkBeep (80,2500,6,2);

#if ISDEBUG
unsigned long endTimer = millis();
unsigned long time = ((endTimer-startTimer)/1000);
Serial.print(time);
Serial.println ("  seconds  ");
#endif

}

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
    Write_DS2408_PIO(Pin, ledState);
  } else 
  {
   Serial.println ("TYPE 0");
  beep(ledState,Pin);
  }

  }
  }
    beep (0,Pin);
} //   END OF BlinkBeep FUNCTION




bool beep (bool state,int Pin) {
//int PiezoPin = 5;
pinMode(Pin, OUTPUT);
while (state == true) {
tone (Pin, 1300);
return;
};
noTone(Pin);
//Serial.println ("NO TONE");
return;
}
bool simpleBlink (byte times) {

for (int i = 1 ; i <= times; i++)
delay (200);
Write_DS2408_PIO (6, 1);
delay (200);
Write_DS2408_PIO (6, 0);
}
//***************************************************************************************************************************************
