void ManualExposure()
{
#if SIMPLEDEBUG
  Serial.println ("take Manual picture");
#endif
  byte PictureType = 0;
  //                    eepromUpdate ();

  shutterCLOSE ();

  //                  delay (200); //added to fix bad photos
  delay (100); //added to fix bad photos WITH LESS delay

  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
    ;
  Ydelay();

  startCounterCalibration();

#if SIMPLEDEBUG
  Serial.print ("--------------------------------------------------CLICK:  ");
  Serial.println (ShutterSpeed[selector]);
#endif

    int ShutterSpeedDelay = (ShutterSpeed[selector]+ShutterConstant) ;
#if SIMPLEDEBUG

	Serial.print("ShutterSpeed[");
	Serial.print(selector);
	Serial.print("] :");
	Serial.println(ShutterSpeed[selector]);

	Serial.print("ShutterConstant:");
	Serial.println(ShutterConstant);

	Serial.print("ShutterSpeedDelay:");
	Serial.println(ShutterSpeedDelay);

#endif

	shutterOPEN();  //SOLENOID OFF MAKES THE SHUTTER TO OPEN!
	delay (ShutterSpeedDelay);        

if (ShutterSpeedDelay >= FastestFlashSpeed)
     {
      FastFlash ();
#if SIMPLEDEBUG
	  Serial.println("FF");
#endif
     }
    shutterCLOSE ();                                         //close the shutter
    delay (10);
    cli();
    counter = TCNT1;
    sei();
	finish();
/*
//	Serial.print("Counter:");
//	Serial.println(counter);
  if (switch1 == 1)
  {
    shots = ++shots;
    return;
  } else if (switch1 == 0)
  {
    delay (100);                             //AGAIN is this delay necessary?
    mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
    delay (200);                             //AGAIN is this delay necessary?
    shutterOPEN();
    shots = 0;
    return;
  }
  */
  return;
}
