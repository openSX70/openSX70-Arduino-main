#include "Arduino.h"
#include "open_sx70.h"
#include "logging.h"

ClickButton sw_S1(PIN_S1, S1Logic);

uDongle peripheral(PIN_S2);
Camera openSX70(&peripheral);

status current_status;
status previous_status;

int savedISO;

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
  #if DEBUG
    serial_init();
    output_line_serial(F("Welcome to openSX70 Version: 05_08_2025 Integrator STM32"));
    output_serial(F("Magic Number: A100="));
    output_serial(String(A100));
    output_serial(F("| A600 ="));
    output_line_serial(String(A600));
  #endif

  io_init();
  meter_init();

  peripheral.initDS2408();
  init_EEPROM(); //#writes Default ISO to EEPROM

  // (These are default if not set, but changeable for convenience)
  sw_S1.debounceTime   = 15;   // Debounce timer in ms 15
  sw_S1.multiclickTime = 250;  // Time limit for multi clicks
  sw_S1.longClickTime  = 0; // time until "held-down clicks" register

  current_status = peripheral.get_peripheral_status();

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
    output_line_serial(F("Initialize: mirrorDOWN"));
    #endif
  }
  S1ISOSwap();
  validateISO();
}

/*LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP*/
void loop() {
  previous_status = current_status;
  current_status = peripheral.get_peripheral_status();
  sonarFocus();
  sw_S1.Update();
  state = STATE_MACHINE[state]();
}

camera_state do_state_darkslide (void) {
  camera_state result = STATE_DARKSLIDE;
  #if SHUTTERDARKSLIDE
  sw_S1.Update();
  if (((sw_S1.clicks == -1) || (sw_S1.clicks == 1)) || (digitalRead(PIN_S8) == LOW)){
  #endif
    if (digitalRead(PIN_S8) == HIGH && digitalRead(PIN_S9) == LOW){
      if(peripheral.checkDongle() == 0){
        openSX70.darkslideEJECT(); 
      }
      else{
        peripheral.dongleLed(GREEN, HIGH); //green uDongle LED on while ejecting Darkslide
        openSX70.darkslideEJECT();
        peripheral.dongleLed(GREEN, LOW); //switching off green uDongle LED
      }
    }
    if ((current_status.selector <= 15) && (peripheral.checkDongle() > 0)){ //((selector <= 15) && (peripheral.checkDongle() > 0))
      result = STATE_DONGLE;
      savedISO = ReadISO();
      delay(100);
      BlinkISO();
      #if STATEDEBUG
        output_line_serial(F("TRANSITION TO STATE_DONGLE FROM STATE_DARKSLIDE"));
      #endif
    }
    else if ((current_status.selector == 100) && (peripheral.checkDongle() == 0)){
      result = STATE_FLASHBAR;
      savedISO = ReadISO();
      #if STATEDEBUG
        output_line_serial(F("TRANSITION TO STATE_FLASHBAR FROM STATE_DARKSLIDE"));
      #endif
    }
    else{
      result = STATE_NODONGLE;
      savedISO = ReadISO();
      #if STATEDEBUG
        output_line_serial(F("TRANSITION TO STATE_NODONGLE FROM STATE_DARKSLIDE"));
      #endif
    }
    
  #if SHUTTERDARKSLIDE
  sw_S1.Reset();
  }
  #endif
  return result;
}

camera_state do_state_noDongle (void){
  camera_state result = STATE_NODONGLE;
  //savedISO = ReadISO();
  LightMeterHelper(1);
  if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1)){
    LightMeterHelper(0); 
    beginExposure();
    openSX70.AutoExposure(savedISO);
    sw_S1.Reset();
  }
  #if DOUBLECLICK
  if (sw_S1.clicks == 2){ //Doubleclick the Red Button with no Dongle inserted
    LightMeterHelper(0); 
    beginExposure();
    switch2Function(1);
    openSX70.AutoExposure(savedISO);
    sw_S1.Reset();
  }
  #endif
  //Checks for dongle or flashbar insertion

  if (current_status.selector<=15){
    #if STATEDEBUG
      output_line_serial(F("TRANSITION TO STATE_DONGLE FROM STATE_NODONGLE"));
    #endif
    result = STATE_DONGLE;
    savedISO = ReadISO();
    if(((current_status.switch1 == 1) && (current_status.switch2 == 1))){
      dongleISOSwap();
    }
    else if(current_status.selector<=13){ //Dont blink on AUTOMODE
      BlinkISO();
    }
  }
  else if (current_status.selector==100){
    result = STATE_FLASHBAR;
    savedISO = ReadISO();
    #if STATEDEBUG
      output_line_serial(F("TRANSITION TO STATE_FLASHBAR FROM STATE_NODONGLE"));
    #endif
  }
  return result;
}

