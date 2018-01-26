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

#if ISDEBUG
Serial.begin (9600);
Serial.println ("Welcome to openSX70");
#endif

//Motor is OUTPUT
pinMode(Motor, OUTPUT);

//I make sure that the Motor is OFF
digitalWrite(Motor, LOW);

//I also want the Shutter to be open for SLR-TTL fuctionality

analogWrite(Solenoid1, 0);
analogWrite(Solenoid2, 0);

    
    #if LIGHTMETER
    // Light Meter initialization
    lightmeter.begin(luxMode); // Inicialize BH1750
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
    #endif


device_count = ds.find(&devices);

// THIS IS FUNDAMENTAL


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
