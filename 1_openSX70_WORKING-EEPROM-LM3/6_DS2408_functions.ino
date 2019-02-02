//***************************************************************************************************************************************
// --------------------------------------------------------------------------------------------------------------------------------------

//                                DS2408 FUNCTIONS

// --------------------------------------------------------------------------------------------------------------------------------------
//***************************************************************************************************************************************
/*
void loop() {

  // TEST Write_DS2408_PIO function!
  //***********************************************************************
  while (digitalRead(buttonPin) == LOW) {
    Write_DS2408_PIO (DSled, 1);
  }
  {
    Write_DS2408_PIO (DSled, 0);

  }
  //***********************************************************************

*/
/*
  if (DS2408(0) < 100)
  {
    byte ActualSlot = DS2408(0);
    Serial.print ("Selector: ");
    Serial.println (ActualSlot);

    byte S1 = DS2408(1);
    Serial.print ("S1: ");
    Serial.println (S1);

    byte S2 = DS2408(2);
    Serial.print ("S2: ");
    Serial.println (S2);
    return;
  }
  if (DS2408(0) == 100)
  {
    Serial.println ("FLASH");
    return;
  }
  if (DS2408(0) == 200)
  {
    Serial.println ("NOTHING");
    return;
  }
  else
  {
    Serial.println ("DONT KNOW ");
  }
  //  int DS2408();
  return;

} // END OF loop
//******************************************************************************************************
*/

byte Read_DS2408_PIO(int Slot) {
   // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
   // Slot = 1 S1
   // Slot = 2 S2
     
  byte selector = B0000;
  uint8_t readDevice = ds.get_state(devices[0]);
  if ((digitalRead(S2) == HIGH) && (readDevice > 0)) { ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

    // BIT: 7 6 5 4 3 2 1 0
    if (Slot == 0) {

      uint8_t readDevice = ds.get_state(devices[0]);
#if !origamiV1

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

    byte Switch;
    if (Slot == 1) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00010000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    }
    if (Slot == 2) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00100000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    } else
      return 0;
#endif
#if origamiV1

      if (readDevice & 0b00000001) {
        bitSet(selector, 3);
      } else bitClear(selector, 3);
      if (readDevice & 0b00000010) {
        bitSet(selector, 2);
      } else bitClear(selector, 2);
      if (readDevice & 0b00000100) {
        bitSet(selector, 1);
      } else bitClear(selector, 1);
      if (readDevice & 0b000001000) {
        bitSet(selector, 0);
      } else bitClear(selector, 0);
      return selector;
    } //END OF Slot=0

    byte Switch;
    if (Slot == 2) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00010000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    }
    if (Slot == 1) {
      uint8_t readDevice = ds.get_state(devices[0]);

      if (readDevice & 0b00100000) {
        bitSet(Switch, 0);
      } else bitClear(Switch, 0);
      return Switch;
    } else
      return 0;
#endif


  } // END OF if ((digitalRead(S2) == HIGH) && (device_count > 0)){  //CASE DONGLE INSERTED

pinMode(S2, INPUT_PULLUP); // perhaps not such a good idea internal pullup??

  if (digitalRead(S2) == LOW)  { 
    //////////////////////////////////////////////////////////// CASE FLASH
    return 100; // FLASH

  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH

  return 200; //NOTHING

} //END OF Read_DS2408_PIO() function

//******************************************************************************************************
byte Write_DS2408_PIO(byte port, bool ON) {
// This is to turn on LED P6 or PC-FLASH socket (opto) P7
// port 6 = P6 = LED
// port 7 = P7 = PC-FLASHM
  byte OutPIO = B00000000;
  
  if (ON == true )
  {
    uint8_t readDevice = ds.get_state(devices[0]);
    bitSet(OutPIO, port);
    ds.set_state(devices[0], ~OutPIO);
    return;
  } else {
    uint8_t readDevice = ds.get_state(devices[0]);
    bitClear(OutPIO, port);
    ds.set_state(devices[0], ~OutPIO);
    return;
  }
} //END OF Write_DS2408_PIO
//******************************************************************************************************
