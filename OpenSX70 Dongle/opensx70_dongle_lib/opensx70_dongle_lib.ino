#include "Arduino.h"
#include "dongle_pcb.h"
#include "dongle.h"

HardwareSerial DEBUGPORT(PIN_RX, PIN_TX);
extern HardwareSerial PERIPHERALPORT;
HardwareTimer *LEDPWM = new HardwareTimer(TIM3);

iso currentISO;

void setup() {
    io_init();
    DEBUGPORT.begin(115200);
    PERIPHERALPORT.begin(115200);
    PERIPHERALPORT.enableHalfDuplexRx();
    LEDPWM->setCaptureCompare(1, 1000);
    
}

//Cases are a bit repetitive but I wanted to ensure LED states were easy to program as it is possible 
//to have multiple LEDs on at the same time. A toggle would make tracking these states kinda maddening.
void loop() {
    byte state = getDongleState();
    if(PERIPHERALPORT.available() > 0) {
        uint8_t command = PERIPHERALPORT.read();
        switch (command) {
            case PERIPHERAL_PING_CMD:
                sendResponse(PERIPHERAL_ACK);
                break;
            case PERIPHERAL_READ_CMD:
                sendResponse(state);
                break;
            case GREEN_ON:
                digitalWrite(LED_GREEN, HIGH);
                break;
            case GREEN_OFF:
                digitalWrite(LED_GREEN, LOW);
                break;
            case RED_ON:
                digitalWrite(LED_RED, HIGH);
                break;
            case RED_OFF:
                digitalWrite(LED_RED, LOW);
                break;
            case BLUE_ON:
                digitalWrite(LED_BLUE, HIGH);
                break;
            case BLUE_OFF:
                digitalWrite(LED_BLUE, LOW);
                break;
            case ALL_LEDS_OFF:
                turnOffLEDs();
                break;
            case PERIPHERAL_SELF_TIMER_CMD:
                BlinkTimerDelay(10);
                break;
            default:
                break;
        }
    }
}
