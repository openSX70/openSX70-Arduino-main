/*
  Polaroid Light Meter
  Dave Walker
  16th February 2019
  For use with the TSL235R sensor, connected to digital pins 3,4,5
*/

//const int GND_Pin = 3;                   // GND pin for the light sensor
//const int VDD_Pin = 4;                   // Vdd pin for the light sensor
const int output_compare = 500;        // How many pulses before triggering interrupt
unsigned int old_millis;                 // Temporary variable for displaying time between interrupts
const int buttonPin = 3;     //Red button SHUTTER RELEASE
int b;
void setup()
{
  //  pinMode(GND_Pin, OUTPUT);              // Enable GND pin for the light sensor
  //  pinMode(VDD_Pin, OUTPUT);              // Enable Vdd pin for the light sensor

  //  digitalWrite(GND_Pin, LOW);            // Set GND value
  //  digitalWrite(VDD_Pin, HIGH);           // Set Vdd value

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);                    // Start serial monitor

  Serial.println ("Setup Setup Setup Setup Setup Setup Setup Setup Setup ");
  
  cli();                                 // Stop interrupts
  TCCR1A = 0;                            // Reset timer/counter control register A
  bitSet(TCCR1B , CS12);                 // Counter Clock source is external pin
  bitSet(TCCR1B , CS11);                 // Clock on rising edge
  OCR1A = output_compare;                // Set output compare value

//  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt (overflow)

/*
    • Bit 0 – TOIE1: Timer/Counter1, Overflow Interrupt Enable 
      When this bit is written to one, and the I-flag in the status register is set (interrupts globally enabled), the Timer/Counter1 
      overflow interrupt is enabled. The corresponding interrupt vector (see Section 11. “Interrupts” on page 49) 
      is executed when the TOV1 flag, located in TIFR1, is set.
  */

/*
    x |= (1 << n);       // forces nth bit of x to be 1.  all other bits left alone.
*/

// TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt

 /*  
    • Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
      When this bit is written to one, and the I-flag in the status register is set (interrupts globally enabled), 
      the Timer/Counter1 output compare A match interrupt is enabled. The corresponding interrupt vector 
      (see Section 11. “Interrupts” on page 49) is executed when the OCF1A flag, located in TIFR1, is set.
*/

  TIMSK1 &= (1 << TOIE1);                // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= (1 << OCIE1A);                // Disable compare B Match Interrupt 

  sei();                                 // Restart interrupts

  Serial.println ("Setup Setup Setup Setup Setup Setup Setup Setup Setup ");
}

void loop()
{


//  TIMSK1 &= (1 << OCIE1A);
//   x &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
  // Get button event and act accordingly
   b = checkButton();
  if ((b == 1)| (b==3))

  {
    Serial.println ("OPEN 1");
//      cli();
      TCNT1 = 0;                            // Reset the hardware counter
      TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt (overflow)
      TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt
//      sei();     
  }

}

ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  
  Serial.println ("CLOSE");
  Serial.print("time = ");
  Serial.print(millis() - old_millis);
  Serial.println(" ms");
  TCNT1 = 0;                            // Reset the hardware counter
  old_millis = millis();
  
//  cli();
  TIMSK1 &= (1 << TOIE1);                // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= (1 << OCIE1A);                // Disable compare B Match Interrupt 
            
//   x &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
//  sei();

}

ISR(TIMER1_OVF_vect)                    // Overflow Flag interrupt service routine
{
  Serial.print("Overflow Occured!");
  TCNT1 = 0;                            // Reset the hardware counter
}
