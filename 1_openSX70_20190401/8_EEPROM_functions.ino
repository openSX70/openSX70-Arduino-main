
void eepromUpdate ()
{
  
                            //EEPROM OK
                        EEPROM.get (13,ActualPicture);
                        CurrentPicturePack = (CurrentPicturePack+1);
                     
                      #if ISDEBUG
                      Serial.print ("POSITION:  ");
                      Serial.println( Read_DS2408_PIO(0));
                      #endif
//                        byte PictureType = 0;

                  byte Shutter = (ShutterSpeed[Read_DS2408_PIO(0)]);
//                  Picture MyPicture ={ ActualPicture, CurrentPicture, PictureType, Shutter, lux};

                  Picture MyPicture ={ ActualPicture, CurrentPicturePack, PictureType, Shutter, counter, sensorValueHIGH };


                      Serial.println ("*****************************");
                      Serial.println (ActualPicture);
                      Serial.println (CurrentPicturePack);
                      Serial.println (PictureType);
                      Serial.println (Shutter);
                      Serial.println (counter);
                      Serial.println (sensorValueHIGH);
                      Serial.println ("*****************************");
                      

                   //EEPROM
                 EEPROM.write(4,CurrentPicturePack);
                 Serial.print ("Picture taken: ");
                  Serial.println (EEPROM.read (4));  
                  EEPROM.put(eeAddress,MyPicture);
                   //   EEPROM.get (eeAddress,MyPicture);
                      eeAddress += sizeof(MyPicture);  //Next eeAdress
                      if(eeAddress+sizeof(MyPicture) >= EEPROM.length()) {eeAddress = 0;}  //check for address overflow
                    
                  //     EEPROM.update (10,eeAddress);
                  //    ActualPicture = ActualPicture+1;
                  //    EEPROM.update (13,ActualPicture);
                  
                       EEPROM.put (10,eeAddress);
                      ActualPicture = ActualPicture+1;
                      EEPROM.put (13,ActualPicture);    

                 return;     
}

void eepromDump ()
{

//EEPROM 


byte PictureType = 0;
// PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600


//
// int ActualPicture;
//byte CurrentPicture;
//byte PictureType;
//int ShutterSpeed;

//Picture MyPicture = {ActualPicture,CurrentPicture, PictureType, eepromSpeed,  lux};
//Picture MyPicture = {ActualPicture,CurrentPicturePack, PictureType, eepromSpeed };

  Picture MyPicture ={ ActualPicture, CurrentPicturePack, PictureType, eepromSpeed, Counter, sensorValueHIGH };

EEPROM.get (eeAddress,MyPicture);

EEPROM.get(10,eeAddress);

int ReadAddress  = (eeAddress - (sizeof(MyPicture)*8));

Serial.println("======================= Entering loop =======================");
Serial.print("INITIAL eeAddress before loop: ");
Serial.println (eeAddress);
  
Serial.print("ReadAddress before loop: ");
Serial.println (ReadAddress);
  
for (int i = 0; i < 8; i++)

{
int thisRecordAddress = ReadAddress + (i * sizeof(MyPicture));
 // int sequence = i+1;
// 

  EEPROM.get(thisRecordAddress, MyPicture);
  Serial.println("=======================================================");
  Serial.print("eeAddress read (thisRecordAddress): ");
  Serial.println (thisRecordAddress);
//  Serial.print ("Pack: ");
//  Serial.println (Pack);
//  Serial.print ("Pack order: ");
//  Serial.println (sequence);
  Serial.print( " Picture: " );
  Serial.println( MyPicture.StructPicture );
  Serial.print ("Current Picture: ");
  Serial.println (MyPicture.StructPackPicture);
  Serial.print( " Type raw: " );
  Serial.println( MyPicture.StructType );

  // PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600
// PictureType = +10 ---> MIRROR DELAY
// PictureType = +100 ---> MULTIPLE EXPOSURE
// PictureType = +200 ---> TIMER DELAY


  Serial.print( " Type: " );
  if (MyPicture.StructType == 0){
  Serial.println( "MANUAL" );  }    
  if (MyPicture.StructType == 1){
  Serial.println( "AUTO 100" ) ; }
  if (MyPicture.StructType == 2){
  Serial.println( "FLASH DONGLELESS" );}
  if (MyPicture.StructType == 4){
  Serial.println( "FLASH F8 DONGLE" ); }
  if (MyPicture.StructType == 5){
  Serial.println( "FLASH DONGLE" ); }
  if (MyPicture.StructType == 6){
  Serial.println( "AUTO 600" );  }
  if (MyPicture.StructType == 7){
  Serial.println( "BULB" );  }
  if (MyPicture.StructType == 8){
  Serial.println( "T" );  }
  Serial.print( " ShutterSpeed: " );
  Serial.println( MyPicture.StructSpeed );

  Serial.print( " LOW: " );
  Serial.println( MyPicture.StructLightVlow );
  Serial.print( " HIGH: " );
  Serial.println( MyPicture.StructLightVhigh );

  
}
   Pack = Pack+1;
 //  return;
}

