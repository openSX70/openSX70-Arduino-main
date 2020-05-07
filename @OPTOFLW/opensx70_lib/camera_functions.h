/*
**the openSX70 project**

  It is many things at once, but simply put, openSX70 is an open source
  (hardware and software) project that aims to take the SX70 beyond what
  is possible now in a cheap and non destructive way.
  https://opensx70.com/

  https://github.com/openSX70

  As a legal reminder please note that the code and files is under Creative Commons
  "Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)" is free and open
  for hobbyist NON-COMMERCIAL USE.
  https://creativecommons.org/licenses/by-nc/4.0/

  You are free to:
  Share — copy and redistribute the material in any medium or format
  Adapt — remix, transform, and build upon the material
  The licensor cannot revoke these freedoms as long as you follow the license terms.
  Under the following terms:
  Attribution — You must give appropriate credit, provide a link to the license,
  and indicate if changes were made. You may do so in any reasonable manner, but
  not in any way that suggests the licensor endorses you or your use.

  NonCommercial — You may not use the material for commercial purposes.

  No additional restrictions — You may not apply legal terms or technological
  measures that legally restrict others from doing anything the license permits.

  Notices:
  You do not have to comply with the license for elements of the material in the
  public domain or where your use is permitted by an applicable exception or limitation.

  No warranties are given. The license may not give you all of the permissions necessary
  for your intended use. For example, other rights such as publicity, privacy, or moral r
  ights may limit how you use the material.


*/
#ifndef Camera_h
  #define Camera_h
  #include <Arduino.h>
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
      void AutoExposure(int _myISO);
      void ExposureFinish();
      void ManualExposure(int _selector);
      void FlashBAR();
      void ShutterB();
      void ShutterT();
      void BlinkISO();
      void multipleExposureFinish();
      void multipleExposure(int exposureMode);
      void setLIGHTMETER_HELPER(bool active);
      bool getLIGHTMETER_HELPER();
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
