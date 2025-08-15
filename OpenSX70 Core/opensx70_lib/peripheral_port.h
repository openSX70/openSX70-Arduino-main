#ifndef PERIPHERAL_PORT_H
#define PERIPHERAL_PORT_H

#include <Arduino.h>


#define PERIPHERAL_PING_CMD 0x01
#define PERIPHERAL_ACK 0x02
#define CAMERA_ISO_600 0x03
#define CAMERA_ISO_SX70 0x04

#define ALL_LEDS_OFF 0x05

#define GREEN_ON 0x06
#define GREEN_OFF 0x07

#define RED_ON 0x08
#define RED_OFF 0x09

#define BLUE_ON 0x0A
#define BLUE_OFF 0x0B

//Keeping things similar to the DS2408 for funsies.
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
    peripheral_type type;
} peripheral_device;

void initializePeripheralDevice(peripheral_device *device);
void setPeripheralDevice(peripheral_device *device, uint8_t selector, bool switch1, bool switch2, peripheral_type type);
void updatePeripheralStatus(peripheral_device *device);
void sendCommand(uint8_t command);
bool getDongleSettings(peripheral_device *device);

#endif