camera_state do_state_dongle (void){
  camera_state result = STATE_DONGLE;
  DongleInserted();
  

  if(ShutterSpeed[current_status.selector] == A100 || ShutterSpeed[current_status.selector] == A600){
    LightMeterHelper(1); //LMHelper Auto Mode
  }
  else{
    LightMeterHelper(0);
  }
  
  if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0)){
    #if SIMPLEDEBUG
      output_serial("SELECTOR: ");
      output_line_serial(current_status.selector);
    #endif
    LightMeterHelper(0);

    if(current_status.switch2 == 1){
      switch2Function(0); //switch2Function Manual Mode
    }
    beginExposure();
    if(current_status.selector<12){ //MANUAL SPEEDS  
      openSX70.ManualExposure(savedISO, current_status.selector);
    }
    else if(ShutterSpeed[current_status.selector] == POST){ //POST
      turnLedsOff();
      openSX70.ShutterT();
    }
    else if(ShutterSpeed[current_status.selector] == POSB){ //POSB
      turnLedsOff();
      openSX70.ShutterB();
    }
    else{ //Auto catch-all. Passes the value stored in the ShutterSpeed list at the selector value
      switch(ShutterSpeed[current_status.selector]){
        case AUTO100:
          openSX70.AutoExposure(ISO_SX70);
          break;
        case AUTO600:
          openSX70.AutoExposure(ISO_600);
          break;
      }
    }
    sw_S1.Reset();
  } 
  
  // Dongle Removed
  if (current_status.selector == 200){
    result = STATE_NODONGLE;
    savedISO = ReadISO();
    #if STATEDEBUG
      output_line_serial(F("TRANSITION TO STATE_NODONGLE FROM STATE_DONGLE"));
    #endif
  } 
  // Multiple Exposure switch flipped
  else if (current_status.switch1){
    result = STATE_MULTI_EXP;
    multipleExposureMode = true;
    mEXPFirstRun = true;
    LightMeterHelper(0);
    #if STATEDEBUG
      output_line_serial(F("TRANSITION TO STATE_MULTI_EXP FROM STATE_DONGLE"));
    #endif
  }

  return result;
}

camera_state do_state_flashBar (void){
  LightMeterHelper(0);
  camera_state result = STATE_FLASHBAR;
  if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1)){
    beginExposure();
    openSX70.AutoExposureFF(savedISO);
    sw_S1.Reset();
  }
  #if DOUBLECLICK
  if (sw_S1.clicks == 2){
    beginExposure();
    switch2Function(1);
    openSX70.AutoExposureFF(savedISO);
    sw_S1.Reset();
  } 
  #endif
  
  if (current_status.selector == 200){
    result = STATE_NODONGLE;
    savedISO = ReadISO();
    #if STATEDEBUG
      output_line_serial(F("TRANSITION TO STATE_NODONGLE FROM STATE_FLASHBAR"));
    #endif
  } 
  return result;
}

