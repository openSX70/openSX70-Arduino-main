 #include "Arduino.h"
#include "open_SX70.h"
//Version 25_08_2020_Meroe2_TCS327T and UDONGLE
// checkISOChange(); makes the Dongle blink on Dialing on Auto600 and SX70 Mode to show what ISO is selected
//Exposure Helper now blinks on faster or slower predicted Exposure Values than possible Shutterspeeds with margin ~1 Aperture
//Stop the Timer on T and B Mode and after each Exposure made
//Has the Sonar S1 logic and restrictions so the camera is waiting for a new picture to make till the S1F is depressed again to prevent multiple Pictures on longpress
//Implemented Rotary Switch debounce -- not active in this version
//Cleaned the ISO select functions
//Changed the Sensor Sensetivity for the TCS3200 to 20% and Changed the Magic numbers
//ClickButton sw_S1(PIN_S1, S1Logic, CLICKBTN_PULLUP);
ClickButton sw_S1(PIN_S1, S1Logic);

int selector ;
bool switch1 ;
bool switch2 ;

uDongle myDongle (PIN_S2);
Camera openSX70(&myDongle);
byte prev_selector = 0;
byte prev_switch1 = 0;
byte prev_switch2 = 0;
int prevDongle ;
int nowDongle ;
int savedISO;
int activeISO;
static int checkedcount;
static int inizialized = 0;
static int metercount;
static int multipleExposureCounter = 0;
/*
#if SONAR == 0 
  bool GTD = 1; //For non Sonar Models
#endif
*/
#if SONAR
  bool FT = 0;
  bool S1F = 0;
  bool isFocused = 0; //neccessary? should be done by GTD???
  int currentPicOnFocus; //dont know what this is for
#endif
//long oldMillis = 0;
//byte firstRun = 0;

//https://electronics.stackexchange.com/questions/95569/best-practice-to-keep-main-in-embedded-systems/96415#96415
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
  /* 
     I have absolutely no clue why I need this here but I do. My software flips out if 
     I do not have this. It is maddening. There is nothing going over Serial but hey if it works...
     Will remove the Serial.begin once I figure out why the hell it makes things work.
  */
  Serial.begin (9600);

  #if DEBUG
    Serial.println("Version: 20.10.2020 - Meroe2 - TCS3200 100%sensetivity - green Filter");
    Serial.println("State machine core by Zane Pollard, Sonar code by Hannes");
    Serial.println("Magic Number: A100 400 / A600 150");
  #endif
  
  myDongle.initDS2408();
  init_EEPROM(); //#writes Default ISO to EEPROM
  // (These are default if not set, but changeable for convenience)

  sw_S1.debounceTime   = 15;   // Debounce timer in ms 15
  sw_S1.multiclickTime = 250;  // Time limit for multi clicks
  sw_S1.longClickTime  = 10; // time until "held-down clicks" register

  io_init();
  metercount = 0; //For the Lightmeter Helper Skipping Function
  checkFilmCount();
  inizialized++;

  if (digitalRead(PIN_S5) != LOW)
  {
    openSX70.mirrorDOWN();
    #if SIMPLEDEBUG
      Serial.println ("Initialize: mirrorDOWN");
    #endif
  }

  #if SIMPLEDEBUG
    Serial.print("Inizialized: ");
    Serial.println(inizialized);
  #endif
  
}

void loop() {//loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop
  #if SONAR
    prefocus();
    unfocusing();
  #endif
  normalOperation();
  state = STATE_MACHINE[state]();
}

camera_state do_state_darkslide (void) {
  camera_state result;
  if (digitalRead(PIN_S8) == HIGH && digitalRead(PIN_S9) == LOW){
    currentPicture = 0; 
    WritePicture(currentPicture);
    checkFilmCount(); //For Filmpack Status
    //OPTION TURN ON AND OFF LED WHILE DARKSLIDE EJECT
    if (nowDongle != 0) {
      myDongle.dongleLed (GREEN, HIGH); //green uDongle LED on while ejecting Darkslide
    }
    openSX70.darkslideEJECT(); //Disabled Darkslide eject to change Filmpack in Darkroom
    if (nowDongle != 0) { 
      myDongle.dongleLed (GREEN, LOW); //switching off green uDongle LED
    } 
    #if SIMPLEDEBUG
        Serial.println("STATE1: EJECT DARK SLIDE");
        Serial.print("currentPicture on Darkslide eject: ");
        Serial.println(currentPicture);
    #endif
  }
  else{
    if ((selector <= 15) && (myDongle.checkDongle() > 0)){ //((selector <= 15) && (myDongle.checkDongle() > 0))
      result = STATE_DONGLE;
      BlinkISO();
    }
    else if ((selector == 100) && (myDongle.checkDongle() == 0)){
      result = STATE_FLASHBAR;
    }
    else{
      result = STATE_NODONGLE;
    }
  }
  
  
  return result;
}

