void setup() {


//SAY HELLO (remove)
    Serial.begin(9600);
    Serial.println ("openSX70!!!!!!!!!!!!!!!");


// this code only runs once:
//Solenoid #1 and #2 PINS are OUTPUTS

//Alpha Boards have two leds visible in a sonar-type VF.

  pinMode(led1, OUTPUT);  //BLUE   
//Check good
  pinMode(led2, OUTPUT);  //RED
#if 0
  pinMode(Solenoid1, OUTPUT);
  pinMode(Solenoid2, OUTPUT);

// OPTION I turn on LEDs once on power-up:
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 
  delay (600);
  digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 
  digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 
  delay (600);
  digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 

/*
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                
  digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                
  delay (600);
  
  digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 
  digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)                                                                                                                                                                                                 
 */
                                                                                                                                                                                
//SX70 switches are INPUTS
//I like to think that the SX70 has mechanical electronics :-)
//CHeck bad
pinMode(S1, INPUT_PULLUP);
pinMode(S2, INPUT_PULLUP);
pinMode(S3, INPUT_PULLUP);
pinMode(S5, INPUT_PULLUP);
pinMode(S8, INPUT);
pinMode(S9, INPUT);
pinMode(FFA, OUTPUT);

digitalWrite(FFA, LOW);           // make sure I don't fire the flash!

#if SIMPLEDEBUG
Serial.begin (9600);
Serial.println ("WelcomE to openSX70");
Serial.print ("Version: ");
Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));
#endif

//Motor is OUTPUT
pinMode(Motor, OUTPUT);


//I make sure that the Motor is OFF
digitalWrite(Motor, LOW);

//I also want the Shutter to be open for SLR-TTL fuctionality

analogWrite(Solenoid1, 0);
analogWrite(Solenoid2, 0);

//CHECK BAD
//void intializeDS2408();

// EEPROM STUFF INITIALIZING SEQUENCE
//    Serial.begin (9600);
    char initJP [2];
    initJP[0] = EEPROM.read(0);
    initJP[1] = EEPROM.read(1);

    if (initJP[0] != 'S' || initJP[1] != 'X')
        {
  #if SIMPLEDEBUG
  Serial.println ("Initializing EEPROM....");
  #endif
  
  EEPROM.write(0,'S');
  EEPROM.write(1,'X');
  int eeAddress = 101;
  EEPROM.put (10,eeAddress);
  int ActualPicture= 1;
  EEPROM.put (13,ActualPicture);
  }
      else 
  {

if ((switch1 == 1) && (switch2 == 1)) {
  #if SIMPLEDEBUG
  Serial.println ("RE-nitializing EEPROM....");
  #endif
  
  EEPROM.write(0,'S');
  EEPROM.write(1,'X');
  int eeAddress = 101;
  EEPROM.put (10,eeAddress);
  int ActualPicture= 1;
  EEPROM.put (13,ActualPicture);
}

EEPROM.get(10,eeAddress);  // where to write
EEPROM.get (13, ActualPicture); //the picture taken counter
/*
#if SIMPLEDEBUG
Serial.println ("EEPROM already initialized...");
Serial.print ("eeAddress: ");
Serial.println (eeAddress);
Serial.print ("ActualPicture: ");
Serial.println (ActualPicture);
#endif */
  }
/*
EEPROM.get(10,eeAddress);  // where to write
EEPROM.get (13, ActualPicture); //the picture taken counter

Serial.print ("eeAddress READ:");
Serial.println (eeAddress);
Serial.print ("ActualPicture READ:");
Serial.println (ActualPicture);
*/
  
//byte PictureType = 0;
// PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600

//Start with Dongle LED off
//Write_DS2408_PIO (6, 0);

//byte CurrentPicture = EEPROM.read (4);

//device_count = ds.find(&devices);
//uint8_t readDevice = ds.get_state(devices[0]);
//selector = Read_DS2408_PIO(0);
//switch1 = Read_DS2408_PIO(1);
//switch2 = Read_DS2408_PIO(2);

device_count = ds.findsingle(&dongleDevice);    

if (device_count != 0)
  initializeDS2408(); //Pitsie fix for non-blinking LED on dongle

//OPTION this is the so called LED counter coment auto this part to disable the counter.
//LED COUNTER 
      
   //if (digitalRead(S8) != HIGH || digitalRead(S9) != LOW)
   if (digitalRead(S8) == LOW && digitalRead(S9) == LOW && device_count > 0) //NORMAL OPERATION
      {
      simpleBlink (8 - (EEPROM.read (4)));
      #if SIMPLEDEBUG
      Serial.println ("BLINK NUMBER OF SHOTS");
      #endif
      }
//LED COUNTER END

#if 0
// SETUP STUFF FOR INTERRUPTS.
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
  //      TCCR1B = 1; //start counter????

sei();                               // Restart interrupts
#endif

//BRING MIRROR DOWN IF HALFWAY OR UP

if (digitalRead(S5) != LOW || digitalRead(S3) != LOW)
{
  mirrorDOWN();
}
 /*
 //This is just in case the mirror is (for whatever reason) halfway when I power up I want to run this JUST ONCE
while (digitalRead(S5) != LOW || digitalRead(S3) != LOW)  // while S5 or S3 are different than CLOSED (mirror at an angle)
{ motorON ();
  
  motorOFF();
}
*/
#endif
}
