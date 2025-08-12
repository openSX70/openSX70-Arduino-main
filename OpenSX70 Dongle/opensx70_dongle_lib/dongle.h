#ifndef DONGLE_H
#define DONGLE_H

#include "dongle_pcb.h"

#define PERIPHERAL_PING_CMD 0x01
#define PERIPHERAL_ACK 0x02
#define CAMERA_ISO_600 0x03
#define CAMERA_ISO_SX70 0x04

#define DONGLE_LED_OFF 0x05

#define PERIPHERAL_READ_CMD 0xF5
#define PERIPHERAL_SELF_TIMER_CMD 0xF6

typedef enum iso{
    ISO_600 = 0,
    ISO_SX70 = 1
} iso;

byte getDongleState(void);
void sendResponse(uint8_t message);
void setISOLED(iso isoSetting);
void turnOffLEDs();

#endif