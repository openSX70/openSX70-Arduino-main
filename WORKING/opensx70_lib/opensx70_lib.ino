#include "Arduino.h"
#include "open_sx70.h"
#include "logging.h"

ClickButton sw_S1(PIN_S1, S1Logic);

uDongle peripheral(PIN_S2);
Camera openSX70(&peripheral);

status current_status;
status previous_status;

int savedISO;
int activeISO;

bool mEXPFirstRun;
bool multipleExposureMode;
static int multipleExposureCounter = 0;
#if MULTIPLE_EXPOSURES_TIMEOUT_ENABLED
  static int multipleExposureLastStartTimestamp = 0; // last time the MX mode started
#endif

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
  currentPicture = ReadPicture();
  #if DEBUG
    serial_init();
    output_line_serial(F("Welcome to openSX70 Version: 05_08_2025 Integrator STM32"));
    output_serial(F("Magic Number: A100="));
    output_serial(String(A100));
    output_serial(F("| A600 ="));
    output_line_serial(String(A600));
    output_serial(F("currentPicture stored in EEPROM: "));
    output_line_serial(String(currentPicture));
  #endif

  io_init();
  //openSX70.solenoid_init();
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

  checkFilmCount();

  savedISO = ReadISO();
  meter_set_iso(savedISO);

  if (digitalRead(PIN_S5) != LOW)
  {
    openSX70.shutterCLOSE();
    openSX70.mirrorDOWN();
    openSX70.shutterOPEN();
    #if SIMPLEDEBUG
    output_line_serial(F("Initialize: mirrorDOWN"));
    #endif
  }

  #if SIMPLEDEBUG
    output_serial(F("currentPicture: "));
    output_line_serial(currentPicture);
  #endif
  #if DONGLE_FREE_ISO_CHANGE
  S1ISOSwap();
  #endif
}

/*LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP*/
void loop() {
  if ((digitalRead(PIN_S1F) == HIGH)){
    preFocus();
  }
  else{
    unfocusing();
  }
  previous_status = current_status;
  current_status = peripheral.get_peripheral_status();
  normalOperation();
  state = STATE_MACHINE[state]();
}

