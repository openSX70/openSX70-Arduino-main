//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                TIMER FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************

void timerDelay() {

              #if ISDEBUG 
              Serial.println("timerDelay");      
              #endif

  // this is a test function to do the progressing blinking of the LED using my blink function
  // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay() 
  // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.

unsigned long startTimer = millis();
Serial.println ("START 500,2600,5,0");
BlinkBeep (500,2600,5,0);
;
Serial.println ("START 300,2400,5,0");
BlinkBeep (300,2400,5,0);
;
Serial.println ("START 200,2400,5,0");
BlinkBeep (200,2400,5,0);
;
Serial.println ("START 70,1400,5,0");
BlinkBeep (70,1400,5,0);
;

Serial.println ("START 1200,1200,5,0");
BlinkBeep (1200,1200,5,0);
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

int BlinkBeep (int interval, int timer, int Pin, bool isLED)
// blink (blink interval=blinking speed, timer=duration blinking, Pin=pin of LED isLED 1 = LED 0 = Piezo)
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
  if (isLED == true) {
    digitalWrite (Pin, ledState);
  } else{
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
Serial.println ("NO TONE");
return;
}
//***************************************************************************************************************************************
