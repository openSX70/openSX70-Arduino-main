#include "dongle_pcb.h"
#include "dongle.h"
#include "Arduino.h"

HardwareSerial PERIPHERALPORT(PIN_S2);

struct PinConfig {
    uint8_t pin;
    uint8_t mask;
    uint8_t activeHigh;  // 1 if HIGH is active, 0 if LOW is active
};

/*
    {PIN, MASK, ACTIVE_HIGH}
*/
static const struct PinConfig donglePins[] = {
    {ENC_1, 0x01, 0},
    {ENC_2, 0x02, 0},
    {ENC_4, 0x04, 0},
    {ENC_8, 0x08, 0},
    {SWITCH_1, 0x10, 1},
    {SWITCH_2, 0x20, 1}
};

// Just in case we want to add more later, technically we can have up to 8 pins with the current communication protocol
// Maybe two can be virtual for focus and shutter release. Bluetooth dongle with a remote?
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

void setISOLED(iso isoSetting) {
    switch(isoSetting) {
        case ISO_600:
            digitalWrite(LED_BLUE, HIGH);
            break;
        case ISO_SX70:
            digitalWrite(LED_RED, HIGH);
            break;
        default:
            // Turn off all LEDs
            break;
    }
}

void turnOffLEDs() {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
}

void BlinkTimerDelay(byte time){
    // DONGLE-LED BLINKS ON COUNTDOWN (10secs)
    // this is a test function to do the progressing blinking of the LED using my blink function
    // it last exactly 10 seconds (2500x4) and I could not accomplish this with the delay()
    // everytime the led (in pin 5) blinks faster 1000, 700, 400, and 100.
    //uint32_t startTimer = millis();
    //*******************************************************
    uint32_t steps = (time * 1000) / 4;

    Blink(1000, steps, LED_GREEN);
    Blink(600, steps, LED_GREEN);
    Blink(200, steps, LED_GREEN);
    steps = steps / 2;
    Blink(80, steps, LED_GREEN);
    Blink(80, steps, LED_RED);
}

void Blink(unsigned int interval, int timer, int Pin){
    int ledState = LOW;             // ledState used to set the LED
    pinMode(Pin, OUTPUT);
    uint32_t previousMillis = 0;        // will store last time LED was updated
    uint32_t currentMillisTimer = millis();
    while (millis() < (currentMillisTimer + timer)){
        uint32_t currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            // save the last time you blinked the LED
            previousMillis = currentMillis;
            // if the LED is off turn it on and vice-versa:
            if (ledState == 0){
                ledState = 1;
                digitalWrite(Pin, HIGH);
            } 
            else{
                ledState = 0;
                digitalWrite(Pin, LOW);
            }
        }
    }
}