#ifndef DONGLE_H
#define DONGLE_H

#include "dongle_pcb.h"

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

typedef enum iso{
    ISO_600 = 0,
    ISO_SX70 = 1
} iso;

byte getDongleState(void);
void sendResponse(uint8_t message);
void setISOLED(iso isoSetting);
void turnOffLEDs();

#endif