camera_state do_state_noDongle (void){
  camera_state result = STATE_NODONGLE;

  savedISO = ReadISO();
  //LightMeterHelper(0);
  LightMeterHelper(0); //Added 05.06.2020
  if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1)){
    switch1 = 0; //necessary?
    openSX70.AutoExposure(savedISO, false);
    sw_S1.Reset();
  }
  if (sw_S1.clicks == 2){ //Doubleclick the Red Button with no Dongle inserted
    switch1 = 0; //necessary?
    delay (10000);
    openSX70.AutoExposure(savedISO, false);
    sw_S1.Reset();
  }

  //Checks for dongle or flashbar insertion
  if (myDongle.checkDongle() > 0){ //((selector <= 15) && (myDongle.checkDongle() > 0))
    result = STATE_DONGLE;
    BlinkISO();
  }
  else if ((selector == 100) && (myDongle.checkDongle() == 0)){
    result = STATE_FLASHBAR;
  }
  return result;
}

camera_state do_state_dongle (void){
  camera_state result = STATE_DONGLE;

  DongleInserted();

  if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0)){
    LightMeterHelper(1);
    if(switch2 == 1){

    }
    //TODO ADD ANALOGWORKS DONGLE OPTION. SAVES COMPARE TIME.
    #if UDONGLE
      if((selector>=0) && (selector<12)){ //MANUAL SPEEDS
        switch2Function(0); //switch2Function Manual Mode
        openSX70.ManualExposure(selector, false);
      }
      else if(selector == 12){ //POST
        #if SIMPLEDEBUG
          Serial.println("POS T");
        #endif
        lmTimer_stop();
        turnLedsOff();
        openSX70.ShutterT(false);
        checkFilmCount();
      }
      else if(selector == 13){ //POSB
        #if SIMPLEDEBUG
          Serial.println("POS B");
        #endif
        lmTimer_stop();
        turnLedsOff(); //why?
        openSX70.ShutterB(false);
        checkFilmCount();
      }
      else{ //Auto catch-all. Passes the value stored in the ShutterSpeed list at the selector value
        openSX70.AutoExposure(ShutterSpeed[selector], false); 
      }
    #endif
    sw_S1.Reset();
  } 

  // Dongle Removed
  if (myDongle.checkDongle() == 0){
    result = STATE_NODONGLE;
  } 
  // Multiple Exposure switch flipped
  else if (switch1 == 1){
    result = STATE_MULTI_EXP;
  }

  return result;
}

camera_state do_state_flashBar (void){
  camera_state result = STATE_FLASHBAR;

  if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1))
  {
    openSX70.FlashBAR();
    sw_S1.Reset();
    checkFilmCount();
  }
  if (sw_S1.clicks == 2)
  {
    switch2Function(3); //Switch Two Function in Flash Mode
    openSX70.FlashBAR();
    sw_S1.Reset();
    checkFilmCount(); 
  } 


  if ((selector == 200) && (myDongle.checkDongle() == 0)){
    result = STATE_NODONGLE;
  } 

  return result;
}

