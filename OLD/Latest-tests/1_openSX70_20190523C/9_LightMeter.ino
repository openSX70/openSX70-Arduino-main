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
  //counter = 9999;
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
                    delay (100);                             //AGAIN is this delay necessary?
                    mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                    delay (300);                  //WAS 60           //AGAIN is this delay necessary?
                    shutterOPEN();
                    shots = 0;  
                    return;   
                  }

}


int PredictedExposure(int ISO)
{
	static int PrevExp;
	//ISO is the Magic Number 
	unsigned long interval = 100;
	//unsigned long counter; defined elsewhere 
	float PredExp;

	//            unsigned long currentMillis = millis();


	unsigned long timeMillis;

	if ((millis() - previousMillis) >= interval)
	{
		counter = TCNT1;
//		TCNT1 = 0;
		timeMillis = millis() - previousMillis;
		previousMillis = millis();
		startCounterCalibration();
		
		//PredExp = ISO * (timeMillis / (1 + counter));
		float ISOpercent; //shutter closing compensation now 30%
		ISOpercent = ((ISO * 5)/100);
		ISO = ISO + ISOpercent;
		//
//		PredExp = ISO / ( ((counter+1) / timeMillis));
		PredExp = ISO / (1 + (counter / timeMillis));

//		PredExp = (counter / timeMillis);
//		PredExp = ISO / ( ((counter) / timeMillis));

		//		PredExp = PredExp * 0,99;

 /*
		Serial.print(ISO);
		Serial.print(" / ");
		Serial.print(timeMillis);
		Serial.print(" / ");
		Serial.print(counter);
		Serial.print(" / ");
		Serial.println(PredExp);
//		Serial.print(" / ");
+ */
		PrevExp = PredExp;
		return PrevExp;
	}
	else 
		return PrevExp;


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
