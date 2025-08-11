#include "Arduino.h"
#include "dongle_pcb.h"
#include "dongle.h"

HardwareSerial DEBUGPORT(PIN_RX, PIN_TX);
extern HardwareSerial PERIPHERALPORT;

void setup() {
    io_init();
    DEBUGPORT.begin(115200);
    PERIPHERALPORT.begin(115200);
    PERIPHERALPORT.enableHalfDuplexRx();
}

void loop() {
    byte state = getDongleState();
    if(PERIPHERALPORT.available() > 0) {
        uint8_t command = PERIPHERALPORT.read();
        //delayMicroseconds(500);
        switch (command) {
            case PERIPHERAL_PING_CMD:
                sendResponse(PERIPHERAL_ACK);
                break;
            case PERIPHERAL_READ_CMD:
                sendResponse(state);
                break;
            default:
                break;
        }
    }

}
