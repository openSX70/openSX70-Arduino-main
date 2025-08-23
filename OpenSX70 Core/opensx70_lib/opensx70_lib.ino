#include "Arduino.h"
#include "open_sx70.h"


ClickButton sw_S1(PIN_S1, HIGH);

peripheral_device current_status;
peripheral_device previous_status;

Camera openSX70;

HardwareSerial DEBUG_OUTPUT(USART_RX, USART_TX);

int savedISO;

bool isoBlinked = false;
bool mEXPFirstRun;
bool multipleExposureMode;

/*------------BEGIN STATE MACHINE SET_UP------------*/
typedef enum{
    STATE_DARKSLIDE,
    STATE_NODONGLE,
    STATE_DONGLE,
    STATE_FLASHBAR,
    STATE_MULTI_EXP,
    STATE_N
} camera_state;

typedef camera_state (*camera_state_funct)(void);

camera_state do_state_darkslide (void);
camera_state do_state_noDongle (void);
camera_state do_state_dongle (void);
camera_state do_state_flashBar (void);
camera_state do_state_multi_exp (void);

static const camera_state_funct STATE_MACHINE [STATE_N] = {
    &do_state_darkslide,
    &do_state_noDongle,
    &do_state_dongle,
    &do_state_flashBar,
    &do_state_multi_exp
};

//Default state
camera_state state = STATE_DARKSLIDE;

void setup() {//setup - Inizialize
    DEBUG_OUTPUT.begin(115200);
    DEBUG_OUTPUT.println(F("Welcome to openSX70 Version: 08_04_2025 STM32 edition"));

    io_init();
    validateISO();
    meter_init();
    meter_set_iso(savedISO);

    //peripheral.initDS2408();
    init_EEPROM(); //#writes Default ISO to EEPROM

    // (These are default if not set, but changeable for convenience)
    sw_S1.debounceTime   = 15;   // Debounce timer in ms 15
    sw_S1.multiclickTime = 250;  // Time limit for multi clicks
    sw_S1.longClickTime  = 0; // time until "held-down clicks" register

    //current_status = peripheral.get_peripheral_status();
    initializePeripheralDevice(&current_status);

    mEXPFirstRun = false;
    multipleExposureMode = false;

    
    // Move some setup stuff into its own init state
    // Reason for this is to add branching option for making the dark slide eject a standard exposure
    // (Changing bag pack swap)
    if (digitalRead(PIN_S5) != LOW)
    {
        openSX70.shutterCLOSE();
        openSX70.mirrorDOWN();
        openSX70.shutterOPEN();
        #if SIMPLEDEBUG
        DEBUG_OUTPUT.println(F("Initialize: mirrorDOWN"));
        #endif
    }
    S1ISOSwap();
}

/*LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP*/
void loop() {
    previous_status = current_status;
    updatePeripheralStatus(&current_status);
    sonarFocus();
    sw_S1.Update();
    state = STATE_MACHINE[state]();
}

camera_state do_state_darkslide (void) {
    #if SHUTTERDARKSLIDE
    sw_S1.Update();
    if (((sw_S1.clicks == -1) || (sw_S1.clicks == 1)) || (digitalRead(PIN_S8) == LOW)){
    #endif
        if (digitalRead(PIN_S8) == HIGH && digitalRead(PIN_S9) == LOW){
            openSX70.darkslideEJECT();         
        }
   
    #if SHUTTERDARKSLIDE
    sw_S1.Reset();
    }
    #endif
  if(!isoBlinked){
      ISOBlink();
  }
  return returnState();    
}

camera_state do_state_noDongle (void){
    LightMeterHelper(1);
    if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1)){
        LightMeterHelper(0); 
        beginExposure();
        openSX70.AutoExposure(savedISO);
        sw_S1.Reset();
    }

    return returnState();
}

