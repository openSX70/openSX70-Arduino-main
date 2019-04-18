byte Read_DS2408_PIO(int Slot) {

  device_count = ds.find(&devices);

  if (digitalRead(S2) == LOW)  {  //////////////////////////////////////////////////////////// CASE FLASH
    Serial.println ("FLASH INSERTED");

    return 100;
        
  } // END OF if (digitalRead(S2) == LOW)  {  // CASE FLASH
   
  device_count = ds.find(&devices);

if ((digitalRead(S2) == HIGH) && (device_count > 0)){  ////////////////////////////////////////////////////////////CASE DONGLE INSERTED

Serial.println("openSX70 Dongle connected!");

// BIT: 7 6 5 4 3 2 1 0

byte S1 = B0000; 
byte S2 = B0000; 
byte selector = B0000;


#if !origamiV1      

       uint8_t readDevice = ds.get_state(devices[0]);

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
//    } //END OF Slot=0

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

        uint8_t readDevice = ds.get_state(devices[0]);


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


/*
#if origamiV1
 for(int index=0; index < device_count; index++) {
      uint8_t readDevice = ds.get_state(devices[index]);
      if (readDevice & 0b00000001) {
                bitSet(selector, 3);
                  }
      
      if (readDevice & 0b00000010) {
                bitSet(selector, 2);
                  }
      if (readDevice & 0b00000100) {
                bitSet(selector, 1);
                  }
      if (readDevice & 0b000001000) {
                bitSet(selector, 0);
                  }
#endif
#if !origamiV1
 for(int index=0; index < device_count; index++) {
      uint8_t readDevice = ds.get_state(devices[index]);
      if (readDevice & 0b00000001) {
                bitSet(selector, 0);
                  }
      
      if (readDevice & 0b00000010) {
                bitSet(selector, 1);
                  }
      if (readDevice & 0b00000100) {
                bitSet(selector, 2);
                  }
      if (readDevice & 0b000001000) {
                bitSet(selector, 3);
                  }
                        
      if (readDevice & 0b00010000) {
                bitSet(S1, 0);
               //ds.set_state(devices[index], ~0b01000000); 
                ds.set_state(devices[index], ~0b01000000); 
                } else {
                ds.set_state(devices[index], ~0b00000000);
                //ds.set_state(devices[index], ~0b11111111);  
                 }

      if (readDevice & 0b00100000) {
                bitSet(S2, 0);
                }
                }

#endif                  */

#if origamiV1
     if (readDevice & 0b00010000) {
                bitSet(S2, 0);
               //ds.set_state(devices[index], ~0b01000000); 
                ds.set_state(devices[index], ~0b01000000); 
                } else {
                ds.set_state(devices[index], ~0b00000000);
                //ds.set_state(devices[index], ~0b11111111);  
                 }

      if (readDevice & 0b00100000) {
                bitSet(S1, 0);
                }
                }
#endif

  Serial.print ("selector: ");
  Serial.println (selector );

  Serial.print("switch 1: ");
  Serial.println(S1, BIN);
 
  Serial.print("switch 2: ");
  Serial.println(S2, BIN);

  return;
  } // END OF if ((digitalRead(S2) == HIGH) && (device_count > 0)){  //CASE DONGLE INSERTED


    device_count = ds.find(&devices);
  
  if  ((device_count == 0) && (digitalRead(S2) == HIGH)) {  ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED

   Serial.println("NOTHING CONNECTED");
   return 200;
  } // END OF if  ((device_count == 0) && (digitalRead(S2) == HIGH)) {  //CASE NOTHING CONNECTED

}
