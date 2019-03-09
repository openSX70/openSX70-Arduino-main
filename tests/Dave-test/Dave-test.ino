/*
  Polaroid Light Meter
  Dave Walker
  16th February 2019
  For use with the TSL235R sensor, connected to digital pins 3,4,5
*/

//const int GND_Pin = 3;                   // GND pin for the light sensor
//const int VDD_Pin = 4;                   // Vdd pin for the light sensor
const int output_compare = 32767;        // How many pulses before triggering interrupt
unsigned int old_millis;                 // Temporary variable for displaying time between interrupts
const int S1 = 12;     //Red button SHUTTER RELEASE

void setup()
{
//  pinMode(GND_Pin, OUTPUT);              // Enable GND pin for the light sensor
//  pinMode(VDD_Pin, OUTPUT);              // Enable Vdd pin for the light sensor

//  digitalWrite(GND_Pin, LOW);            // Set GND value
//  digitalWrite(VDD_Pin, HIGH);           // Set Vdd value

pinMode(S1, INPUT_PULLUP);
 
  Serial.begin(115200);                    // Start serial monitor

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
{
 if ((digitalRead(S1) == LOW))
 {
  delay (50);
 old_millis = millis;
 sei();
 TCNT1 = 0;                            // Reset the hardware counter
  Serial.println("CLICK"); 
  
 }
}

ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine 
{
  Serial.print("Exposure time = ");
  Serial.print(millis()-old_millis);
  Serial.println(" ms");
  TCNT1 = 0;                            // Reset the hardware counter
//  old_millis = millis();
  cli();
}

ISR(TIMER1_OVF_vect)                    // Overflow Flag interrupt service routine 
{
  Serial.print("Overflow Occured!");
  TCNT1 = 0;                            // Reset the hardware counter
}