camera_state do_state_dongle (void){
    if(ShutterSpeed[current_status.selector] == A100 || ShutterSpeed[current_status.selector] == A600){
        LightMeterHelper(1); //LMHelper Auto Mode
    }
    else{
        LightMeterHelper(0);
    }

    if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0)){
        #if SIMPLEDEBUG
            DEBUG_OUTPUT.print("SELECTOR: ");
            DEBUG_OUTPUT.println(current_status.selector);
        #endif
        LightMeterHelper(0);

        if(getSwitchStates(SELF_TIMER)){
            switch2Function(0); //switch2Function Manual Mode
        }
        beginExposure();
        dongleFunctions();
        sw_S1.Reset();
    } 

    return returnState();
}

camera_state do_state_flashBar (void){
    LightMeterHelper(0);

    if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1)){
        beginExposure();
        openSX70.AutoExposureFF(savedISO);
        sw_S1.Reset();
    }

    return returnState();
}

camera_state do_state_multi_exp (void){
    bool mexpSwitchStatus = getSwitchStates(MEXP_MODE);

    if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0)){
    if(mexpSwitchStatus){
        if(getSwitchStates(SELF_TIMER)){
            switch2Function(0); // start self timer 
        }
        if(mEXPFirstRun){
            mEXPFirstRun = false;
            beginExposure();
        }
        dongleFunctions();
    }
    else if((mexpSwitchStatus == 0) && (mEXPFirstRun == false)){
        openSX70.multipleExposureLastClick();
        return STATE_DONGLE;
    }
    sw_S1.Reset();
    }

    if((mexpSwitchStatus == false) && (mEXPFirstRun == true)){
        multipleExposureMode = false;
        #if STATEDEBUG
            DEBUG_OUTPUT.println(F("TRANSITION TO STATE_DONGLE FROM STATE_MULTI_EXP"));
        #endif
        return STATE_DONGLE;
    }
    return STATE_MULTI_EXP;
}

camera_state returnState(){
    switch(current_status.type){
        case PERIPHERAL_NONE:
            #if STATEDEBUG
                DEBUG_OUTPUT.println(F("TRANSITION TO STATE_NODONGLE"));
            #endif
            return STATE_NODONGLE;
        case PERIPHERAL_DONGLE:
            #if STATEDEBUG
                DEBUG_OUTPUT.println(F("TRANSITION TO STATE_DONGLE"));
            #endif
            return STATE_DONGLE;
        case PERIPHERAL_FLASHBAR:
            #if STATEDEBUG
                DEBUG_OUTPUT.println(F("TRANSITION TO STATE_FLASHBAR"));
            #endif
            return STATE_FLASHBAR;
        default:
            #if STATEDEBUG
                DEBUG_OUTPUT.println(F("TRANSITION TO STATE_UNKNOWN"));
            #endif
            return STATE_NODONGLE;
    }
}

void dongleFunctions(){
    static uint16_t isoSelection;
    bool dongleAutoFlash = getSwitchStates(DONGLE_AUTO_FLASH);

    turnLedsOff();
    if(current_status.selector<12){ //MANUAL SPEEDS  
        openSX70.ManualExposure(current_status.selector);;
    }
    else if(ShutterSpeed[current_status.selector] == POST){ //POST
        openSX70.ShutterT();
    }
    else if(ShutterSpeed[current_status.selector] == POSB){ //POSB
        openSX70.ShutterB();
    }
    else{ //Auto catch-all. Passes the value stored in the ShutterSpeed list at the selector value
        switch(ShutterSpeed[current_status.selector]){
            case AUTO600:
                isoSelection = ISO_600;
                break;
            case AUTO100:
                isoSelection = ISO_SX70;
                break;
        } 
        if(!dongleAutoFlash){
            openSX70.AutoExposure(isoSelection);
        }
        else{
            openSX70.AutoExposureFF(isoSelection);
        }
    }
}

