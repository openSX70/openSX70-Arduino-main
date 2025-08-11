#include "Arduino.h"
#include "dongle_pcb.h"


void io_init() {
    // Output definition
    //pinMode(PIN_S2, INPUT); // GND
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    // input definition
    pinMode(ENC_1, INPUT_PULLUP);
    pinMode(ENC_2, INPUT_PULLUP);
    pinMode(ENC_4, INPUT_PULLUP);
    pinMode(ENC_8, INPUT_PULLUP);
    pinMode(SWITCH_1, INPUT_PULLUP);
    pinMode(SWITCH_2, INPUT_PULLUP);
}