camera_state do_state_multi_exp (void){
  camera_state result = STATE_MULTI_EXP;

  DongleInserted();

  if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0)){
    if(switch1 == 1){
      //TODO again add analog dongle
      #if UDONGLE
        if((selector>=0) && (selector<12)){ //MANUAL SPEEDS
          switch2Function(0); //switch2Function Manual Mode
          openSX70.ManualExposure(selector, true);
          multipleExposureCounter++;
        }
        else if(selector == 12){ //POST
          #if SIMPLEDEBUG
            Serial.println("POS T");
          #endif

          lmTimer_stop();
          turnLedsOff();
          openSX70.ShutterT(true);
          checkFilmCount();
          multipleExposureCounter++;
        }
        else if(selector == 13){ //POSB
          #if SIMPLEDEBUG
            Serial.println("POS B");
          #endif

          lmTimer_stop();
          turnLedsOff(); //why?
          openSX70.ShutterB(true);
          checkFilmCount();
          multipleExposureCounter++;
        }
        //TODO maybe get rid of auto on multiple exposure? unless maybe we do a half time auto. make it just two images
        else{ //Auto catch-all. Passes the value stored in the ShutterSpeed list at the selector value
          openSX70.AutoExposure(ShutterSpeed[selector], true); 
          multipleExposureCounter++;
        }
      #endif
      
    }

    else if(switch1 == 0 && multipleExposureCounter > 0){
      openSX70.multipleExposureLastClick();
      multipleExposureCounter = 0;
      Serial.println("TO DONGLE sw1=0 mex>0");
      result = STATE_DONGLE;
    }
    sw_S1.Reset();
    Serial.println(multipleExposureCounter);
  }

  if(switch1 == 0 && multipleExposureCounter == 0){
    Serial.println("to dongle 0 0");
    result = STATE_DONGLE;
  }
  return result;
}

#if SONAR
void printReadings() {
  //Serial.print("GTD: ");
  //Serial.print(openSX70.getGTD());
  Serial.print("Is Focused: ");
  Serial.print(isFocused);
  Serial.print(" | GTD: ");
  Serial.print(analogRead(PIN_GTD));
  Serial.print(" | S1F: ");
  Serial.print(digitalRead(PIN_S1F));
  Serial.print(" | FT: ");
  Serial.print(analogRead(PIN_FT));
  Serial.print(" | FF: ");
  Serial.println(digitalRead(PIN_FF));
  return;
}

void preFocus() {
  if ((digitalRead(PIN_S1F) == HIGH) && (isFocused == 0)) { // S1F pressed
    openSX70.S1F_Focus();
    currentPicOnFocus = currentPicture;
    isFocused = 1;
    return;
  }
}

void unfocusing(){
  //delay(100);
  if ((digitalRead(PIN_S1F) == LOW) && (isFocused == 1)) { // S1F pressed  -- selftimer (doubleclick the red button) is not working this way
    //delay(100);
    openSX70.S1F_Unfocus();
    //currentPicOnFocus = currentPicture;
    isFocused = 0;
    turnLedsOff();
    return;
  }
}
#endif

void turnLedsOff(){ //todo:move to camerafunction
   digitalWrite(PIN_LED1, LOW);
   digitalWrite(PIN_LED2, LOW);
   //delay(400); 
}

#if SONAR
void getGTD(){
  GTD = digitalRead(PIN_GTD);
}

void getS1F(){
  S1F = digitalRead(PIN_S1F);
}

void getFT(){
   FT = digitalRead(PIN_FT);
}

void printReadings(){
  Serial.print("GTD: ");
  Serial.print(GTD);
  Serial.print(" | S1F: ");
  Serial.print(S1F);
  Serial.print(" | FT: ");
  Serial.print(FT);
  Serial.print(" | FF: ");
  Serial.println(digitalRead(PIN_FFA));
  }
#endif

void DongleInserted() { //Dongle is pressend LOOP
  if (digitalRead(PIN_S1) != S1Logic) {
  #if SONAR
    if (digitalRead(PIN_S1F) != S1Logic) { //Dont run DongleInserted Function on S1F pressed
  #endif
      { //Serial.println("S1F HIGH");
        selector = myDongle.selector();
        switch1 = myDongle.switch1();
        switch2 = myDongle.switch2();
        saveISOChange();//Moved here form loop 11.06.
        //BlinkISO(); //check if dongle inserted, read the default ISO and blink once for SX70 and twice for 600.
        if ((selector != prev_selector)) //Update Dongle changes
        {
          #if ADVANCEDEBUG
            Serial.print ("DONGLE Mode:  ");
            Serial.print ("Selector: ");
            Serial.print (selector);
            Serial.print (" Switch1: ");
            Serial.print (switch1);
            Serial.print (" Switch2: ");
            Serial.print (switch2);
            Serial.print (" speed: ");
            Serial.println (ShutterSpeed[selector]);
          #endif
          blinkAutomode();
          blinkSpecialmode(); //B and T Mode Selector LED Blink
          prev_selector = selector;
          return;
        }
        if ( (switch1 != prev_switch1) || (switch2 != prev_switch2)) {
          #if ADVANCEDEBUG
            Serial.print ("DONGLE Mode:  ");
            Serial.print ("Selector: ");
            Serial.print (selector);
            Serial.print ("     Switch1: ");
            Serial.print (switch1);
            Serial.print ("     Switch2: ");
            Serial.print (switch2);
            Serial.print ("        speed: ");
            Serial.println (ShutterSpeed[selector]);
          #endif
          prev_switch1 = switch1;
          prev_switch2 = switch2;
          return;
        }
      }
  #if SONAR
    }
  #endif
  }
}


