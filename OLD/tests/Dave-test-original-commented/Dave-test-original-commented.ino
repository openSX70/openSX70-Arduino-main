/*
  Polaroid Light Meter
  Dave Walker
  16th February 2019
  For use with the TSL235R sensor, connected to digital pins 3,4,5
*/
 
const int GND_Pin = 3;                   // GND pin for the light sensor
const int VDD_Pin = 4;                   // Vdd pin for the light sensor
const int output_compare = 327;        // How many pulses before triggering interrupt
unsigned int old_millis;                 // Temporary variable for displaying time between interrupts
 
void setup()
{
  pinMode(GND_Pin, OUTPUT);              // Enable GND pin for the light sensor
  pinMode(VDD_Pin, OUTPUT);              // Enable Vdd pin for the light sensor
 
  digitalWrite(GND_Pin, LOW);            // Set GND value
  digitalWrite(VDD_Pin, HIGH);           // Set Vdd value
 
  Serial.begin(9600);                    // Start serial monitor
 
  cli();                                 // Stop interrupts
  TCCR1A=0;                              // Reset timer/counter control register A
  bitSet(TCCR1B ,CS12);                  // Counter Clock source is external pin
  bitSet(TCCR1B ,CS11);                  // Clock on rising edge
  OCR1A = output_compare;                // Set output compare value
  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt
  sei();                                 // Restart interrupts
 }
 
void loop()
{                                       // Nothing to see here
}
 
ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine 
{
  Serial.print("Time since last event = ");
  Serial.print(millis()-old_millis);
  Serial.println(" ms");
  TCNT1 = 0;                            // Reset the hardware counter
  old_millis = millis();
}
 
ISR(TIMER1_OVF_vect)                    // Overflow Flag interrupt service routine 
{
  Serial.print("Overflow Occured!");
  TCNT1 = 0;                            // Reset the hardware counter
}
/*
---------------------------------------------------------------------------------
 
The nice thing about this is that, if it goes in on pin 5, it doesn't use the processor at all - the whole thing's done in the ATMEGA peripheral counter/timer :o)  (Notice, there's nothing in the main loop whatsoever).
 
You set the number of pulses before you get an interrupt in the "output_compare" variable.  When it reaches this figure, it dumps a message to the serial output. What you get in the serial monitor is something like this:
 
01:27:59.026 -> Time since last event = 113 ms
01:27:59.073 -> Time since last event = 85 ms
01:27:59.155 -> Time since last event = 72 ms
01:27:59.249 -> Time since last event = 62 ms
01:27:59.296 -> Time since last event = 57 ms
01:27:59.343 -> Time since last event = 54 ms
01:27:59.390 -> Time since last event = 53 ms
01:27:59.436 -> Time since last event = 55 ms
01:27:59.483 -> Time since last event = 53 ms
01:27:59.578 -> Time since last event = 55 ms
01:27:59.625 -> Time since last event = 61 ms
01:27:59.706 -> Time since last event = 67 ms
01:28:01.075 -> Time since last event = 1394 ms
01:28:03.949 -> Time since last event = 2886 ms
01:28:06.846 -> Time since last event = 2887 ms
01:28:09.762 -> Time since last event = 2908 ms
 
The timing isn't exactly perfect, as I'm just using the "millis()" function for a rough indication, and I'm using the Serial.print() routine in the interrupt service routine... but it gives a rough idea what's going on.  On the example above, I shone my phone torch onto the sensor (the low numbers), or had it facing out to a dim room (the big numbers).
 
I need to test it in various lighting, and see if it can be calibrated against my Weston  Master V :o)
 
So, to re-cap... the theory is that we determine how many pulses are required for a "good" exposure.  We open the shutter, reset the counter, and start counting... when it reaches the magic number, it triggers an interrupt, which closes the shutter.  By fiddling with the output compare register, we can choose different values of "good" exposure.  It's a 16-bit counter (so, what... 65,535 pulses before it overflows? But, if you need more, you can just handle the overflow flag and manipulate it to count up a couple of times.)
 
...this is why I only connected one LED on my OpenSX70 board... I wanted to save pin 5, because it's the only pin that can be used as an external trigger to the counter-timer :o) 
 
The only down-side is that it needs at least 2.7V (which is why I was asking about regulators, brown-outs and decoupling earlier).  By the way, I'm pretty sure we can increase back to 3V3 with a bit of cleverer decoupling...
*/
