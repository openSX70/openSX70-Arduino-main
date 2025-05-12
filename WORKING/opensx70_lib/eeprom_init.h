#ifndef eeprom_init_h
  #define eeprom_init_h
  #include "Arduino.h"
  void init_EEPROM();
  void WriteISO(int _currentISO);
  int ReadISO();
#endif
