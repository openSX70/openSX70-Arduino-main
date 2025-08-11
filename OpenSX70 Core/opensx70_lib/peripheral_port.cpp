#include "Arduino.h"
#include "peripheral_port.h"
#include "sx70_pcb.h"
#include "settings.h"

extern HardwareSerial DEBUG_OUTPUT;

HardwareSerial PERIPHERAL_PORT(PIN_S2);
static uint8_t selector_mask = 0b00001111, switch1_mask = 0b00010000, switch2_mask = 0b00100000;

typedef enum peripheral_state{
    STATE_NODONGLE,
    STATE_DONGLE,
    STATE_FLASHBAR,
    STATE_N
} peripheral_state;

typedef peripheral_state (*peripheral_state_funct)(peripheral_device *device);

peripheral_state do_state_noDongle(peripheral_device *device);
peripheral_state do_state_dongle(peripheral_device *device);
peripheral_state do_state_flashBar(peripheral_device *device);

static const peripheral_state_funct PERIPHERAL_MACHINE[STATE_N] = {
    &do_state_noDongle,
    &do_state_dongle,
    &do_state_flashBar
};

peripheral_state port_state = STATE_NODONGLE;

peripheral_state do_state_noDongle(peripheral_device *device){
    if(digitalRead(PIN_S2) == LOW){
        setPeripheralDevice(device, 100, false, false, PERIPHERAL_FLASHBAR);
        return STATE_FLASHBAR;
    }
    
    PERIPHERAL_PORT.write(PERIPHERAL_PING_CMD);
    PERIPHERAL_PORT.flush();
    PERIPHERAL_PORT.enableHalfDuplexRx();
    
    unsigned long start_time = millis();
    while(millis() - start_time < PERIPHERAL_TIMEOUT_MS){
        if(PERIPHERAL_PORT.available() > 0){
            uint8_t response = PERIPHERAL_PORT.read();
            if(response == PERIPHERAL_ACK){

                setPeripheralDevice(device, 255, false, false, PERIPHERAL_DONGLE);
                return STATE_DONGLE;
            }
        }
    }

    setPeripheralDevice(device, 200, false, false, PERIPHERAL_NONE);
    return STATE_NODONGLE;
}

peripheral_state do_state_dongle(peripheral_device *device){
    PERIPHERAL_PORT.write(PERIPHERAL_READ_CMD);
    PERIPHERAL_PORT.flush();
    PERIPHERAL_PORT.enableHalfDuplexRx();
    
    unsigned long start_time = millis();
    while(millis() - start_time < PERIPHERAL_TIMEOUT_MS){
        if(PERIPHERAL_PORT.available()){
            uint8_t response = PERIPHERAL_PORT.read();
            setPeripheralDevice(device, (response & selector_mask), (response & switch1_mask), (response & switch2_mask), PERIPHERAL_DONGLE);
            return STATE_DONGLE;
        }
    }

    setPeripheralDevice(device, 200, false, false, PERIPHERAL_NONE);
    return STATE_NODONGLE;
}

peripheral_state do_state_flashBar(peripheral_device *device){
    if(digitalRead(PIN_S2) == HIGH){  
        setPeripheralDevice(device, 200, false, false, PERIPHERAL_NONE);
        return STATE_NODONGLE;
    }
    return STATE_FLASHBAR;
}

void initializePeripheralDevice(peripheral_device *device) {
    setPeripheralDevice(device, 200, false, false, PERIPHERAL_NONE);
    PERIPHERAL_PORT.begin(115200);
}

void setPeripheralDevice(peripheral_device *device, uint8_t selector, bool switch1, bool switch2, peripheral_type type) {
    device->selector = selector;
    device->switch1 = switch1;
    device->switch2 = switch2;
    device->type = type;
}

void sendCommand(uint8_t command, peripheral_device *device){
    PERIPHERAL_PORT.write(command);
    PERIPHERAL_PORT.flush();
    PERIPHERAL_PORT.enableHalfDuplexRx();
}

void waitForPeripheralResponse(peripheral_device *device) {
    unsigned long start_time = millis();
    while (millis() - start_time < PERIPHERAL_TIMEOUT_MS) {
        if (PERIPHERAL_PORT.available() > 0) {
            uint8_t response = PERIPHERAL_PORT.read();
            if (response == PERIPHERAL_ACK) {
                return;
            }
        }
    }
}

void updatePeripheralStatus(peripheral_device *device){
    port_state = PERIPHERAL_MACHINE[port_state](device);
}

