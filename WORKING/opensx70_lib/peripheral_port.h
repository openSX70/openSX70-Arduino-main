#ifndef PERIPHERAL_PORT_H
#define PERIPHERAL_PORT_H

#include <Arduino.h>

//Keeping things similar to the DS2408 for funsies.
#define PERIPHERAL_PING_CMD 0x01
#define PERIPHERAL_PONG_CMD 0x02
#define PERIPHERAL_READ_CMD 0xF5

extern HardwareSerial DEBUG_OUTPUT;

enum peripheral_type {
    PERIPHERAL_NONE = 0,
    PERIPHERAL_DONGLE,
    PERIPHERAL_FLASHBAR,
    PERIPHERAL_UNKNOWN
};

typedef struct peripheral_device {
    uint8_t selector;
    bool switch1;
    bool switch2;
    peripheral_type type;
} peripheral_device;

void initializePeripheralDevice(peripheral_device *device);
void updatePeripheralStatus(peripheral_device *device);

#endif