void sonarFocus(){
    if ((digitalRead(PIN_S1F) == HIGH)){
        digitalWrite(PIN_S1F_FBW, HIGH);
    }
    else{
        digitalWrite(PIN_S1F_FBW, LOW);
    }
}

//Added to remove check for multiple exposure mode from standard manual exposure state. 
//Offload the check to multiple exposure state
void beginExposure(){
    openSX70.shutterCLOSE();
    delay(40);
    openSX70.mirrorUP();
}

void turnLedsOff(){ //TODO :move to camerafunction
    digitalWrite(PIN_LED1, LOW);
    digitalWrite(PIN_LED2, LOW);
}

void switch2Function(int mode) {
    #if SIMPLEDEBUG
        DEBUG_OUTPUT.println(F("Self Timer Activated"));
    #endif
    //0 Dongle 1 No dongle
    if (mode == 0) {
        digitalWrite(PIN_LED2, LOW);
        digitalWrite(PIN_LED1, LOW);

        // TODO: rewrite self timer to use new dongle functionality

        digitalWrite(PIN_S1F_FBW, LOW);
        sendCommand(PERIPHERAL_SELF_TIMER_CMD);
        delay(4000);
        openSX70.shutterCLOSE();
        delay(2000);
        openSX70.mirrorUP();
        delay(1000);
        digitalWrite(PIN_S1F_FBW, HIGH);
        delay(3000);

    }
    #if SIMPLEDEBUG
        DEBUG_OUTPUT.println(F("Self Timer End"));
    #endif
}

void LightMeterHelper(byte ExposureType){
    if(lightmeterHelper){
        meter_led(current_status.selector, ExposureType);
    }
}

void ISOBlink(){
    switch(savedISO){
        case ISO_600:
            for(uint8_t i=0; i<2; i++){
                sendCommand(BLUE_ON);
                digitalWrite(PIN_LED2, HIGH);
                delay(100);
                sendCommand(BLUE_OFF);
                digitalWrite(PIN_LED2, LOW);
                delay(100);
            }
            break;
        case ISO_SX70:
            for(uint8_t i=0; i<2; i++){
                sendCommand(RED_ON);
                digitalWrite(PIN_LED1, HIGH);
                delay(100);
                sendCommand(RED_OFF);
                digitalWrite(PIN_LED1, LOW);
                delay(100);
            }
            break;
    }
}

void saveISO(int iso){
    WriteISO(iso);
    savedISO = iso;
    meter_set_iso(iso);
}

void S1ISOSwap(){
    int _selectedISO;

    if(digitalRead(PIN_S1) == HIGH){
        savedISO = ReadISO();
        if (savedISO == ISO_600) { 
            #if DEBUG
                DEBUG_OUTPUT.println("ISO HAS BEEN SWAPPED TO: SX70");
            #endif
            _selectedISO = ISO_SX70;
        }
        else if ((savedISO == ISO_SX70)) {
            #if DEBUG
                DEBUG_OUTPUT.println("ISO HAS BEEN SWAPPED TO: 600");
            #endif
            _selectedISO = ISO_600;
        }
        else{
            //Boards come without a set ISO, this will set the ISO to 600 without a dongle.
            #if DEBUG
                DEBUG_OUTPUT.println("ISO HAS BEEN SWAPPED TO: 600");
            #endif
            _selectedISO = ISO_600;
        }
        saveISO(_selectedISO);
        ISOBlink();
        isoBlinked = true;
    }
    while(digitalRead(PIN_S1) == HIGH);
    sw_S1.Reset();
}

void validateISO(){
    savedISO = ReadISO();
    if(savedISO != ISO_600 && savedISO != ISO_SX70){
        //ISO in EEPROM is invalid, set to 600
        saveISO(ISO_600);
    }
}

bool getSwitchStates(uint8_t switchValue){
    switch (switchValue){
        case 1:
            return current_status.switch1;
        case 2:
            return current_status.switch2;
        default:
            return false;
    }
}