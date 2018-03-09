void loop() {

  Serial.println (Read_DS2408_PIO(0));
  
  //WHAT TO DO WHEN POWER-UP:
  //  S8     S9
  //closed  open  --> EJECT DARKSLIDE (DEFAULT)
  // open  closed --> FILM REACH 0 (NO FLASH)
  // open   open  --> NORMAL OPERATION 10 TO 1

 
   // STATE 1: EJECT DARKSLIDE:
  #if MOTOR 
  if (digitalRead(S8) == HIGH && digitalRead(S9) == LOW)
    //EJECT DARK SLIDE
  {
     darkslideEJECT();
                                   #if ISDEBUG 
                                  Serial.println("STATE1: EJECT DARK SLIDE");      
                                  #endif
  }
  #endif
  #if MOTOR 
  //STATE 2: PACK IS EMPTY--> NO WASTE OF FLASH
  if (digitalRead(S8) == LOW && digitalRead(S9) == HIGH)
    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
     {
                                   #if ISDEBUG 
                                  Serial.println("STATE2: PACK IS EMPTY");      
                                  #endif

    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
    // KEEP IN MIND THAT THIS **THE CAMERA** SAYING IT HAS ALREADY MADE 10 SHOTS.
    // I COULD SET MY OWN COUNTER (UP TO 8) AND MAKE IT MORE IMPOSSIBLE-8-SHOTS-FRIENDLY
      Write_DS2408_PIO (6, 1);
//EEPROM 

byte PictureType = 0;
// PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600

//
uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750


//
Picture MyPicture ={ ActualPicture, PictureType, ShutterSpeed, lux};




EEPROM.get (eeAddress,MyPicture);



if (digitalRead(S1) == LOW) 
  {
//EEPROM.get(10,eeAddress);
int ReadAddress = (eeAddress - (sizeof(MyPicture)*8));

//Serial.print("======================= Entering loop =======================");
//Serial.print("eeAddress before loop: ");
//Serial.println (eeAddress);
Serial.begin (9600);
Serial.print("ReadAddress before loop: ");
Serial.println (ReadAddress);
  
for (int i = 0; i < 8; i++)
{
  int thisRecordAddress = ReadAddress + (i * sizeof(MyPicture));
  int sequence = i+1;
//  EEPROM.get(thisRecordAddress, MyPicture);
  Serial.println("=======================================================");
  Serial.print("eeAddress read: ");
  Serial.println (thisRecordAddress);
  Serial.println ("-");
  Serial.print ("Pack order: ");
  Serial.println (sequence);
  Serial.print( " Picture: " );
  Serial.println( MyPicture.StructPicture );
  Serial.print( " Type raw: " );
  Serial.println( MyPicture.StructType );

  // PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600

  Serial.print( " Type: " );
  if (MyPicture.StructType == 0){
  Serial.println( "MANUAL" );  }  
  if (MyPicture.StructType == 1){
  Serial.println( "AUTO 100" ) ; }
  if (MyPicture.StructType == 2){
  Serial.println( "FLASH DONGLELESS" );}
  if (MyPicture.StructType == 4){
  Serial.println( "FLASH F8 DONGLE" ); }
  if (MyPicture.StructType == 6){
  Serial.println( "AUTO 600" );  }
  
  Serial.print( " ShutterSpeed: " );
  Serial.println( MyPicture.StructSpeed );
  Serial.print( " Lux: " );

  Serial.println( MyPicture.StructLux );
  
  delay(500);  
  
}
//Serial.print("======================= After loop =======================");
//Serial.print ("Read: ");
//Serial.println (ReadAddress);
//    delay (1000);
  }

      
}
#endif
#if MOTOR 
  //STATE 3: NORMAL OPERATION
  if (digitalRead(S8) == LOW && digitalRead(S9) == LOW)
  {
                                 #if ISDEBUG 
                                Serial.println("STATE3: NORMAL OPERATION (BIG LOOP)");      
                                 #endif
    
#endif
   
// ///////////////////////////////////PICTURE TAKING OPERATION//////////////////////////////////////////////////
//    FOUR CASES:
//   *   CASE 1 NORMAL OPERATION: FULL CYCLE
//   *  SELECTOR = NORMAL (LOW)
//   *  SHOTS = 0
//   *  S1 = LOW (RED BUTTON PRESSED)
//   *  
//   *  CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
//   *  SELECTOR = DOUBLE (HIGH)
//   *  SHOTS = 0
//   *  S1 = LOW (RED BUTTON PRESSED)
//   *  
//   *  CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
//   *  SELECTOR = DOUBLE (HIGH)
//   *  SHOTS >= 1
//   *  S1 = LOW (RED BUTTON PRESSED)
//   *  CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
//   *  
//   *  SELECTOR = NORMAL (LOW)
//   *  SHOTS >= 1
 
              takePicture= false;                                  
              byte ActualSlot = (Read_DS2408_PIO(0));


              /*
              if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB)))) //////////////POSITION B
                  {
                      Serial.print ("POSITION B:  ");
                      Serial.println( ActualSlot);
                                    
                  #if SHUTTER
                  shutterCLOSE (); 
                  #endif
                  
                  #if MOTOR 
                  mirrorUP();   //Motor Starts: MIRROR COMES UP!!!
                  while (digitalRead(S3) != HIGH)            //waiting for S3 to OPEN
                   ;
                  delay (40);                               //S3 is now open start Y-delay (40ms)
                  #endif
 
                  #if !MOTOR
                  delay (500);
                  #endif

                  #if SHUTTER
                  shutterOPEN ();
                   
                        while (digitalRead(S1) == LOW)
                        ;
                        
                        shutterCLOSE ();
                  #endif
                  
                  #if !MOTOR
                  delay (1000);
                  #endif

                  #if MOTOR
                  delay (200);                             //AGAIN is this delay necessary?
                  mirrorDOWN ();                          //Motor starts, let bring the mirror DOWN
                  delay (200);                             //AGAIN is this delay necessary?
                  #endif

                 #if SHUTTER
                 shutterOPEN();
                 #endif   
                 //shots = 0;  
 
                        

                 return;       
                  
               }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))
                */ 

            int pressTime = REDbutton(S1);
        
       if ((ShutterSpeed[ActualSlot]) != (POSB))
             {
              
            if ((pressTime > shortPress) && (pressTime < longPress)) {
                                       #if ISDEBUG 
                                      Serial.println("---------------------------");
                                      Serial.print ("SHORT:");
                                      Serial.println (pressTime);
                                      Serial.println("---------------------------");
                                      #endif  

            takePicture = true;
              
            }  // END OF if ((pressTime > shortPress) && (pressTime < longPress)) {
            
            else if (pressTime > longPress) {
                                       #if ISDEBUG 
                                      Serial.println("---------------------------");
                                      Serial.print ("LONG: ");
                                      Serial.println (pressTime);
                                      Serial.println("---------------------------");
                                      #endif  

//            BeepTimerDelay();   //Piezo beeps
          BlinkTimerDelay();  //Dongle LED blinks
//          LEDTimerDelay();    //Built-in LED blinks          
            takePicture = true;
             
            }   // END Of else if (pressTime > longPress) 
             }
                                      
          if (Read_DS2408_PIO(0) < 100)  //THIS CASE WE HAVE A PROPER SHUTTER SPEED
                              {
          //byte ActualSlot = (Read_DS2408_PIO(0));

                                      #if ISDEBUG 
                                      byte S1 = Read_DS2408_PIO(1);
                                      byte S2 = Read_DS2408_PIO(2);
                                      Serial.print ("Selector: ");
                                      Serial.print (Read_DS2408_PIO(0));
//                                      Serial.print (ActualSlot);
                                      Serial.print (" / ");
                                      Serial.print (S1);
                                      Serial.print (" / ");
                                      Serial.print (S2);
                                      Serial.print (" Shutter Speed: ");
                                      Serial.println ((ShutterSpeed[Read_DS2408_PIO(0)]));
                                        uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
                                      Serial.print(F("Lux:  "));
                                      Serial.print(lux);
                                      Serial.println(" lx");
                                      delay(500);
                                      #endif



          Dongle (ActualSlot);
          return;
                              }

  if (Read_DS2408_PIO(0) == 100)  //THIS CASE ITS A FLASH PICTURE (FLASH INSERTED IN SX70, NO DONGLE)
  {
                                      #if ISDEBUG 
                                      Serial.print (Read_DS2408_PIO(0));
                                      Serial.println (":  FLASH");
                                      #endif
                                      Flash();
    return;
  }
  if (Read_DS2408_PIO(0) == 200)  //THIS CASE WILL BE AUTO PROBABLY AT 600ASA
  {
                                    #if ISDEBUG 
                                    Serial.print (Read_DS2408_PIO(0));
                                    #endif
                                    #if LIGHTMETER
                                    Serial.println (": Reading LIGHT METER");
                                    uint16_t lux = lightmeter.readLightLevel(); // Reading BH1750
                                    Serial.print(lux);
                                    Serial.println(" lux");
                                    delay(500);
                                    #endif
                                    
    return;
  }
  else
  {
                                  #if ISDEBUG 
                                  Serial.print (Read_DS2408_PIO(0));
                                  Serial.println (":   DONT KNOW, SHOULD NOT BE HERE! ");
                                  #endif
  }


  }
} //END OF loop      

//***************************************************************************************************************************************

