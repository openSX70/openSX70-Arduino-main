void setup() {

  //previousMillis = 0;
  //SAY HELLO (remove)
  Serial.begin(9600);
  Serial.println("openSX70!!!!!!!!!!!!!!!");


  //EEPROM.get(20, currentISO);



  // this code only runs once:
  //Solenoid #1 and #2 PINS are OUTPUTS

  //Alpha Boards have two leds visible in a sonar-type VF.

  pinMode(led1, OUTPUT);  //BLUE
  pinMode(led2, OUTPUT);  //RED

  //digitalWrite(led1, LOW);

  pinMode(Solenoid1, OUTPUT);
  pinMode(Solenoid2, OUTPUT);

  /*
    // OPTION I turn on LEDs once on power-up:
    digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay (600);
    digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay (600);
    digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)

  	digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
  	digitalWrite(led2, HIGH);   // turn the LED on (HIGH is the voltage level)
  	delay (600);

  	digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)
  	digitalWrite(led2, LOW);   // turn the LED on (HIGH is the voltage level)

  */


  //SX70 switches are INPUTS
  //I like to think that the SX70 has mechanical electronics :-)
#if !SONAR
  pinMode(S1, INPUT_PULLUP);
#endif

#if SONAR
  pinMode(S1, INPUT);
  pinMode(S1F, INPUT);
  pinMode(GTD, INPUT);
#endif  

  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S5, INPUT_PULLUP);
  pinMode(S8, INPUT);
  pinMode(S9, INPUT);
  pinMode(FFA, OUTPUT);
  //LIGHTMETER ENABLE AS OUTPUT
  //pinMode(OE_Pin, OUTPUT);
  //ENABLING CHIP (LOW)
  // digitalWrite(OE_Pin, LOW);

#if TCS3200
  pinMode(S1_Pin, OUTPUT);    
  pinMode(S3_Pin, OUTPUT);    
  pinMode(OE_Pin, OUTPUT);    


  digitalWrite(S1_Pin, HIGH); //scaling LOW = 20% HIGH = 100%
  digitalWrite(S3_Pin, LOW); //filter LOW = clear HIGH = green
  digitalWrite(OE_Pin, LOW); //LOW = enabled
#endif
#if TSL237T
  pinMode(OE_Pin, OUTPUT);    
  digitalWrite(OE_Pin, LOW); //LOW = enabled
#endif

  digitalWrite(FFA, LOW);           // make sure I don't fire the flash!

#if SIMPLEDEBUG
  Serial.begin(9600);
  Serial.println("WelcomE to openSX70");
  Serial.print("Version: ");
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));
#endif

  //Motor is OUTPUT
  pinMode(Motor, OUTPUT);


  //I make sure that the Motor is OFF
  digitalWrite(Motor, LOW);

  //I also want the Shutter to be open for SLR-TTL fuctionality

  analogWrite(Solenoid1, 0);
  analogWrite(Solenoid2, 0);

  void intializeDS2408();

  // EEPROM STUFF INITIALIZING SEQUENCE
  //    Serial.begin (9600);
  char initJP[2];
  initJP[0] = EEPROM.read(0);
  initJP[1] = EEPROM.read(1);

  if (initJP[0] != 'S' || initJP[1] != 'X')
  {
#if SIMPLEDEBUG
    Serial.println("Initializing EEPROM....");
#endif

    EEPROM.write(0, 'S');
    EEPROM.write(1, 'X');
    int eeAddress = 101;
    EEPROM.put(10, eeAddress);
    int ActualPicture = 1;
    EEPROM.put(13, ActualPicture);
  }
  else
  {

    if ((switch1 == 1) && (switch2 == 1)) {
#if SIMPLEDEBUG
      Serial.println("RE-nitializing EEPROM....");
#endif

      EEPROM.write(0, 'S');
      EEPROM.write(1, 'X');
      int eeAddress = 101;
      EEPROM.put(10, eeAddress);
      int ActualPicture = 1;
      EEPROM.put(13, ActualPicture);
    }

    EEPROM.get(10, eeAddress); // where to write
    EEPROM.get(13, ActualPicture); //the picture taken counter
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

  selector = 200;
  switch1 = 0;
  switch2 = 0;

  //ISO for dongleless is going to be stored in EEPROM position 20.

  //int cISO is currentISO;

  cISO = EEPROM.read(20);
  Serial.print("cISO read:");
  Serial.println(cISO);
  //       cISO = EEPROM.read(20);

  if ((cISO != 60) && (cISO != 10))
  {
    //int update = 600;
    EEPROM.write(20, 60);
    Serial.println("initializing cISO as 600");
  }
  //      cISO = EEPROM.read(20);
 output_compare = ISO();

 
  //EEPROM.get(20, currentISO);

  device_count = ds.findsingle(&dongleDevice);

  if (device_count != 0)
    initializeDS2408(); //Pitsie fix for non-blinking LED on dongle


  //****************************************************************************************************************************
#if LEDCOUNTER
  //OPTION this is the so called LED counter coment auto this part to disable the counter.
  //LED COUNTER

  //if (digitalRead(S8) != HIGH || digitalRead(S9) != LOW)
  if (digitalRead(S8) == LOW && digitalRead(S9) == LOW && device_count > 0) //NORMAL OPERATION
  {
    simpleBlink(8 - (EEPROM.read(4)),6);
#if SIMPLEDEBUG
    Serial.println("BLINK NUMBER OF SHOTS");
#endif
  }
#endif

  //#if LIGHTMETER
  
#if !LEDCOUNTER
  //indicate type of film blink on startup

  //  byte cISO = EEPROM.read(20);
  if (cISO == 60)
  {
    simpleBlink(2,6);
    output_compare = A600;

  }
  if (cISO == 10)
  {
    simpleBlink(1,6);
    output_compare = A100;

  }
#endif

  //#endif
  //LED COUNTER END
  //****************************************************************************************************************************
  /*
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
  */
  tsl237_init();

  //BRING MIRROR DOWN IF HALFWAY OR UP

  //if (digitalRead(S5) != LOW || digitalRead(S3) != LOW)
  if (DebouncedRead(S5) != LOW || DebouncedRead(S3) != LOW)
  
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
}
