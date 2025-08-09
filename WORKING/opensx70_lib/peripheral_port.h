#ifndef PERIPHERAL_PORT_H
#define PERIPHERAL_PORT_H

#include <Arduino.h>

//Keeping things similar to the DS2408 for funsies.
#define PERIPHERAL_PING_CMD 0x01
#define PERIPHERAL_READ_CMD 0xF5

typedef struct peripheral_status {
  uint8_t selector;
  bool switch1;
  bool switch2;
} peripheral_status;



#endif