void BlinkISO() { //read the default ISO and blink once for SX70 and twice for 600
  if((switch2 != 1) || (switch1 != 1)){ //Not Save ISO //Changed to OR 01.06.2020
      #if SIMPLEDEBUG
        Serial.println("Blink for the saved ISO setting on Dongle insertion.");
      #endif
      //blinkAutomode();
      savedISO = ReadISO();
      turnLedsOff();
      if (savedISO == ISO_600){
        myDongle.simpleBlink(2, GREEN);
        delay(500);
      }
      else if (savedISO == ISO_SX70){
        myDongle.simpleBlink(1, GREEN);
        delay(500);
      }
      else{
      #if SIMPLEDEBUG
        Serial.println("No ISO Selected");
        myDongle.simpleBlink(5, RED);
      #endif
      }
      #if SIMPLEDEBUG
          Serial.print ("EEPROM READ ISO: ");
          Serial.println (savedISO);
      #endif
      
      prevDongle = nowDongle;
      checkFilmCount();
      return;
    }
}

void blinkAutomode(){
      if((switch2 != 1) || (switch1 != 1)){ //Not Save ISO Mode
          turnLedsOff();
          if(ShutterSpeed[selector]== AUTO600){
            myDongle.simpleBlink(2, GREEN);
            #if SIMPLEDEBUG
              Serial.println("Blink 2 times Green on Auto600 select");
            #endif
          }
          if(ShutterSpeed[selector]== AUTO100){
            myDongle.simpleBlink(1, GREEN);
            #if SIMPLEDEBUG
              Serial.println("Blink 1 times Green on Auto100 select");
            #endif
          }
      }
}

void blinkSpecialmode(){
  turnLedsOff();
  if((switch2 != 1) || (switch1 != 1)){ //Not Save ISO Mode
    if(ShutterSpeed[selector]== POST){
      myDongle.simpleBlink(1, GREEN);
      myDongle.simpleBlink(1, RED);
    }
    if(ShutterSpeed[selector]== POSB){
      myDongle.simpleBlink(1, RED);
      myDongle.simpleBlink(1, GREEN);
    }
    checkFilmCount();  
  }
  if((switch2 == 1) || (switch1 == 1)){ //Lightmeterhelper Activation Mode
    if(ShutterSpeed[selector]== POST){
      myDongle.simpleBlink(1, GREEN);
      //myDongle.simpleBlink(1, RED);
    }
    if(ShutterSpeed[selector]== POSB){
      myDongle.simpleBlink(1, RED);
      //myDongle.simpleBlink(1, GREEN);
    }
    checkFilmCount();  
  }
}

void BlinkISORed() { //read the active ISO and blink once for SX70 and twice for 600 - on ISO change
    //if ((selector>=14)&&(selector <=15)){ //Blink only on AUTOMODE
      #if SIMPLEDEBUG
          Serial.print("Blink RED on ISO change: ");
      #endif
      turnLedsOff();
      if (activeISO == ISO_SX70){
        myDongle.simpleBlink(1, RED);
      }
      else if (activeISO == ISO_600){
        myDongle.simpleBlink(2, RED);
      }
      #if SIMPLEDEBUG
          Serial.print ("active ISO: ");
          Serial.println (activeISO);
      #endif
      checkFilmCount();
      return;
    //}
}

