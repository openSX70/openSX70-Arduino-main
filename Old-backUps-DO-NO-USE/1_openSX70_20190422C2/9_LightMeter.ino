ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  TIMSK1 &= ~(1 << TOIE1);                 // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);              // Disable compare B Match Interrupt
  TCNT1 = 0;                                            // Reset the hardware counter
  //Serial.print("I");
  finish();
}
 
ISR(TIMER1_OVF_vect)                        // Overflow Flag interrupt service routine
{
  Serial.print("Overflow Occured!");
  TIMSK1 &= ~(1 << TOIE1);                 // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);              // Disable compare B Match Interrupt
}
 
void startCounter()
{
                    takePicture = false;
  cli();                                                        // Stop interrupts
  //TCCR0A = 0;    // stop timer 0
  //TCCR0B = 0;    
  
  
  TCCR1A=0;                                            // Reset timer/counter control register A
  TCCR1B = 0;
  TIFR1=0xFF;                                          //needed to "kill" "lost" interrupts
  
  TCNT1 = 0;                                            //mycounter = TCNT1 would be the total count
  //bitSet(TCCR1B ,CS12);                        // Counter Clock source is external pin
  //bitSet(TCCR1B ,CS11);                        // Clock on rising edge
  GTCCR = bit (PSRASY);        // reset prescaler now
  
  OCR1A = output_compare;                // Set output compare value
  

  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt

  sei();                                                      // Restart interrupts


  TCCR1B =  bit (CS10) | bit (CS11) | bit (CS12);

  
}

void startCounterCalibration()
{
  
  cli();                                                        // Stop interrupts
  TIFR1=0xFF;                                          //needed to "kill" "lost" interrupts
  TCCR1A=0;                                            // Reset timer/counter control register A
  TCNT1 = 0;                                            //mycounter = TCNT1 would be the total count
  bitSet(TCCR1B ,CS12);                        // Counter Clock source is external pin
  bitSet(TCCR1B ,CS11);                        // Clock on rising edge
    sei();                                                      // Restart interrupts
}


void AutoExposure()

{
  //////AUTOEXPOSURE DEBUG

                  shutterCLOSE (); 
                
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
//         analogWrite (Solenoid2, 130);
                  Ydelay ();                               //S3 is now open start Y-delay (40ms)

                  startCounter();
                  shutterOPEN (); 
//                  startMillis = millis;
//                  shots =  ++shots;  

                  return;  
}

void finish()
{
       shutterCLOSE();
       delay (200); //Was 20
        //switch1 = Read_DS2408_PIO(1);
        //Serial.print ("finish, switch1 =");
        //Serial.println (switch1);
 
               if (switch1 == 1)
                  {
                  shots = ++shots;
                  return;
                  } else if (switch1 == 0)
                  {
                    delay (200);                             //AGAIN is this delay necessary?
                    mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                    delay (200);                  //WAS 60           //AGAIN is this delay necessary?
                    shutterOPEN();
                    shots = 0;  
                    return;   
                  }

}











/*
 ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
//  digitalWrite(led2,LOW);                  //Just turn OFF the LED to avoid serial.Print...
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
  TIFR1=0xFF;                             //needed to "kill" "lost" interrupts 
  
   TCCR1A=0;                              // Reset timer/counter control register A
  TCCR1B = 0;                                                         //ADDED
  TCNT1 = 0;                              //mycounter = TCNT1 would be the total count
  bitSet(TCCR1B ,CS12);                  // Counter Clock source is external pin
  bitSet(TCCR1B ,CS11);                  // Clock on rising edge
  //OCR1A = output_compare;                // Set output compare value
  //  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt

  
// TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt
      TCCR1B = 1; //start counter????

  sei();                               // Restart interrupts
 }


void startCounter2()
{
  TCCR1A = 0; //initialize Timer1
  TCCR1B = 0;
  TCNT1 = 0;
      TCNT1 = 0;//initialize counter
    // External clock source on Timer1, pin (D5). Clock on rising edge.
    // Setting bits starts timer
    TCCR1B =  bit (CS10) | bit (CS11) | bit (CS12); //external clock source pin D5 rising edge
}

unsigned long FreqCounter()
{

  //    cli();                                    
//    TCCR1B = 0; //stop counter
    counter = TCNT1;
    return counter;
//    sei();

}

*/
