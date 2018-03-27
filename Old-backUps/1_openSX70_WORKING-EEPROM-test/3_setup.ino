void setup() {
// this code only runs once:
//Solenoid #1 and #2 PINS are OUTPUTS

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

//#if ISDEBUG
Serial.begin (9600);
Serial.println ("Welcome to openSX70");
Serial.print ("Version: ");
Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));
//#endif

//Motor is OUTPUT
pinMode(Motor, OUTPUT);

//I make sure that the Motor is OFF
digitalWrite(Motor, LOW);

//I also want the Shutter to be open for SLR-TTL fuctionality

//analogWrite(Solenoid1, 0);
//analogWrite(Solenoid2, 0);

    
    #if LIGHTMETER
    // Light Meter initialization
    lightmeter.begin(luxMode); // Inicialize BH1750
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
    #endif


device_count = ds.find(&devices);

// THIS IS FUNDAMENTAL


// EEPROM STUFF INITIALIZING SEQUENCE
Serial.begin (9600);
    char initJP [2];
    initJP[0] = EEPROM.read(0);
    initJP[1] = EEPROM.read(1);

    if (initJP[0] != 'S' || initJP[1] != 'X')
        {
  Serial.println ("Initializing EEPROM....");
  EEPROM.write(0,'S');
  EEPROM.write(1,'X');
  int eeAddress = 101;
  EEPROM.put (10,eeAddress);
  int ActualPicture= 1;
  EEPROM.put (13,ActualPicture);
  }
      else 
  {

if ((Read_DS2408_PIO(1) == 1) && (Read_DS2408_PIO(2) == 1)) {
  Serial.println ("RE-nitializing EEPROM....");
  EEPROM.write(0,'S');
  EEPROM.write(1,'X');
  int eeAddress = 101;
  EEPROM.put (10,eeAddress);
  int ActualPicture= 1;
  EEPROM.put (13,ActualPicture);
}
Serial.println ("EEPROM already initialized...");

EEPROM.get(10,eeAddress);  // where to write
EEPROM.get (13, ActualPicture); //the picture taken counter
Serial.print ("eeAddress: ");
Serial.println (eeAddress);
Serial.print ("ActualPicture: ");
Serial.println (ActualPicture);

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

simpleBlink (8 - (EEPROM.read (4)));




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
}