void switch2Function(int mode) {
  //0 Manual, 1 Auto600, 2 AutoSX70, FlashBar
  if ((switch2 == 1) || (sw_S1.clicks == 2))
  {
    if (mode == 0) {
      openSX70.SelfTimerMUP();
      digitalWrite(PIN_LED2, LOW);
      digitalWrite(PIN_LED1, LOW);
      openSX70.BlinkTimerDelay (GREEN, RED, 10);
      //preFocus();
    }
    else if (mode == 1) {
      openSX70.SelfTimerMUP();
      digitalWrite(PIN_LED2, LOW);
      digitalWrite(PIN_LED1, LOW);
      openSX70.BlinkTimerDelay (GREEN, RED, 10);
      //preFocus();
    } else if (mode == 2) {
      openSX70.SelfTimerMUP();
      digitalWrite(PIN_LED2, LOW);
      digitalWrite(PIN_LED1, LOW);
      openSX70.BlinkTimerDelay (GREEN, RED, 10);
      //preFocus();
    } else if (mode == 3) {
      openSX70.S1F_Unfocus();
      openSX70.SelfTimerMUP();
      Serial.println("Selftimer");
      delay (10000); //NoDongleMode
      //preFocus();
      openSX70.S1F_Focus();
      delay(1000);
    }
    else {
      //return false;
      return;
    }
  }
  return;
}

void checkFilmCount(){
  if ((currentPicture == 8) || currentPicture == 9){
      #if SIMPLEDEBUG
        Serial.print("Two Frames left!");
        Serial.print(", currentPicture on Two Frames left: ");
        Serial.println(currentPicture);
      #endif
      //myDongle.simpleBlink(2, RED);
      myDongle.dongleLed(RED, LOW);
      myDongle.dongleLed(GREEN, HIGH);
      return;
  }
  else if(currentPicture == 10){ 
    #if SIMPLEDEBUG
      Serial.print("Ten Frames shot!");
      Serial.print(", currentPicture: ");
      Serial.println(currentPicture);
    #endif
    myDongle.dongleLed(GREEN, LOW);
    myDongle.dongleLed(RED, HIGH);
    return;
  }
}


void ispackEmpty(){
  static int firstRun = 0;
  //STATE 2: PACK IS EMPTY--> NO WASTE OF FLASH
  //Camera Counter is Zero and Switch S9 is CLOSED
  // changed this to allow shooting until counter is actually 0, in case "something" happens and I loose count!
  if ((digitalRead(PIN_S8) == LOW && digitalRead(PIN_S9) == HIGH) && (currentPicture >= 8))
  {
    if (firstRun==0){ //Run only one time when Switch S9 change to HIGH
      firstRun++;
      if  (nowDongle != 0) {
        //Serial.println("STATE2: Set LED RED to High");
        //myDongle.dongleLed(RED, HIGH);
      }
      #if SIMPLEDEBUG
          Serial.print("STATE2: PACK IS EMPTY - S9 Closed");
          Serial.print(", Current Picture on Empty Pack: ");
          Serial.println(currentPicture);
      #endif
    }
  }
}

void normalOperation(){
  //STATE 3: NORMAL OPERATION *************************************************************************************************************************************************
  if (digitalRead(PIN_S8) == LOW && digitalRead(PIN_S9) == LOW) //S8 and S9 are OPEN
  {
      //WHAT TO DO WHEN POWER-UP:
      //  S8     S9
      // closed  open  --> EJECT DARKSLIDE (DEFAULT)
      // open  closed --> FILM REACH 0 (NO FLASH)
      // open   open  --> NORMAL OPERATION 10 TO 1
      // ///////////////////////////////////PICTURE TAKING OPERATION//////////////////////////////////////////////////
      //    FOUR CASES:
      //   *  CASE 1 NORMAL OPERATION: FULL CYCLE
      //   *  SELECTOR = NORMAL (LOW)
      //   *  NXSHOTS = 0
      //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
      //   *
      //   *  CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
      //   *  SELECTOR = DOUBLE (HIGH)
      //   *  MXSHOTS = 0
      //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
      //   *
      //   *  CASE 3 DOUBLE EXPOSURE ULTERIOR MXSHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
      //   *  SELECTOR = DOUBLE (HIGH)
      //   *  NXSHOTS >= 1
      //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
      //   *
      //   *  CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
      //   *
      //   *  SELECTOR = NORMAL (LOW)
      //   *  MXSHOTS >= 1
      sw_S1.Update();
  }
}

