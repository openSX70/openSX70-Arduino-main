// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Timer_0_interrupt_test.ino
    Created:	25/05/2019 10:36:47
    Author:     Ronda2A\Joaquin
*/

// Define User Types below here or use a .h file
//
volatile uint8_t count;
volatile uint8_t delayMS

// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//
void MyDelay(void)
{
	TCNT0 = 0;  //Set counter 0 value to 0
	count = 0; //set count to zero

	//TCCR0 |= (1 << CS02) | (1 << CS00); // PRESCALER 1024
	TCCR0 |= (1 << CA01) ; // PRESCALER 8
	/*
	The clock is external crystal 8MHz.We apply a prescaler of 1024.
		Now the clock freq to counter becomes 8000000/8 = 1 Mhz
		Time period = 1/freq = 1/1000000 = 0.000001usec or 0.001 milli sec
		Overflow occurs every 256*0.001 = 0.256 msec
		To generate a time delay of 1 sec 1000/32.768 =  30.5 or 31
		So the counter TCNT0 has to overflow 31 times to generate A TIME DELAY OF 1 SEC.
	*/
	TIMSK |= (1 << OCIE0);	// enable compare interrupt
	TIMSK = (1 << TOIE0);

	sei();
}

void  timer_init(void)
{
	// set up timer with prescaler = 1024 and CTC mode
	TCCR0 = (1 << WGM01);
	TCCR0 = (1 << CS00) | (1 << CS02);
		TCNT0 = 0; 	// initialize counter

		OCR0 = 220;	// initialize compare value

		TIMSK |= (1 << OCIE0);	// enable compare interrupt

		sei();		// enable global interrupts

}
ISR(TIMER0_OVF_vect)    // Timer1 ISR
{
	count = count++;
	if count = delayMS
	{
		TCNT0 = 0; 	// initialize counter
		finish();
	}
}

// The setup() function runs once each time the micro-controller starts
void setup()
{


	}

// Add the main program code into the continuous loop() function
void loop()
{


}
