#ifndef uDongle_h
  #define uDongle_h
  
  #if (ARDUINO >=100)
    #include "Arduino.h"
  #else
    #include "WProgram.h" //??
  #endif
  #include "DS2408.h"

  struct status{
    uint8_t selector;
    bool switch1;
    bool switch2;
  };
  
  class uDongle {
    public:
      // Constructor 
      uDongle(uint8_t pin);
      
      // Methods
      void initDS2408();
      void Write_DS2408_PIO(byte port, bool ON);
      byte checkDongle();
      void dongleLed (byte led,bool on);
      void simpleBlink (int _times, int _led);
      void bothBlink (int _times);
      void doubleBlink (int _times);
      status get_peripheral_status();
      
 
    private:
      byte Read_DS2408_PIO(int slot);
      
      uint8_t _Pin;
      uint8_t _device_count;
      DS2408* _ds;
      Device  _dongleDevice;
      status peripheral_status;
  };
#endif
