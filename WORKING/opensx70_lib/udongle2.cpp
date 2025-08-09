#include "DS2408.h"
#include "udongle2.h"
#include "settings.h" // included because of for #if ORIGAMIV1

byte previous_count = 0; //for debouncing

uDongle::uDongle (uint8_t pin){
  _ds = new DS2408(pin);
  Device  _dongleDevice;
  _Pin = pin;
  status peripheral_status;
}

void uDongle::initDS2408(){
  _device_count = _ds->findsingle(&_dongleDevice);
  _ds->reset();
  _ds->write(0x96);
  for (int i = 0; i < 8; i++){
    _ds->write(_dongleDevice[i]);
  }
  _ds->write(0x3C);
  _ds->reset();

}

status uDongle::get_peripheral_status(){
  uint8_t readDevice;
  uint8_t selector_mask = 0b00001111, switch1_mask = 0b00010000, switch2_mask = 0b00100000;
  uint8_t prev_readDevice;

  //While loop acts as a debounce for peripheral state
  //Need two confirmations of the same state in a row in order to get a returned change.
  while(true){
    _device_count = checkDongle();

    pinMode(_Pin, INPUT_PULLUP);
    if (digitalRead(_Pin) == LOW){
      //CASE: FLASH
      peripheral_status.selector = 100;
      peripheral_status.switch1 = false;
      peripheral_status.switch2 = false;
    }
    else if((_device_count == 0) && (digitalRead(_Pin) == HIGH)){  
      //CASE: NO PERIPHERAL
      peripheral_status.selector = 200;
      peripheral_status.switch1 = false;
      peripheral_status.switch2 = false;
    }
    else{
      //Case command module/dongle
      readDevice = _ds->get_state(_dongleDevice);
      peripheral_status.selector = readDevice & selector_mask;
      peripheral_status.switch1 = readDevice & switch1_mask;
      peripheral_status.switch2 = readDevice & switch2_mask;
    }

    if(prev_readDevice != readDevice){
      prev_readDevice = readDevice;
    }
    else{
      return peripheral_status;
    }
  }
}


void uDongle::Write_DS2408_PIO(byte port, bool ON) {
  // This is to turn on LED P6 or LED P7
  // port 6 = P6 = LED1 
  // port 7 = P7 = LED2 (was flash)
  byte OutPIO = B00000000;

  if (ON == true ){
    uint8_t readDevice = _ds->get_state(_dongleDevice);
    bitSet(OutPIO, port);
    _ds->set_state(_dongleDevice, ~OutPIO);
  } 
  else{
    uint8_t readDevice = _ds->get_state(_dongleDevice);
    bitClear(OutPIO, port);
    _ds->set_state(_dongleDevice, ~OutPIO);
  }
} //END OF Write_DS2408_PIO

byte uDongle::checkDongle(){
  _device_count = _ds->findsingle(&_dongleDevice);
  return _device_count;
}
