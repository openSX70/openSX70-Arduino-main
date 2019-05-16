ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  TIMSK1 &= ~(1 << TOIE1);                 // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);              // Disable compare B Match Interrupt
  TCNT1 = 0;                                            // Reset the hardware counter
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
  TIFR1=0xFF;                                          //needed to "kill" "lost" interrupts
  TCCR1A=0;                                            // Reset timer/counter control register A
  TCNT1 = 0;                                            //mycounter = TCNT1 would be the total count
  bitSet(TCCR1B ,CS12);                        // Counter Clock source is external pin
  bitSet(TCCR1B ,CS11);                        // Clock on rising edge
  OCR1A = output_compare;                // Set output compare value
  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt

  sei();                                                      // Restart interrupts
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
                 #if SIMPLEDEBUG
                  Serial.println ("AUTO100");
                 #endif 

   //                 byte PictureType = 1;
   //                 CurrentPicture = EEPROM.read(4) ; 
   //
   //                 eepromUpdate ();
 
       if (takePicture == true)
       
       {

                   byte PictureType = 1;                    
                    eepromUpdate ();

             
             #if SHUTTER
//         HighSpeedPWM ();
//         analogWrite(Solenoid2, 255);
                  shutterCLOSE (); 
             #endif
                
                  #if MOTOR 
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
//         analogWrite (Solenoid2, 130);
                  Ydelay ();                               //S3 is now open start Y-delay (40ms)
                  #endif
 
                  #if !MOTOR
                  delay (500);
                  #endif


                  startCounter();
                  shutterOPEN (); 
//                  startMillis = millis;
                  shots = 0;  
                  return;  


       }
}

void finish()
{
  
//  endMillis = millis();
  shutterCLOSE();
  delay (20);
  mirrorDOWN();
delay (50); //changed from 200
//  mirrorUP; WTF????
//delay (200);
 shutterOPEN();
  
//  Serial.print ("exposure time = ");
//  Serial.println(endMillis-startMillis);
  return;
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
