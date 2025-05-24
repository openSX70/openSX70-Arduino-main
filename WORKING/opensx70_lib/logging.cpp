#include "Arduino.h"
#include "sx70_pcb.h"
#include "logging.h"

#ifdef ARDUINO_GENERIC_G030K8TX
#if DEBUG
HardwareSerial STMUART(USART_RX, USART_TX);
#endif
#endif

void serial_init(){
    #if DEBUG
    #ifdef ARDUINO_AVR_PRO
        Serial.begin(9600);
    #elif defined ARDUINO_GENERIC_G030K8TX
        STMUART.begin(19200);
    #endif
    #endif
}

void output_line_serial(String const& input){
    #if DEBUG
    #ifdef ARDUINO_AVR_PRO
        Serial.println(input);
    #elif defined ARDUINO_GENERIC_G030K8TX
        STMUART.println(input);
    #endif
    #endif
}

void output_serial(String const& input){
    #if DEBUG
    #ifdef ARDUINO_AVR_PRO
        Serial.print(input);
    #elif defined ARDUINO_GENERIC_G030K8TX
        STMUART.print(input);
    #endif
    #endif
}