#if !origamiV1
//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                DS2408 FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------


byte Read_DS2408_PIO(int Slot) {


  // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
  // Slot = 1 S1          //Return switch 1 on or off
  // Slot = 2 S2         //Return switch 2 on or off

  byte selector = B0000;
  //Serial.print ("readDevice = ");
  //Serial.println (readDevice, HEX);


  if  (((device_count == 0)) && (digitalRead(S2) == HIGH))   ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED
  {
    return 200;
  }


  if ((digitalRead(S2) == HIGH) && ((device_count > 0))  ) { ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

    // BIT: 7 6 5 4 3 2 1 0


    if (Slot == 0) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00000001) {
        bitSet(selector, 0);
      } else bitClear(selector, 0);
      if (readDevice & 0b00000010) {
        bitSet(selector, 1);
      } else bitClear(selector, 1);
      if (readDevice & 0b00000100) {
        bitSet(selector, 2);
      } else bitClear(selector, 2);
      if (readDevice & 0b000001000) {
        bitSet(selector, 3);
      } else bitClear(selector, 3);
      return selector;
    } //END OF Slot=0

    byte Switch = 0;
    if (Slot == 1) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00010000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    }
    if (Slot == 2) {

      uint8_t readDevice = ds.get_state(dongleDevice);

      if (readDevice & 0b00100000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    } else
      return 0;

  } // END OF if ((digitalRead(S2) == HIGH) && (ds.find(&devices); > 0)){  //CASE DONGLE INSERTED

  //pinMode(S2, INPUT_PULLUP); // perhaps not such a good idea internal pullup??

  //   readDevice = ds.get_state(devices[0]);
  if (digitalRead(S2) == LOW)  {
    //////////////////////////////////////////////////////////// CASE FLASH
    return 100; // FLASH

  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH

  if  ((device_count == 0) && (digitalRead(S2) == HIGH)) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED

    //   Serial.println("NOTHING CONNECTED");
    //delay (1000);
    return 200; //NOTHING

  }

  return 200;
} //////////////////////////////////END OF Read_DS2408_PIO() function




//******************************************************************************************************
void Write_DS2408_PIO(byte port, bool ON) {
  // This is to turn on LED P6 or PC-FLASH socket (opto) P7
  // port 6 = P6 = LED
  // port 7 = P7 = PC-FLASHM
  byte OutPIO = B00000000;

  if (ON == true )
  {
    uint8_t readDevice = ds.get_state(dongleDevice);
    bitSet(OutPIO, port);
    ds.set_state(dongleDevice, ~OutPIO);
    return;
  } else {
    uint8_t readDevice = ds.get_state(dongleDevice);
    bitClear(OutPIO, port);
    ds.set_state(dongleDevice, ~OutPIO);
    return;
  }
} //END OF Write_DS2408_PIO
//******************************************************************************************************




void initializeDS2408()
{
  //INTITIALIZE DS2408
  //ds.find(&devices); = ds.find(&devices);
  ds.reset();
  ds.write(0x96);
  for (int i = 0; i < 8; i++)
    ds.write(dongleDevice[i]);
  ds.write(0x3C);
  ds.reset();
  // THIS IS FUNDAMENTAL
}
#endif
byte SelectorAfter8()
{
	byte label;
	byte previousRead;
	int j = 1; // integer used in scanning the array designating column number
  //2-dimensional array for asigning the buttons and there high and low values
  /*
   * TEST for After8 dongle
   * R100 = 50K  (2) --> S2-
   * R101 = 120K (8) --> S2-
   * R102 = 100K (1) --> S2-
   * R103 = 196K (4) --> S2-
   *
   * Common R105 = 10K (S2+ --> VCC)
   */
	int SelectorPos[16][3] = { {0, 1022,1023},  // Pos 0  --> 2000
							  {1, 980,995},   // Pos 1  --> OFF
							  {2, 1010,1021},   // Pos 2  --> A100
							  {3, 940,950},   //  Pos 3  --> A600
							  {4, 890,904},   //  Pos 4  --> T
							  {5, 835,850},   //  Pos 5  --> B
							  {6, 855,870},   //  Pos 6  --> 2
							  {7, 808,820},   //  Pos 7  --> 4
							  {8, 965,975},   //  Pos 8  --> 8
							  {9, 905,915},   //  Pos 9  --> 15
							  {10, 925,935},  //  Pos A  --> 30
							  {11, 875,885},  //  Pos B  --> 60
							  {12, 825,835},  //  Pos C  --> 125
							  {13, 780,785},  //  Pos D  --> 250
							  {14, 795,805},  //  Pos E  --> 500
							  {15, 750,770} }; //  Pos F  --> 1000 


	 // loop for scanning the button array.
	for (int i = 0; i <= 15; i++)
	{
		int j = 1;
		// checks the ButtonVal against the high and low vales in the array
		if ((analogRead(S2)) >= SelectorPos[i][j] && (analogRead(S2)) <= SelectorPos[i][j + 1])
		{
			label = SelectorPos[i][0];

		}
	}
	/*
	Serial.print ("label: ");
	Serial.println (label);
	Serial.print ("previousRead: ");
	Serial.println(previousRead);
	*/
	if ((label == 0) && ((previousRead != 1) || (previousRead != 15)))
	{
		label = 200;

	}
	else
	{

		previousRead = label;
		//label = 200;
	}

	return label;

}