camera_state do_state_darkslide (void) {
  camera_state result = STATE_DARKSLIDE;
  #if SHUTTERDARKSLIDE
  sw_S1.Update();
  if (((sw_S1.clicks == -1) || (sw_S1.clicks == 1)) || (digitalRead(PIN_S8) == LOW)){
  #endif
    if (digitalRead(PIN_S8) == HIGH && digitalRead(PIN_S9) == LOW){
      currentPicture = 0; 
      WritePicture(currentPicture);
      checkFilmCount();
      if(peripheral.checkDongle() == 0){
        openSX70.darkslideEJECT(); 
      }
      else{
        peripheral.dongleLed(GREEN, HIGH); //green uDongle LED on while ejecting Darkslide
        openSX70.darkslideEJECT();
        peripheral.dongleLed(GREEN, LOW); //switching off green uDongle LED
      }
      #if SIMPLEDEBUG
        output_line_serial(F("STATE1: EJECT DARK SLIDE"));
        output_serial(F("currentPicture on Darkslide eject: "));
        output_line_serial(currentPicture);
      #endif
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
      saveISOChange();
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
  
  if(current_status.selector<=11){
    LightMeterHelper(2); //LMHelper Manual Mode
  }
  else if(ShutterSpeed[current_status.selector] == A100 || ShutterSpeed[current_status.selector] == A600){
    LightMeterHelper(1); //LMHelper Auto Mode
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
    checkFilmCount();
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
    checkFilmCount();
  }
  #if DOUBLECLICK
  if (sw_S1.clicks == 2){
    beginExposure();
    switch2Function(1);
    openSX70.AutoExposureFF(savedISO);
    sw_S1.Reset();
    checkFilmCount(); 
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
  camera_state result = STATE_MULTI_EXP;
  DongleInserted();


  // TODO May want to remove lmhelper for MEXP
  if(current_status.selector<11){
    LightMeterHelper(2); //LMHelper Manual Mode
  }
  else if(current_status.selector==14 || current_status.selector==15){
    LightMeterHelper(1); //LMHelper Auto Mode
  }

  
  if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0)){
    LightMeterHelper(0); //Turns off LMHelper on picutre Taking
    if(current_status.switch1){ //Why Switch1 == true?!    This is for checking if we are still taking mexp pics
      if(mEXPFirstRun){
        beginExposure();
        mEXPFirstRun = false;
        #if MULTIPLE_EXPOSURES_TIMEOUT_ENABLED
          multipleExposureLastStartTimestamp = millis();
        #endif
      }
      if(current_status.switch2){
        switch2Function(0); // start self timer 
      }

      if(current_status.selector<12){ //MANUAL SPEEDS  
        openSX70.ManualExposure(savedISO, current_status.selector);;
        multipleExposureCounter++;
      }
      else if(ShutterSpeed[current_status.selector] == POST){ //POST
        turnLedsOff();
        openSX70.ShutterT();
        multipleExposureCounter++;
      }
      else if(ShutterSpeed[current_status.selector] == POSB){ //POSB
        turnLedsOff(); //why?
        openSX70.ShutterB();
        multipleExposureCounter++;
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
        multipleExposureCounter++;
      }
    }
    else if(current_status.switch1 == 0 && multipleExposureCounter > 0){
      openSX70.multipleExposureLastClick();
      checkFilmCount();
      multipleExposureCounter = 0;
      #if MULTIPLE_EXPOSURES_TIMEOUT_ENABLED
        multipleExposureLastStartTimestamp = 0;
      #endif
      result = STATE_DONGLE;

      #if STATEDEBUG
        output_line_serial(F("TRANSITION TO STATE_DONGLE FROM STATE_MULTI_EXP"));
      #endif
    }
    sw_S1.Reset();
  }

  #if MULTIPLE_EXPOSURES_TIMEOUT_ENABLED
    // Finish multiple exposure due to timeout.
    if(multipleExposureLastStartTimestamp != 0 && millis() - multipleExposureLastStartTimestamp > MULTIPLE_EXPOSURES_TIMEOUT){
      peripheral.simpleBlink(2, RED);
      openSX70.multipleExposureLastClick();
      checkFilmCount();
      multipleExposureCounter = 0;
      multipleExposureLastStartTimestamp = 0;
    }
  #endif

  if(current_status.switch1 == false && multipleExposureCounter == 0){
    result = STATE_DONGLE;
    multipleExposureMode = false;
    #if STATEDEBUG
      output_line_serial(F("TRANSITION TO STATE_DONGLE FROM STATE_MULTI_EXP"));
    #endif
  }

  return result;
}

void preFocus() {
  if ((digitalRead(PIN_S1F) == HIGH)) { // S1F pressed
    openSX70.S1F_Focus();
  }
}

void unfocusing(){
  if ((digitalRead(PIN_S1F) == LOW)) {
    openSX70.S1F_Unfocus();
  }
}

//Added to remove check for multiple exposure mode from standard manual exposure state. 
//Offload the check to multiple exposure state
void beginExposure(){
  currentPicture++;
  WritePicture(currentPicture);

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
    lmEnable(); //added 26.10.
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

void lmEnable(){
  if(current_status.switch1 && current_status.switch2){
    if(current_status.selector == 12){ //POST
      if (openSX70.getLIGHTMETER_HELPER() == false) {
        openSX70.setLIGHTMETER_HELPER(true);
        turnLedsOff();
        digitalWrite(PIN_LED2, HIGH); //Blink Blue -- LMH On
        peripheral.simpleBlink(1, GREEN);
        delay(100);
        digitalWrite(PIN_LED2, LOW);
        #if SIMPLEDEBUG
          output_line_serial(F("Lightmeter is on"));
        #endif
      }
    }
    else if(current_status.selector == 13){ //POSB
      if (openSX70.getLIGHTMETER_HELPER() == true) {
        openSX70.setLIGHTMETER_HELPER(false);
        turnLedsOff();
        digitalWrite(PIN_LED1, HIGH); //Blink RED -- LMH Off
        peripheral.simpleBlink(1, RED);
        delay(100);
        digitalWrite(PIN_LED1, LOW);
      #if SIMPLEDEBUG
        output_line_serial(F("Lightmeter is off"));
      #endif
      }
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
      checkFilmCount();
      //return;
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
    checkFilmCount(); //Needed to Check if Counter is on 8 and 9 or 10 to activate LED again
  }
}

void BlinkISORed() { //read the active ISO and blink once for SX70 and twice for 600 - on ISO change
  #if SIMPLEDEBUG
      output_serial(F("Blinking ISO change"));
  #endif
  turnLedsOff();
  if (activeISO == ISO_SX70){
    peripheral.simpleBlink(1, RED);
  }
  else if (activeISO == ISO_600){
    peripheral.simpleBlink(2, RED);
  }
  #if SIMPLEDEBUG
    output_serial(F("active ISO: "));
    output_line_serial(activeISO);
  #endif
  checkFilmCount();
  //return;
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

void checkFilmCount(){
  #if EIGHT_SHOT_PACK
    if(currentPicture >= 8){ 
      #if SIMPLEDEBUG
        output_serial(F("All 8 Frames shot!"));
        output_serial(F(", currentPicture: "));
        output_line_serial(currentPicture);
      #endif
      peripheral.dongleLed(GREEN, LOW);
      peripheral.dongleLed(RED, HIGH);
      return;
    }
  #else
  if ((currentPicture == 8) || (currentPicture == 9)){
      #if SIMPLEDEBUG
        output_serial(F("Two Frames left!"));
        output_serial(F(", currentPicture on Two Frames left: "));
        output_line_serial(currentPicture);
      #endif
      //peripheral.simpleBlink(2, RED);
      peripheral.dongleLed(RED, LOW);
      peripheral.dongleLed(GREEN, HIGH);
      return;
  }
  else if(currentPicture == 10){ 
    #if SIMPLEDEBUG
      output_serial(F("All ten Frames shot!"));
      output_serial(F(", currentPicture: "));
      output_line_serial(currentPicture);
    #endif
    peripheral.dongleLed(GREEN, LOW);
    peripheral.dongleLed(RED, HIGH);
    return;
  }
  #endif
}

void normalOperation(){
  if (digitalRead(PIN_S8) == LOW && digitalRead(PIN_S9) == LOW){
      //WHAT TO DO WHEN POWER-UP:
      //  S8     S9
      // closed  open  --> EJECT DARKSLIDE (DEFAULT)
      // open  closed --> FILM REACH 0 (NO FLASH)
      // open   open  --> NORMAL OPERATION 10 TO 1
      // ///////////////////////////////////PICTURE TAKING OPERATION//////////////////////////////////////////////////
      //    FOUR CASES:
      //   *  CASE 1 NORMAL OPERATION: FULL CYCLE
      //   *  SELECTOR = NORMAL (LOW)
      //   *  MXSHOTS = 0
      //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
      //   *
      //   *  CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
      //   *  SELECTOR = DOUBLE (HIGH)
      //   *  MXSHOTS = 0
      //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
      //   *
      //   *  CASE 3 DOUBLE EXPOSURE ULTERIOR MXSHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
      //   *  SELECTOR = DOUBLE (HIGH)
      //   *  MXSHOTS >= 1
      //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
      //   *
      //   *  CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
      //   *
      //   *  SELECTOR = NORMAL (LOW)
      //   *  MXSHOTS >= 1
    sw_S1.Update();
  }
}

void saveISOChange() {
  int _selectedISO;
  savedISO = ReadISO(); //read the savedISO from the EEPROM
  if (((ShutterSpeed[current_status.selector]) == AUTO600)) {
    _selectedISO = ISO_600;
  }
  else if (((ShutterSpeed[current_status.selector]) == AUTO100)) {
    _selectedISO = ISO_SX70;
  }
  else {
    //Fall back to current ISO if A100 or A600 are not selected
    _selectedISO = DEFAULT_ISO;
  }
  if (savedISO != _selectedISO) { //Check if new ISO is diffrent to the ISO saved in EEPROM
    #if SIMPLEDEBUG
      output_serial(F("SaveISOChange() Function: "));
      output_serial(F("ISO has changed, previos saved ISO (from EEPROM): "));
      output_line_serial(savedISO);
      output_serial(F("Saving new selected ISO "));
      output_serial(_selectedISO);
      output_line_serial(F(" to the EEPROM"));
    #endif
    activeISO = _selectedISO; //Save selectedISO to volatile Variable activeISO
    WriteISO(_selectedISO); //Write ISO to EEPROM
    savedISO = ReadISO();
    meter_set_iso(activeISO);
    BlinkISORed();

  }
  else{
    #if SIMPLEDEBUG
      output_serial(F("SaveISOChange() Function: "));
      output_line_serial(F("savedISO is equal to selected ISO, dont save!"));
    #endif
    activeISO = _selectedISO;
    BlinkISORed(); //Blink ISO Red
  }
}

void LightMeterHelper(byte ExposureType){
  if(openSX70.getLIGHTMETER_HELPER()){
    meter_led(current_status.selector, ExposureType);
  }
}

#if DONGLE_FREE_ISO_CHANGE
void viewfinderBlink(uint8_t LEDPIN){
  digitalWrite(LEDPIN, HIGH);
  delay(100);
  digitalWrite(LEDPIN, LOW);
  delay(100);
  digitalWrite(LEDPIN, HIGH);
  delay(100);
  digitalWrite(LEDPIN, LOW);
}

void S1ISOSwap(){
  int _selectedISO;
  //sw_S1.Update();

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
    activeISO = _selectedISO;
    WriteISO(_selectedISO);
    savedISO = ReadISO();
    while(digitalRead(PIN_S1) == HIGH){
      //wait....
    }
  }
  sw_S1.Reset();
}
#endif