void LightMeterHelper(byte ExposureType){
    int helperstatus = openSX70.getLIGHTMETER_HELPER();
    if(helperstatus==true){
      //if(metercount==2){ //Lightmeter only on every 3th Cycle of Loop
        meter_led(selector, ExposureType);
        metercount=0;
        /*#if ADVANCEDEBUG
          Serial.print("Lightmeter Helper Status:");
          Serial.print(helperstatus);
          Serial.print(", ExposureType:  ");
          Serial.print(ExposureType);
          Serial.print(", Selector: ");
          Serial.println(selector);
        #endif*/
      //}
      //else{
      //  metercount++;
      //}
    }
}

void saveISOChange() {
  int _selectedISO;
  selector = myDongle.selector();
  //TODO ADD ANALOGDONGLE MODE!!!!!!!!!!!
  #if UDONGLE
    if (selector >= 14 && selector <= 15) { // Only Save on Automode selector slots! Need to be changed if the Selector order changes!!
      savedISO = ReadISO(); //read the savedISO from the EEPROM
      if (((ShutterSpeed[selector]) == AUTO600)) {
        if (_selectedISO != ISO_600) {
          _selectedISO = ISO_600;
        }
      }
      else if (((ShutterSpeed[selector]) == AUTO100)) {
        if (_selectedISO != ISO_SX70) {
          _selectedISO = ISO_SX70;
        }
      }
      else {
        //no ISO Selected
        _selectedISO = DEFAULT_ISO;
      }
      if (savedISO != _selectedISO) { //Check if new ISO is diffrent to the ISO saved in EEPROM
      #if SIMPLEDEBUG
        Serial.print("SaveISOChange() Function: ");
        Serial.print("ISO has changed, previos saved ISO (from EEPROM): ");
        Serial.println(savedISO);
        Serial.print("Saving new selected ISO ");
        Serial.print(_selectedISO);
        Serial.println(" to the EEPROM");
      #endif
        activeISO = _selectedISO; //Save selectedISO to volatile Variable activeISO
        WriteISO(_selectedISO); //Write ISO to EEPROM
        BlinkISORed(); //Blink ISO Red
      }
    }
  #endif
}

void checkISOChange() {
  int _selectedISO;
  //savedISO = ReadISO(); //read the savedISO from the EEPROM
  if (nowDongle != 0) { //Donngle is present
    if ((switch2 != 1) && (switch1 != 1)) { //Save ISO Mode not active
      //savedISO = ReadISO(); //read the savedISO from the EEPROM
      if (((ShutterSpeed[selector]) == AUTO600)) {
        if (_selectedISO != ISO_600) {
          _selectedISO = ISO_600;
        }
      }
      else if (((ShutterSpeed[selector]) == AUTO100)) {
        if (_selectedISO != ISO_SX70) {
          _selectedISO = ISO_SX70;
        }
      }
      else { //All other modes
        _selectedISO = ReadISO();  //read from EEPROM
        //_selectedISO = DEFAULT_ISO;
        //Serial.println("ISO ERROR: selecting Default ISO as selected ISO");
      }
      
      if (activeISO != _selectedISO) {
        activeISO = _selectedISO;
        #if SIMPLEDEBUG
        Serial.print("checkISOChange() Function: ISO has changed, make activeISO = selectedISO, savedISO:");
        Serial.println(savedISO);
        Serial.print("selectedISO: ");
        Serial.print(_selectedISO);
        Serial.print(" activeISO: ");
        Serial.println(activeISO);
        #endif
        return;
        //blinkAutomode();
      }
    }
  }else { //no Dongle pressend
    savedISO = ReadISO(); //read the savedISO from the EEPROM
    if (activeISO != savedISO) {
      #if SIMPLEDEBUG
      Serial.print("checkISOChange Function - noDongle detected: ISO has changed, savedISO:");
      Serial.print(savedISO);
      Serial.print(" activeISO: ");
      Serial.println(activeISO);
      #endif
      activeISO = savedISO;
      #if SIMPLEDEBUG
      Serial.print(" make activeISO = selectedISO, savedISO:");
      Serial.print(savedISO);
      Serial.print(" activeISO: ");
      Serial.println(activeISO);
      #endif
      return;
    }
  }
}

