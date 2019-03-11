/*
  Polaroid Light Meter
  Dave Walker
  16th February 2019
  For use with the TSL235R sensor, connected to digital pins 3,4,5
*/

//const int GND_Pin = 3;                   // GND pin for the light sensor
//const int VDD_Pin = 4;                   // Vdd pin for the light sensor
const int output_compare = 3276;        // How many pulses before triggering interrupt
unsigned int old_millis;                 // Temporary variable for displaying time between interrupts
const int buttonPin = 3;     //Red button SHUTTER RELEASE

void setup()
{
  //  pinMode(GND_Pin, OUTPUT);              // Enable GND pin for the light sensor
  //  pinMode(VDD_Pin, OUTPUT);              // Enable Vdd pin for the light sensor

  //  digitalWrite(GND_Pin, LOW);            // Set GND value
  //  digitalWrite(VDD_Pin, HIGH);           // Set Vdd value

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);                    // Start serial monitor

  cli();                                 // Stop interrupts
  TCCR1A = 0;                            // Reset timer/counter control register A
  bitSet(TCCR1B , CS12);                 // Counter Clock source is external pin
  bitSet(TCCR1B , CS11);                 // Clock on rising edge
  OCR1A = output_compare;                // Set output compare value
  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt
  //  sei();                                 // Restart interrupts
}

void loop()
{
  // Get button event and act accordingly
  int b = checkButton();
  if (b == 1)

  {
    sei();
  }
  //   if (b == 2) doubleClickEvent();
  if (b == 3)
  {
    sei();
  }
}

ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  Serial.print("time = ");
  Serial.print(millis() - old_millis);
  Serial.println(" ms");
  TCNT1 = 0;                            // Reset the hardware counter
  old_millis = millis();
  cli();
}

ISR(TIMER1_OVF_vect)                    // Overflow Flag interrupt service routine
{
  Serial.print("Overflow Occured!");
  TCNT1 = 0;                            // Reset the hardware counter
}
