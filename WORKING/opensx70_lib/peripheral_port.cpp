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
    peripheral_state result = STATE_NODONGLE;
    if(digitalRead(PIN_S2) == LOW){
        //Flashbar detected
        device->type = PERIPHERAL_FLASHBAR;
        device->selector = 100;
        device->switch1 = false; 
        device->switch2 = false;
        result = STATE_FLASHBAR;
        DEBUG_OUTPUT.println(F("Transition to STATE_FLASHBAR from STATE_NODONGLE"));
    }
    return result;
}

peripheral_state do_state_dongle(peripheral_device *device){
    peripheral_state result = STATE_DONGLE;
    return result;
}

peripheral_state do_state_flashBar(peripheral_device *device){
    peripheral_state result = STATE_FLASHBAR;
    if(digitalRead(PIN_S2) == HIGH){
        
        device->type = PERIPHERAL_NONE;
        device->selector = 200;
        device->switch1 = false;
        device->switch2 = false;
        DEBUG_OUTPUT.println(F("Transition to STATE_NODONGLE from STATE_FLASHBAR"));
        result = STATE_NODONGLE;
    }
    return result;
}

void initializePeripheralDevice(peripheral_device *device) {
    device->selector = 200;
    device->switch1 = false;
    device->switch2 = false;
    device->type = PERIPHERAL_NONE;

    PERIPHERAL_PORT.begin(115200);
    //PERIPHERAL_PORT.setTimeout(1000);
    //PERIPHERAL_PORT.enableHalfDuplexRx();
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

