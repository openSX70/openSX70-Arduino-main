#ifndef Camera_h
  #define Camera_h
  #include "Arduino.h"
  #include "uDongle2.h"
  class Camera
  {
    public:
      Camera(uDongle *dongle);
      void shutterCLOSE();
      void shutterOPEN();
      void mirrorDOWN();
      void mirrorUP();
      void darkslideEJECT();
      void motorON();
      void motorOFF();
      void Ydelay ();
      void BlinkTimerDelay(byte led1, byte led2, byte time = 10) ;
      //  uDongle2 *myDongle _Write_DS2408_PIO(byte _port, bool _ON);
      //  uDongle2 *myDongle (pin_S2);
      //  uDongle(pin_S2).myDongle;
      //  int Blink (int interval, int timer, int Pin, byte type);
      void Blink (unsigned int interval, int timer, int Pin, byte type = 1);
      void HighSpeedPWM ();
      void ExposureFinish();
      void AutoExposure(int _myISO, byte _sw1, byte _sw2);
      void ManualExposure(int _selector, byte _sw1, byte _sw2);
      void FlashBAR(byte _sw1, byte _sw2);
      void ShutterB(byte _sw1, byte _sw2);
      void ShutterT(byte _sw1, byte _sw2);
      void BlinkISO();
      void multipleExposureFinish();
      void multipleExposure(int exposureMode);
      void setLIGHTMETER_HELPER(bool active);
      bool getLIGHTMETER_HELPER();
      void switch2Function(int mode);
    private:
      void FastFlash();
      //  void ShutterB();
      //  void ShutterT();
      void DongleFlashF8();
      void DongleFlashNormal();
      void multipleExposureLastClick();
      //  Basics* _Basics;
      bool DebouncedRead(uint8_t pin);
      uDongle * _dongle;
  };
#endif
