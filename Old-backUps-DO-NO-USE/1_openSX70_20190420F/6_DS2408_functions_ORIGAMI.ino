#if origamiV1
byte Read_DS2408_PIO(int Slot) {

   
   // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
   // Slot = 1 S1          //Return switch 1 on or off
   // Slot = 2 S2         //Return switch 2 on or off
     
  byte selector = B0000;
  //byte selector;
//Serial.print ("readDevice = ");
//Serial.println (readDevice, HEX);

  
   if  ((digitalRead(S2) == HIGH) && ((device_count == 0)))   ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED
    {
   return 200;
    }

  
  if ((digitalRead(S2) == HIGH) && ((device_count > 0))  ) { ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

    // BIT: 7 6 5 4 3 2 1 0
    
    //uint8_t readDevice = ds.get_state(devices[0]);
    
    if (Slot == 0) {

    uint8_t readDevice = ds.get_state(devices[0]);

// for (int index=0; index < device_count; index++) {
//      uint8_t readDevice = ds.get_state(devices[index]); }

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

    byte Switch = 0;
    if (Slot == 2) {
//
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

} //////////////////////////////////END OF Read_DS2408_PIO() function
#endif
