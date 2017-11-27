void setup() {
  // this code only runs once:
  
 
  

  //Solenoid #1 PINS are OUTPUT
  pinMode(Solenoid1, OUTPUT);
  pinMode(Solenoid2, OUTPUT);
  
  //SX70 switches are INPUTS
  //I like to think that the SX70 has mechanical electronics :-)
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S5, INPUT_PULLUP);
  pinMode(S8, INPUT);
  pinMode(S9, INPUT);
  pinMode(FFA, OUTPUT);
  digitalWrite(FFA, LOW);           // make sure I don't fire the flash!
  pinMode(S2, INPUT_PULLUP);  // initialize the pushbutton pin as an input:
  
                                      #if ISDEBUG 
                                      Serial.begin (57600);
                                      Serial.println ("Welcome to openSX70");
                                      #endif 
  
  //pinMode(Selector, INPUT);
  // pinMode (Sol2, OUTPUT);

  //LED are OUTPUTs

  //Motor is OUTPUT
  pinMode(Motor, OUTPUT);

  //I make sure that the Motor is OFF
  digitalWrite(Motor, LOW);

  //I also want the Shutter to be open for SLR-TTL fuctionality
  analogWrite(Solenoid1, 0);
  analogWrite(Solenoid2, 0);

   device_count = ds.find(&devices);
  // THIS IS FUNDAMENTAL
  
 /* 
  //This is just in case the mirror is (for whatever reason) halfway when I power up I want to run this JUST ONCE
  while (digitalRead(S5) != LOW || digitalRead(S3) != LOW)  // while S5 or S3 are different than CLOSED (mirror at an angle)
  {
  void motorON ();
  void motorOFF();
  }
  */
}
  
//***************************************************************************************************************************************
