#include "Arduino.h"
#include "dongle_pcb.h"
#include "dongle.h"

HardwareSerial DEBUGPORT(PIN_RX, PIN_TX);
HardwareSerial PERIPHERALPORT(PIN_S2);

void setup() {
    io_init();
    DEBUGPORT.begin(115200);
}

void loop() {

}
