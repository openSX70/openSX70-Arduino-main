ISR(TIMER1_COMPA_vect){
  TIMSK1 = 0;
  //integrationFinished = 1;
  // function / flag.
    finish();
}

/*
ISR(TIMER1_COMPA_vect)                  // Output Compare interrupt service routine
{
  TIMSK1 &= ~(1 << TOIE1);                 // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);              // Disable compare B Match Interrupt
  TCNT1 = 0;                                            // Reset the hardware counter
  //Serial.print("I");
  finish();
}*/

ISR(TIMER1_OVF_vect)                        // Overflow Flag interrupt service routine
{
  Serial.print("Overflow Occured!");
  TIMSK1 &= ~(1 << TOIE1);                 // Disable compare A Match Interrupt (overflow)
  TIMSK1 &= ~(1 << OCIE1A);              // Disable compare B Match Interrupt
  //counter = 9999;
}



void tsl237_init(){
  cli();
  // Clear all interrupts flags
  TIFR1 = (1 << ICF1) | (1 << OCF1B) | (1 << OCF1A) | (1 << TOV1);
  // Set timer 1 for normal operation, clocked by rising edge on T1 (port D5 / pin 5)
  TCCR1A = 0;
  TCCR1B = (1 << CS10) | (1 << CS11) | (1 << CS12);
  TCCR1C = 0;
  // set current value to 0 (needs to be done, maybe, on each new conversion)
  TCNT1 = 0;
  sei();                    // Restart interrupts
}


/*{
  takePicture = false;
  cli();                                                        // Stop interrupts
  //TCCR0A = 0;    // stop timer 0
  //TCCR0B = 0;


  TCCR1A = 0;                                          // Reset timer/counter control register A
  TCCR1B = 0;
  TIFR1 = 0xFF;                                        //needed to "kill" "lost" interrupts

  TCNT1 = 0;                                            //mycounter = TCNT1 would be the total count
  //bitSet(TCCR1B ,CS12);                        // Counter Clock source is external pin
  //bitSet(TCCR1B ,CS11);                        // Clock on rising edge
  GTCCR = bit (PSRASY);        // reset prescaler now

  OCR1A = output_compare;                // Set output compare value


  TIMSK1 |= (1 << TOIE1);                // Enable compare A Match Interrupt
  TIMSK1 |= (1 << OCIE1A);               // enable compare B Match Interrupt

  sei();                                                      // Restart interrupts


  TCCR1B =  bit (CS10) | bit (CS11) | bit (CS12);


}*/

void startCounter()
{
  takePicture = false;

  cli();
  // Clear interrupts flags we are using
//  TIFR1 = (1 << OCF1A) | (1 << TOV1);
  TIFR1 = (1 << OCF1A);
  // set compare value given sensivity
  OCR1A = output_compare;
  // clear counter value.
  TCNT1 = 0;
  // Set interrupt vectors for compare match A.
//  TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);
  TIMSK1 = (1 << OCIE1A);

  sei();
}

void startCounterCalibration()
{

  cli();                                                        // Stop interrupts
  TIFR1 = 0xFF;                                        //needed to "kill" "lost" interrupts
  TCCR1A = 0;                                          // Reset timer/counter control register A
  TCNT1 = 0;                                            //mycounter = TCNT1 would be the total count
  bitSet(TCCR1B , CS12);                       // Counter Clock source is external pin
  bitSet(TCCR1B , CS11);                       // Clock on rising edge
  sei();                                                      // Restart interrupts
}


void AutoExposure()

{
#if LIGHTMETER
  //////AUTOEXPOSURE DEBUG
  //lets turn off all the LED!!!!
  digitalWrite(led2, LOW);
  digitalWrite(led1, LOW);

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
#endif
#if !LIGHTMETER
  return;
#endif

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
    delay (100);                             //AGAIN is this delay necessary?
    mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (300);                  //WAS 60           //AGAIN is this delay necessary?
    shutterOPEN();
    shots = 0;
    tsl237_init();
    return;
  }

}


int PredictedExposure()
{

  long interval = 100;
  //unsigned long counter; defined elsewhere
  unsigned long PredExp;

  //            unsigned long currentMillis = millis();


  //  unsigned long timeMillis;

  startCounterCalibration();
  delay (interval);
  counter = TCNT1;
  //    TCNT1 = 0;

  //Serial.print("ISO:  ");
  //Serial.println(ISO);
  /*
      if (cISO == 10)
      {
        ISO = A100;
        //      PredExp = 100000 / counter;

      }
      else if (cISO == 60)
      {
        ISO = A600;
      }

      /*
      Serial.print("ISO: ");
      Serial.println (ISO);
      Serial.print("minCounter: ");
      Serial.println(minCounter);
  */

//unsigned long preISO;

//preISO = ISO * interval;

/*  if (ISO = 450)
  preISO = 450000;

  if (ISO = 1000)
  preISO = 100000;
*/
//PredExp = (preISO / counter );
  
  PredExp = ((output_compare * interval) / counter );

  if (PredExp > 300)
  {
//    PredExp = 333;
      return 333;
  }
  else if (PredExp < 30)
  {
//    PredExp = 16;
      return 16;
  }
  
 /* 
      Serial.print("Magic Number:");
      Serial.print(ISO);
      Serial.print(" / timeMillis ");
      Serial.print(timeMillis);
      Serial.print(" / counter: ");
      Serial.print(counter);
      Serial.print(" / PredExp: ");
      Serial.println(PredExp);
      //    Serial.print(" / ");

  */
  return PredExp;
}





int nearest(int x, int myArray[], int elements, bool sorted)
{
  int idx = 0; // by default near first element

  int distance = abs(myArray[idx] - x);
  for (int i = 1; i < elements; i++)
  {
    int d = abs(myArray[i] - x);
    if (d < distance)
    {
      idx = i;
      distance = d;
    }
    else if (sorted) return idx;
  }
  return idx;
}
int ISO()
{
      cISO = EEPROM.read(20);
    if (cISO == 10)
    {
     // output_compare = A100;
      int iso = A100;
      return iso;
    }
    else if (cISO == 60)
    {
    //  output_compare = A600;
    int iso = A600;
    return iso;
    }
}
