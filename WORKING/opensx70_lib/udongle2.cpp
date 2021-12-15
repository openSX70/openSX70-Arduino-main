#include "DS2408.h"
#include "udongle2.h"
#include "settings.h" // included because of for #if ORIGAMIV1

byte previous_count = 0; //for debouncing

uDongle::uDongle (uint8_t pin) 
{
  _ds = new DS2408(pin);
  Device  _dongleDevice;
  _Pin = pin;
  status peripheral_status;
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

byte uDongle::get_peripheral_status(){
  uint8_t readDevice;
  uint8_t selector_mask = 0b00001111, switch1_mask = 0b00010000, switch2_mask = 0b00100000;

  pinMode(_Pin, INPUT_PULLUP);
  if (digitalRead(_Pin) == LOW){
    //CASE: FLASH
    peripheral_status.selector = 100;
    return this->peripheral_status; // FLASH
  }
  if  ((_device_count == 0) && (digitalRead(_Pin) == HIGH)){  
    //CASE: NO PERIPHERAL
    peripheral_status.selector = 200;
    return this->peripheral_status;
  }

  readDevice = _ds->get_state(_dongleDevice);
  #if UDONGLE
    peripheral_status.selector = readDevice & selector_mask;
  #elif ORIGAMIV1
    //TODO!
  #endif

  peripheral_status.switch1 = readDevice & switch1_mask;
  peripheral_status.switch2 = readDevice & switch2_mask;

  return this->peripheral_status;
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
