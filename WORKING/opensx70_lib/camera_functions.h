#ifndef Camera_h
  #define Camera_h
  #include "Arduino.h"
  #include "udongle2.h"
  class Camera
  {
    public:
      Camera(uDongle *dongle);
      void S1F_Unfocus();
      void S1F_Focus();
      void shutterCLOSE();
      void shutterOPEN();
      void solenoid_init();
      void sol2Engage();
      void sol2LowPower();
      void sol2Disengage();
      void mirrorDOWN();
      void mirrorUP();
      void darkslideEJECT();
      void motorON();
      void motorOFF();
      void Ydelay ();
      void BlinkTimerDelay(byte led1, byte led2, byte time = 10) ;
      void Blink (unsigned int interval, int timer, int Pin, byte type = 1);
      void Blink (unsigned int interval, int timer, int PinDongle, int PinPCB, byte type);
      void AutoExposure(int _myISO);
      void AutoExposureFF(int _myISO);
      void ExposureFinish();
      void ManualExposure(int _myISO,uint8_t selector);
      void VariableManualExposure(int _myISO,uint8_t selector);
      void FlashBAR();
      void ShutterB();
      void ShutterT();
      void multipleExposureFinish();
      void multipleExposure(int exposureMode);
      bool setLIGHTMETER_HELPER(bool active);
      bool getLIGHTMETER_HELPER();
      void multipleExposureLastClick();
    private:
      void FastFlash();
      void AperturePriority();
      void DongleFlashF8();
      void DongleFlashNormal();
      bool DebouncedRead(uint8_t pin);
      uDongle * _dongle;
  };
#endif