camera_state do_state_multi_exp (void){
  static uint32_t multipleExposureLastStartTimestamp;
  camera_state result = STATE_MULTI_EXP;
  DongleInserted();

  if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0)){
    if(current_status.switch1){
      if(current_status.switch2){
        switch2Function(0); // start self timer 
      }
      if(mEXPFirstRun){
        mEXPFirstRun = false;
        multipleExposureLastStartTimestamp = millis();
        beginExposure();
      }
      
      if(current_status.selector<12){ //MANUAL SPEEDS  
        openSX70.ManualExposure(savedISO, current_status.selector);;
      }
      else if(ShutterSpeed[current_status.selector] == POST){ //POST
        turnLedsOff();
        openSX70.ShutterT();
      }
      else if(ShutterSpeed[current_status.selector] == POSB){ //POSB
        turnLedsOff(); //why?
        openSX70.ShutterB();
      }
      else{ //Auto catch-all. Passes the value stored in the ShutterSpeed list at the selector value
        switch(ShutterSpeed[current_status.selector]){
        case AUTO100:
          openSX70.AutoExposure(ISO_SX70);
          break;
        case AUTO600:
          openSX70.AutoExposure(ISO_600);
          break;
        } 
      }
    }
    else if((current_status.switch1 == 0) && (mEXPFirstRun == false)){
      result = STATE_DONGLE;
      openSX70.multipleExposureLastClick();
    }
    sw_S1.Reset();
  }

  if((mEXPFirstRun == false) && ((millis() - multipleExposureLastStartTimestamp) >= MULTIPLE_EXPOSURES_TIMEOUT)){
    mEXPFirstRun = true;
    peripheral.simpleBlink(2, RED);
    openSX70.multipleExposureLastClick(); 
  }

  if(current_status.switch1 == false && mEXPFirstRun == true){
    result = STATE_DONGLE;
    multipleExposureMode = false;
    #if STATEDEBUG
      output_line_serial(F("TRANSITION TO STATE_DONGLE FROM STATE_MULTI_EXP"));
    #endif
  }

  return result;
}

