#include "DS2408.h"
#include "peripheral.h"
#include "settings.h" // included because of for #if ORIGAMIV1

byte previous_count = 0; //for debouncing

uDongle::uDongle (uint8_t pin) 
{
  _ds = new DS2408(pin);
  Device  _dongleDevice;
  _Pin = pin;
}

void uDongle::initDS2408() //INTITIALIZE DS2408
{
  //ds.find(&devices); = ds.find(&devices);
  _device_count = _ds->findsingle(&_dongleDevice);
  _ds->reset();
  _ds->write(0x96);
  for (int i = 0; i < 8; i++){
    _ds->write(_dongleDevice[i]);
  }
  _ds->write(0x3C);
  _ds->reset();
  previous_count = selector(); //For Debouncing
  #if ROTARYDEBUG
  Serial.println(F("previous count on init:"));
  Serial.println(previous_count);
  #endif
  // THIS IS FUNDAMENTAL
  //  DS2408 ds(_Pin);
  //  Device _dongleDevice;
  //int _device_count = 0;
  //DS2408 ds(ONE_WIRE_BUS_PORT);
  //Device dongleDevice;
}

byte uDongle::Read_DS2408_PIO(int slot)
{
  //  Device  _dongleDevice;
  //
  uint8_t readDevice;
  //  uint8_t _device_count;
  //  byte slot;
  // Slot = 0 selector    // return value 0-15 (selector) 100 = Flash inserted 200 = NOTHING INSERTED
  // Slot = 1 S1          //Return switch 1 on or off
  // Slot = 2 S2         //Return switch 2 on or off
  byte _Selector = B0000;
  //Serial.print("readDevice = ");
  //Serial.println(readDevice, HEX);
  pinMode(_Pin, INPUT_PULLUP); //changed from INPUT_PULLUP 27_04
  if (digitalRead(_Pin) == LOW)   //////////////////////////////////////////////////////////// CASE FLASH
  {
    return 100; // FLASH
    Serial.println(F("flash"));
  }
  if  ((_device_count == 0) && (digitalRead(_Pin) == HIGH)) ////////////////////////////////////////////////////////////CASE NOTHING CONNECTED
  {  
    return 200; //NOTHING
    Serial.println(F("nothing"));
  }

  if (slot == 0) 
  { 
    readDevice = _ds->get_state(_dongleDevice);
    #if UDONGLE
      if (readDevice & 0b00000001) {
        bitSet(_Selector, 0);
      } else
      {
        bitClear(_Selector, 0);
      }
      if (readDevice & 0b00000010)
      {
        bitSet(_Selector, 1);
      } else 
      {
        bitClear(_Selector, 1);
      }
      if (readDevice & 0b00000100) 
      {
        bitSet(_Selector, 2);
      } else
      {
        bitClear(_Selector, 2);
      }
      if (readDevice & 0b000001000)
      {
        bitSet(_Selector, 3);
      } else{
        bitClear(_Selector, 3);
      }
      return _Selector;
    #endif
    #if ORIGAMIV1
      if (readDevice & 0b00000001) {
        bitSet(_Selector, 3);
      } else
      {
        bitClear(_Selector, 3);
      }
      if (readDevice & 0b00000010)
      {
        bitSet(_Selector, 2);
      } else 
      {
        bitClear(_Selector, 2);
      }
      if (readDevice & 0b00000100) 
      {
        bitSet(_Selector, 1);
      } else
      {
        bitClear(_Selector, 1);
      }
      if (readDevice & 0b000001000)
      {
        bitSet(_Selector, 0);
      } else{
        bitClear(_Selector, 0);
      }
      
      return _Selector;
    #endif
  }//END OF Slot=0
  byte  _Switch = 0;
  if (slot == 1) {
    readDevice = _ds->get_state(_dongleDevice);
    if (readDevice & 0b00010000) {
      bitSet(_Switch, 0);
    } else
    {
      bitClear(_Switch, 0);
    }
  return _Switch;
  }
  if (slot == 2)
  {
    readDevice = _ds->get_state(_dongleDevice);
    if (readDevice & 0b00100000)
    {
      bitSet(_Switch, 0);
    }else
    {
      bitClear(_Switch, 0);
    }
    return _Switch;
  } else{
    return 0;
  }
}

void uDongle::Write_DS2408_PIO(byte port, bool ON) 
{
  // This is to turn on LED P6 or LED P7
  // port 6 = P6 = LED1 
  // port 7 = P7 = LED2 (was flash)
  byte OutPIO = B00000000;

  if (ON == true )
  {
    uint8_t readDevice = _ds->get_state(_dongleDevice);
    bitSet(OutPIO, port);
    _ds->set_state(_dongleDevice, ~OutPIO);
  } 
  else
  {
    uint8_t readDevice = _ds->get_state(_dongleDevice);
    bitClear(OutPIO, port);
    _ds->set_state(_dongleDevice, ~OutPIO);
  }
} //END OF Write_DS2408_PIO

byte uDongle::checkDongle()
{
  _device_count = _ds->findsingle(&_dongleDevice);
  return _device_count;
}
//int recursioncounter= 0;


byte uDongle::selector()
{
    byte sel = uDongle::Read_DS2408_PIO(0);
    //Serial.print("selectorread: ");
    //Serial.println(sel);
    return sel;
}


byte uDongle::switch1()
{
    byte sw = uDongle::Read_DS2408_PIO(1);
    return sw;
}  

byte uDongle::switch2()
{
  byte sw = uDongle::Read_DS2408_PIO(2);
  return sw;
}   

void uDongle::dongleLed (byte _led,bool on)
{
  uDongle::Write_DS2408_PIO(_led,on);
}

void uDongle::simpleBlink (int _times, int _led)
{
  if (_times <= 0)
  {
    uDongle::Write_DS2408_PIO (_led, 1);
    
  }
  else
  {
    for (int i = 1 ; i <= _times; i++) //_times blink
    {
      delay (200);
      uDongle::Write_DS2408_PIO (_led, 1);
      delay (200);
      uDongle::Write_DS2408_PIO (_led, 0);
    }
  }
}

void uDongle::bothBlink (int _times)
{
  if (_times <= 0)
  {
    uDongle::Write_DS2408_PIO (RED, 1);
    uDongle::Write_DS2408_PIO (GREEN, 1);
    
  }
  else
  {
    for (int i = 1 ; i <= _times; i++) //_times blink
    {
      delay (200);
      uDongle::Write_DS2408_PIO (RED, 1);
      uDongle::Write_DS2408_PIO (GREEN, 1);
      delay (200);
      uDongle::Write_DS2408_PIO (RED, 0);
      uDongle::Write_DS2408_PIO (GREEN, 0);
    }
  }
}

void uDongle::doubleBlink (int _times)
{
  if (_times <= 0)
  {
    uDongle::Write_DS2408_PIO (6, 1);
    uDongle::Write_DS2408_PIO (7, 1);
    
  }
  else
  {
    for (int i = 1 ; i <= _times; i++) //_times blink
    {
      delay (50);
      uDongle::Write_DS2408_PIO (6, 1);
      uDongle::Write_DS2408_PIO (7, 0);
      delay (50);
      uDongle::Write_DS2408_PIO (6, 0);
      uDongle::Write_DS2408_PIO (7, 1);
    }
  }
}