//*************************************************************************************************

void eepromDumpCSV ()


{
//    int ActualPicture;
//    byte CurrentPicture;
//    byte PictureType;
//    int ShutterSpeed;

//  Picture MyPicture = {ActualPicture,CurrentPicture, PictureType, eepromSpeed, lux};

  Picture MyPicture = {ActualPicture,CurrentPicturePack, PictureType, eepromSpeed, counter, sensorValueHIGH };

      EEPROM.get(10,eeAddress);

     Serial.begin (9600);

     Serial.println("eeAddress,Pack,Pack order,Picture,Type Raw, Type, ShutterSpeed, Lux");

int ReadAddress  = (eeAddress - ((sizeof(MyPicture)*8)*Pack));

//Serial.println("======================= Entering loop =======================");
//Serial.print("INITIAL eeAddress before loop: ");
//Serial.println (eeAddress);
  
//Serial.print("ReadAddress before loop: ");
//Serial.println (ReadAddress);
  
for (int i = 0; i < 8; i++)

{
  int thisRecordAddress = ReadAddress + (i * sizeof(MyPicture));

  EEPROM.get(thisRecordAddress, MyPicture);
  Serial.print (thisRecordAddress);
  Serial.print (",");
  Serial.print (Pack);
  Serial.print (",");
  Serial.print (MyPicture.StructPackPicture);
  Serial.print (",");
  Serial.print( MyPicture.StructPicture );
  Serial.print (",");
  Serial.print( MyPicture.StructType );
//  Serial.println (CurrentPicture);
//  Serial.print (",");

  // PictureType = 0 ---> MANUAL
// PictureType = 1 ---> A100
// PictureType = 2 ---> FLASH DONGLELESS
// PictureType = 4 ---> FLASH F8 DONGLE 
// PictureType = 6 ---> A600
// PictureType = 7 ---> B


  if (MyPicture.StructType == 0){
  Serial.print( ",MANUAL," );  }    
  if (MyPicture.StructType == 1){
  Serial.print( ",AUTO 100," ) ; }
  if (MyPicture.StructType == 2){
  Serial.print( ",FLASH DONGLELESS," );}
  if (MyPicture.StructType == 4){
if (MyPicture.StructType == 5){
  Serial.print( ",FLASH DONGLE," ); }

  Serial.print( ",FLASH F8 DONGLE," ); }
  if (MyPicture.StructType == 6){
  Serial.print( ",AUTO 600," );  }
  if (MyPicture.StructType == 7){
  Serial.print( ",BULB," );  }

  if (MyPicture.StructType == 8){
  Serial.print( ",T," );  }

  
  Serial.print( MyPicture.StructSpeed );
  Serial.print (",");

//  Serial.println( MyPicture.StructLux );

  
  //delay(500); 
  
//added return 
  //return;
}
//Serial.print("======================= After loop =======================");
//Serial.print ("Read: ");
//Serial.println (ReadAddress);
//    delay (1000);
  //}
  Pack = Pack+1;
  return;
}
