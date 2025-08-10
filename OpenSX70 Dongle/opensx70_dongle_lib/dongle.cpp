#include "dongle_pcb.h"
#include "dongle.h"
#include "Arduino.h"

HardwareSerial PERIPHERALPORT(PIN_S2);

struct PinConfig {
    uint8_t pin;
    uint8_t mask;
    uint8_t activeHigh;  // 1 if HIGH is active, 0 if LOW is active
};

static const struct PinConfig donglePins[] = {
    {ENC_1, 0x01, 0},
    {ENC_2, 0x02, 0},
    {ENC_4, 0x04, 0},
    {ENC_8, 0x08, 0},
    {SWITCH_1, 0x10, 1},
    {SWITCH_2, 0x20, 1}
};

static const uint8_t NUM_DONGLE_PINS = sizeof(donglePins) / sizeof(donglePins[0]);

byte getDongleState(void) {
    uint8_t dongleState = 0;
    
    for (uint8_t i = 0; i < NUM_DONGLE_PINS; i++) {
        uint8_t pinState = digitalRead(donglePins[i].pin);

        if ((pinState == HIGH && donglePins[i].activeHigh) || (pinState == LOW && !donglePins[i].activeHigh)) {
            dongleState |= donglePins[i].mask;
        }
    }
    
    return dongleState;
}

void sendResponse(uint8_t message) {
    PERIPHERALPORT.write(message);
    PERIPHERALPORT.flush();
    PERIPHERALPORT.enableHalfDuplexRx();
}