#include "dongle_pcb.h"
#include "dongle.h"
#include "Arduino.h"

HardwareSerial PERIPHERALPORT(PIN_S2);
//HardwareTimer *LEDPWM = new HardwareTimer(TIM3);

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

/*
void LED_init() {
    LEDPWM->setPWM(3, LED_BLUE, 1000, 0); // 1kHz, 0% dutycycle
    LEDPWM->setPWM(2, LED_GREEN, 1000, 0); // 1kHz, 0% dutycycle
    LEDPWM->setPWM(1, LED_RED, 1000, 0); // 1kHz, 0% dutycycle
}

void set_LED(uint8_t led, uint8_t value) {
    switch (led) {
        case LED_BLUE:
            LEDPWM->setPWM(3, LED_BLUE, 70, value);
            break;
        case LED_GREEN:
            LEDPWM->setPWM(2, LED_GREEN, 70, value);
            break;
        case LED_RED:
            LEDPWM->setPWM(1, LED_RED, 70, value);
            break;
        default:
            // Invalid LED
            break;
    }
}
*/