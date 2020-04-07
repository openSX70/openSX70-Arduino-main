/*
  Polaroid Light Meter
  Dave Walker
  16th February 2019
  For use with the TSL235R sensor, connected to digital pins 3,4,5
  2019/03/19
  Modified by J and P for button operation
  pressing the button turns on the led and ISR turns it off
  if I want to "count photons" (TM) (  ;-)  ) I should use something like:
          mycounter = TCNT1 would be the total count
  I just have to figure the value of the capactior for widest resolution 
  and figure out how tolerances on the capactior are going to affect lightmeter operation
  something around 1000nF to 500nF?
  
*/

const int LED = 13;                   // GND pin for the light sensor
const int output_compare = 1000;        // How many pulses before triggering interrupt
//unsigned int old_millis;                 // Temporary variable for displaying time between interrupts
const int buttonPin = 3;     //Red button SHUTTER RELEASE
byte checkButton();
int b;
void startCounter();
void setup()

{
 pinMode(buttonPin, INPUT_PULLUP);
 pinMode(LED, OUTPUT);
 digitalWrite(LED,LOW);
 
 Serial.begin(9600);                    // Start serial monitor
 Serial.println ("Starting...");
} // end of setup()

void loop()
{
  b = checkButton(); //this is just the button press long or short but it does not matter.
 // digitalWrite(LED,LOW);
  if ((b == 1) || (b==3))
  {
 Serial.println ("BUTTON PRESSED OPEN SHUTTER");
 digitalWrite(LED,HIGH);
startCounter();

}  // end of loop()

}


ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  digitalWrite(LED,LOW);                  //Just turn OFF the LED to avoid serial.Print...
  TIMSK1 &= ~(1 << TOIE1);                // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);                // Disable compare B Match Interrupt 
  //TCCR1A=0; 
  TCNT1 = 0;                            // Reset the hardware counter
}

ISR(TIMER1_OVF_vect)                    // Overflow Flag interrupt service routine
{
  Serial.print("Overflow Occured!");
  TIMSK1 &= ~(1 << TOIE1);                // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);                // Disable compare B Match Interrupt 
//  TCNT1 = 0;                            // Reset the hardware counter
}



void startCounter() //function NOT used now
{
  cli();                                 // Stop interrupts
  
  TCCR1A=0;                              // Reset timer/counter control register A
  TCNT1 = 0;                              //mycounter = TCNT1 would be the total count
  TIFR1=0xFF;                             //needed to "kill" "lost" interrupts 
  bitSet(TCCR1B ,CS12);                  // Counter Clock source is external pin
  bitSet(TCCR1B ,CS11);                  // Clock on rising edge
  OCR1A = output_compare;                // Set output compare value
  //TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt
  sei();                               // Restart interrupts
 }
