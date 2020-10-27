#include "Arduino.h"
#include "settings.h"
//
#include "EEPROM.h"

void WritePicture(int _currentPicture){
  EEPROM.update(EE_ADD_PIC, _currentPicture);
  return;
}

int ReadPicture(){
  int _currentPicture;
  EEPROM.get(EE_ADD_PIC, _currentPicture);
  return _currentPicture;
}
void WriteISO(int _currentISO){
  EEPROM.put(EE_ADD_ISO, _currentISO);
  return;
}

int ReadISO()
{
  int _currentISO;
  EEPROM.get(EE_ADD_ISO, _currentISO);
  return _currentISO;
}

void init_EEPROM() {
  char initJP[2];
  initJP[0] = EEPROM.read(0);
  initJP[1] = EEPROM.read(1);
  //int currentPicture = 1;
  //const uint8_t EE_ADD_PIC = 13;
  if (initJP[0] != 'S' || initJP[1] != 'X')
  {
#if SIMPLEDEBUG
    Serial.println("Initializing EEPROM....");
#endif

    EEPROM.write(0, 'S');
    EEPROM.write(1, 'X');
    //    int eeAddress = 101;
//    DEFAULT_ISO = 640;
    WritePicture(0);
    WriteISO(DEFAULT_ISO);
//    EEPROM.put(EE_ADD_PIC, currentPicture);
//    EEPROM.put(EE_ADD_ISO, DEFAULT_ISO);
      return;
  } 

/*   
   int _myISO = ReadISO ();
      if ((_myISO !=  ISO_600) || (_myISO !=  ISO_SX70))
   {
    Serial.print ("ERROR read ISO: ");
    Serial.println (_myISO);
    Serial.print ("ISO_600: ");
    Serial.println (ISO_600);
    Serial.print ("ISO_SX70: ");
    Serial.println (ISO_SX70);    
    Serial.print ("DEFAULT ISO DEFINED: ");
    Serial.println (DEFAULT_ISO);
    WriteISO (DEFAULT_ISO);
    return;
   }  
    Serial.print ("ISO read correctly: ");
//    Serial.println (_myISO);
 */   
}
