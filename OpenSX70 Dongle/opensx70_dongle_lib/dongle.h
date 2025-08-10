#ifndef DONGLE_H
#define DONGLE_H

#include "dongle_pcb.h"

#define PERIPHERAL_PING_CMD 0x01
#define PERIPHERAL_ACK 0x02
#define PERIPHERAL_READ_CMD 0xF5

byte getDongleState(void);

#endif