void sonarFocus() {
  if ((digitalRead(PIN_S1F) == HIGH)){
    openSX70.S1F_Focus();
  }
  else{
    openSX70.S1F_Unfocus();
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
   //delay(400); 
}

void DongleInserted() { //Dongle is pressend LOOP
  if (digitalRead(PIN_S1) != S1Logic) { //Dont run DongleInserted Function on S1T pressed
    if ((current_status.selector != previous_status.selector)){
      #if ADVANCEDEBUG
        output_serial(F("DONGLE Mode:  "));
        output_serial(F("Selector: "));
        output_serial(current_status.selector);
        output_serial(F(" Switch1: "));
        output_serial(current_status.switch1);
        output_serial(F(" Switch2: "));
        output_serial(current_status.switch2);
        output_serial(F(" speed: "));
        output_line_serial(ShutterSpeed[current_status.selector]);
      #endif
      // TODO Move this into state transition
      blinkAutomode();
    }
  }
}

void BlinkISO() { //read the default ISO and blink once for SX70 and twice for 600
  if((current_status.switch1 != 1) || (current_status.switch2 != 1)){ //Not Save ISO //Changed to OR 01.06.2020
      #if SIMPLEDEBUG
        output_line_serial(F("Blink for the saved ISO setting on Dongle insertion."));
      #endif
      //blinkAutomode();
      savedISO = ReadISO();
      turnLedsOff();
      if (savedISO == ISO_600){
        peripheral.simpleBlink(2, GREEN);
        delay(500);
      }
      else if (savedISO == ISO_SX70){
        peripheral.simpleBlink(1, GREEN);
        delay(500);
      }
      else{
      #if SIMPLEDEBUG
        output_line_serial(F("No ISO Selected"));
        peripheral.simpleBlink(5, RED);
      #endif
      }
      #if SIMPLEDEBUG
          output_serial(F("EEPROM READ ISO: "));
          output_line_serial(savedISO);
      #endif
    }
}

void blinkAutomode(){
  if ((current_status.switch1 != 1) || (current_status.switch2 != 1)) { //Save ISO Mode
    turnLedsOff();
    if(ShutterSpeed[current_status.selector]== AUTO600){
      peripheral.simpleBlink(2, GREEN);
      #if SIMPLEDEBUG
        output_serial(F("Selector at A600, Blink 2"));
        output_line_serial(ShutterSpeed[current_status.selector]);
      #endif
      //return;
    }else if(ShutterSpeed[current_status.selector]== AUTO100){
      peripheral.simpleBlink(1, GREEN);
      #if SIMPLEDEBUG
        output_serial(F("Selector at A100, Blink 1"));
        output_line_serial(ShutterSpeed[current_status.selector]);
      #endif
      //return;
    }
  }
}

void switch2Function(int mode) {
  #if SIMPLEDEBUG
    output_line_serial(F("Self Timer Activated"));
  #endif
  //0 Dongle 1 No dongle
  if (mode == 0) {

    digitalWrite(PIN_LED2, LOW);
    digitalWrite(PIN_LED1, LOW);
    
    openSX70.BlinkTimerDelay (GREEN, RED, 10);

  }
  else if (mode == 1) {
    openSX70.S1F_Unfocus();
    delay(100);
    #if TIMER_MIRROR_UP
      openSX70.shutterCLOSE();
      openSX70.mirrorUP();
    #endif
    delay (10000); //NoDongleMode
    #if TIMER_MIRROR_UP
      openSX70.S1F_Focus();
    #else
      openSX70.S1F_Focus();
      openSX70.shutterCLOSE();
    #endif
    delay(1000);
  } 
  else{
    //undefined
  }
  #if SIMPLEDEBUG
    output_line_serial(F("Self Timer End"));
  #endif
}

void LightMeterHelper(byte ExposureType){
  if(openSX70.getLIGHTMETER_HELPER()){
    meter_led(current_status.selector, ExposureType);
  }
}

void viewfinderBlink(uint8_t LEDPIN){
  for(uint8_t i=0; i<2; i++){
    digitalWrite(LEDPIN, HIGH);
    delay(100);
    digitalWrite(LEDPIN, LOW);
    delay(100);
  }
}

void saveISO(int iso){
  WriteISO(iso);
  savedISO = iso;
  meter_set_iso(iso);
}

void BlinkISORed() { //read the active ISO and blink once for SX70 and twice for 600 - on ISO change
  #if SIMPLEDEBUG
      output_serial(F("Blinking ISO change"));
  #endif
  turnLedsOff();
  if (savedISO == ISO_SX70){
    peripheral.simpleBlink(1, RED);
  }
  else if (savedISO == ISO_600){
    peripheral.simpleBlink(2, RED);
  }
}

void S1ISOSwap(){
  int _selectedISO;

  if(digitalRead(PIN_S1) == HIGH){
    savedISO = ReadISO();
    if (savedISO == ISO_600) { 
      #if DEBUG
        output_line_serial("ISO HAS BEEN SWAPPED TO: SX70");
      #endif
      _selectedISO = ISO_SX70;
      viewfinderBlink(PIN_LED1);
    }
    else if ((savedISO == ISO_SX70)) {
      #if DEBUG
        output_line_serial("ISO HAS BEEN SWAPPED TO: 600");
      #endif
      _selectedISO = ISO_600;
      viewfinderBlink(PIN_LED2);
    }
    else{
      //Boards come without a set ISO, this will set the ISO to 600 without a dongle.
      #if DEBUG
        output_line_serial("ISO HAS BEEN SWAPPED TO: 600");
      #endif
      _selectedISO = ISO_600;
      viewfinderBlink(PIN_LED2);
    }

    saveISO(_selectedISO);

    while(digitalRead(PIN_S1) == HIGH){
      //wait....
    }
  }
  sw_S1.Reset();
}

void dongleISOSwap() {
  int _selectedISO;
  savedISO = ReadISO(); //read the savedISO from the EEPROM
  if (((ShutterSpeed[current_status.selector]) == AUTO600)) {
    _selectedISO = ISO_600;
  }
  else if (((ShutterSpeed[current_status.selector]) == AUTO100)) {
    _selectedISO = ISO_SX70;
  }
  else { //Fall back to current ISO if A100 or A600 are not selected
    _selectedISO = savedISO;
  }

  if (savedISO != _selectedISO) { //Check if new ISO is diffrent to the ISO saved in EEPROM
    saveISO(_selectedISO);
    BlinkISORed();
  }
  else{
    savedISO = _selectedISO;
    BlinkISORed(); //Blink ISO Red
  }
}

void validateISO(){
  savedISO = ReadISO();
  if(savedISO != ISO_600 && savedISO != ISO_SX70){
    //ISO in EEPROM is invalid, set to 600
    saveISO(ISO_600);
  }
}
