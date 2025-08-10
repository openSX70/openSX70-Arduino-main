#ifndef PERIPHERAL_PORT_H
#define PERIPHERAL_PORT_H

#include <Arduino.h>

//Keeping things similar to the DS2408 for funsies.
#define PERIPHERAL_PING_CMD 0x01
#define PERIPHERAL_ACK 0x02
#define PERIPHERAL_READ_CMD 0xF5

extern HardwareSerial DEBUG_OUTPUT;

enum peripheral_type {
    PERIPHERAL_NONE = 0,
    PERIPHERAL_DONGLE,
    PERIPHERAL_FLASHBAR,
    PERIPHERAL_UNKNOWN
};

enum uart_mode {
    TX,
    RX
};

typedef struct peripheral_device {
    uint8_t selector;
    bool switch1;
    bool switch2;
    uint8_t retryCount;
    peripheral_type type;
    uart_mode transmit_mode;
} peripheral_device;

void initializePeripheralDevice(peripheral_device *device);
void setPeripheralDevice(peripheral_device *device, uint8_t selector, bool switch1, bool switch2, uint8_t retryCount, peripheral_type type, uart_mode transmit_mode);
void checkPeripheral(peripheral_device *device);
void updatePeripheralStatus(peripheral_device *device);
void sendCommand(uint8_t command, peripheral_device *device);

#endif
