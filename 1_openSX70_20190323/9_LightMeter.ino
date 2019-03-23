 ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  digitalWrite(led2,LOW);                  //Just turn OFF the LED to avoid serial.Print...
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
  //OCR1A = output_compare;                // Set output compare value
  //TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt

  
//  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt
  sei();                               // Restart interrupts
 }
