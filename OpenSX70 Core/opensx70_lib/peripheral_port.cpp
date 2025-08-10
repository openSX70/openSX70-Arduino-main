#include "Arduino.h"
#include "peripheral_port.h"
#include "sx70_pcb.h"

HardwareSerial PERIPHERAL_PORT(PIN_S2);

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
        setPeripheralDevice(device, 100, false, false, 0, PERIPHERAL_FLASHBAR, TX);
        return STATE_FLASHBAR;
    }
    else if(device->transmit_mode == TX){
        PERIPHERAL_PORT.write(PERIPHERAL_PING_CMD);
        PERIPHERAL_PORT.flush();
        PERIPHERAL_PORT.enableHalfDuplexRx();
        return STATE_DONGLE;
    }
    else{
        return STATE_NODONGLE;
    }
}

peripheral_state do_state_dongle(peripheral_device *device){
    peripheral_state result = STATE_DONGLE;
    return result;
}

peripheral_state do_state_flashBar(peripheral_device *device){
    if(digitalRead(PIN_S2) == HIGH){  
        setPeripheralDevice(device, 200, false, false, 0, PERIPHERAL_NONE, TX);
        return STATE_NODONGLE;
    }
    else{
        return STATE_FLASHBAR;
    }
}

void initializePeripheralDevice(peripheral_device *device) {
    setPeripheralDevice(device, 200, false, false, 0, PERIPHERAL_NONE, TX);
    PERIPHERAL_PORT.begin(115200);
}

void setPeripheralDevice(peripheral_device *device, uint8_t selector, bool switch1, bool switch2, uint8_t retryCount, peripheral_type type, uart_mode transmit_mode) {
    device->selector = selector;
    device->switch1 = switch1;
    device->switch2 = switch2;
    device->retryCount = retryCount;
    device->type = type;
    device->transmit_mode = transmit_mode;
}

void findPeripheral(peripheral_device *device){

}

/*
void findPeripheral(peripheral_device *device){
    if(digitalRead(PIN_S2) == LOW){
        //Flashbar detected
        device->selector = 100;
        device->switch1 = false; 
        device->switch2 = false;
        device->type = PERIPHERAL_FLASHBAR;
        return;
    }
    else if(PERIPHERAL_PORT.available()){
        //Dongle detected
        uint8_t cmd = PERIPHERAL_PORT.read();
        if(cmd == PERIPHERAL_PONG_CMD){
            device->selector = 0; //Assuming selector 0 for dongle
            device->switch1 = false; 
            device->switch2 = false;
            device->type = PERIPHERAL_DONGLE;
            return;
        }
    }
    PERIPHERAL_PORT.write(PERIPHERAL_PING_CMD);
    PERIPHERAL_PORT.flush();
    PERIPHERAL_PORT.enableHalfDuplexRx();
}
*/

void updatePeripheralStatus(peripheral_device *device){
    port_state = PERIPHERAL_MACHINE[port_state](device);
}

