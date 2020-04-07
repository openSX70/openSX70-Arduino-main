#include <WProgram.h>
#include <DS2408.h>
#include <OneWire.h>
#include <string.h>

#include <stdio.h>


#define DEVICE_BUS_PORT 9

DS2408  ds(DEVICE_BUS_PORT);

Device led_device    = {0x29, 0xF5, 0x22, 0x7, 0x00, 0x00, 0x00, 0xBD};
Device reader_device = {0x29, 0xF5, 0x22, 0x7, 0x00, 0x00, 0x00, 0x47};


Devices devices;
uint8_t device_count;

static FILE uartout = {0} ;
static int uart_putchar (char c, FILE *stream) {
    Serial.write(c) ;
    return 0 ;
}
void setup_stdout() {
    fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &uartout ;
}

#ifdef CXA_PURE_VIRTUAL_FIX
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) { while(1); }
#endif

void print_byte(uint8_t data);
void print_address(byte* address);
void print_devices(Devices* devices, uint8_t device_count);
void setup_devices(Devices* devices, uint8_t device_count);
void display_mode(Device device);
void display_activity(Device device);
void display_state(Device device);

void setup(void) {
    Serial.begin(9600);
    setup_stdout();

    Serial.println("Welcome2");

    device_count = ds.find(&devices);
    print_devices(&devices, device_count);
    setup_devices(&devices, device_count);
}


void loop(void) {
    uint8_t state = 0;
    bool left = 1;
    for(int index=0; index<device_count; index++) {
        print_address(devices[index]);
        
        if(devices[index][7] == 0xBD) {
            ds.set_state(devices[index], state);
            for(int index1=0; index1<100; index1++) {
                if(state == 0xFF || state == 0)
                    left = !left;
                if(left)
                    state = (state<<1)+1;
                else
                    state = state>>1;
                ds.set_state(state, true);
                delay(100);
            }
        }
        
        display_mode(devices[index]);
        display_activity(devices[index]);
        display_state(devices[index]);
        Serial.println();
    }
}

void setup_devices(Devices* devices, uint8_t device_count) {
    for(int index=0; index < device_count; index++) {
        ds.set_mode((*devices)[index], RESET_PIN_MODE(STROBE_MODE));
    }
}

void display_mode(Device device) {
    Serial.print(" MODE=");
    print_byte(ds.get_mode(device));
}
void display_activity(Device device) {
    Serial.print(" ACTIVTY=");
    print_byte(ds.get_activity(device));
}
void display_state(Device device) {
    Serial.print(" STATE=");
    print_byte(ds.get_state(device));
}

void print_byte(uint8_t data) {
    for(int index=0; index<8; index++) {
        Serial.print(data & 1, BIN);
        data = data >> 1;
    }
}
void print_address(byte* address) {
    Serial.print("[");
    for(int index = 0; index < sizeof(Device); index++) {
        Serial.print(address[index], HEX);
        if(index < sizeof(Device)-1)
            Serial.print(" ");
    }
    Serial.print("] ");
}

void print_devices(Devices* devices, uint8_t device_count) {
    Serial.print("Found ");
    Serial.print(device_count , HEX);
    Serial.println(" devices.");

    for(int index=0; index < device_count; index++) {
        Serial.print(index+1, HEX);
        Serial.print(". ");
        print_address((*devices)[index]);
        Serial.println();
    }
}
