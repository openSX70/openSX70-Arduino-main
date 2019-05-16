/*
  Polaroid Light Meter
  Dave Walker
  16th February 2019
  For use with the TSL235R sensr, connected to digital pins 3,4,5
*/

const int GND_Pin = 3;                   // GND pin for the light sensor
const int VDD_Pin = 4;                   // Vdd pin for the light sensor
const int LED_Pin = 13;                  // To see how long hte shutter would be open
const int output_compare = 32767;        // How many pulses before triggering interrupt
unsigned int old_millis;                 // Temporary variable for displaying time between interrupts
int LED_Flag;                            // For toggling the LED

void setup()
{
  pinMode(GND_Pin, OUTPUT);              // Enable GND pin for the light sensor
  pinMode(VDD_Pin, OUTPUT);              // Enable Vdd pin for the light sensor
  pinMode(LED_Pin, OUTPUT);              // Enable LED output

  digitalWrite(GND_Pin, LOW);            // Set GND value
  digitalWrite(VDD_Pin, HIGH);           // Set Vdd value
 
  Serial.begin(115200);                  // Start serial monitor

  cli();                                 // Stop interrupts
  TCCR1A=0;                              // Reset timer/counter control register A
  bitSet(TCCR1B ,CS12);                  // Counter Clock source is external pin
  bitSet(TCCR1B ,CS11);                  // Clock on rising edge
  OCR1A = output_compare;                // Set output compare value
  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt
  sei();                                 // Restart interrupts
 }

void loop()                              // Nothing to see here
{   

}

ISR(TIMER1_COMPA_vect)                   // Output Compare interrupt service routine 
{
  if(LED_Flag)
  {
    bitClear(PORTB, LED_Pin - 8);        // Turn LED off
    LED_Flag = 0;                        // Reset flag
  }
  else
  {
    bitSet(PORTB, LED_Pin - 8);          // Turn LED on
    LED_Flag = 1;                        // Set flag
  }
//  Serial.print("Time since last event = ");
//  Serial.print(millis()-old_millis);
//  Serial.println(" ms");
  TCNT1 = 0;                            // Reset the hardware counter
//  old_millis = millis();
}

ISR(TIMER1_OVF_vect)                    // Overflow Flag interrupt service routine 
{
  Serial.println("Overflow Occured!");
  TCNT1 = 0;                            // Reset the hardware counter
}
