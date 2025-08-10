#ifndef DONGLE_PCB_H
    #define DONGLE_PCB_H
    #include <Arduino.h>

    const uint32_t PIN_S2 = PA2;
    const uint32_t ENC_1 = PB9;
    const uint32_t ENC_2 = PC15;
    const uint32_t ENC_4 = PA0;
    const uint32_t ENC_8 = PA1;
    const uint32_t SWITCH_1 = PA5;
    const uint32_t SWITCH_2 = PA4;
    const uint32_t LED_BLUE = PB0;
    const uint32_t LED_GREEN = PA7;
    const uint32_t LED_RED = PA6;

    const uint32_t PIN_RX = PB7;
    const uint32_t PIN_TX = PB6;

    void io_init(void);

#endif