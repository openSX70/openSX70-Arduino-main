#ifndef Camera_h
  #define Camera_h
  #include "Arduino.h"

  extern HardwareSerial DEBUG_OUTPUT;
  class Camera
  {
    public:
      Camera(void);
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
      void AutoExposure(int _myISO);
      void AutoExposureFF(int _myISO);
      void ExposureFinish();
      void ManualExposure(int _myISO,uint8_t selector);
      void ShutterB();
      void ShutterT();
      void multipleExposureFinish();
      void multipleExposure(int exposureMode);
      bool setLIGHTMETER_HELPER(bool active);
      bool getLIGHTMETER_HELPER();
      void multipleExposureLastClick();
    private:
      void FastFlash();
      bool DebouncedRead(uint8_t pin);
      //uDongle * _dongle;
  };
#endif
