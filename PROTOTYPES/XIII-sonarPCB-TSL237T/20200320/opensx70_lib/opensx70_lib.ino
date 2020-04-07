/*
**the openSX70 project**

  It is many things at once, but simply put, openSX70 is an open source
  (hardware and software) project that aims to take the SX70 beyond what
  is possible now in a cheap and non destructive way.
  https://opensx70.com/

  https://github.com/openSX70

  As a legal reminder please note that the code and files is under Creative Commons
  "Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)" is free and open
  for hobbyist NON-COMMERCIAL USE.
  https://creativecommons.org/licenses/by-nc/4.0/

  You are free to:
  Share — copy and redistribute the material in any medium or format
  Adapt — remix, transform, and build upon the material
  The licensor cannot revoke these freedoms as long as you follow the license terms.
  Under the following terms:
  Attribution — You must give appropriate credit, provide a link to the license,
  and indicate if changes were made. You may do so in any reasonable manner, but
  not in any way that suggests the licensor endorses you or your use.

  NonCommercial — You may not use the material for commercial purposes.

  No additional restrictions — You may not apply legal terms or technological
  measures that legally restrict others from doing anything the license permits.

  Notices:
  You do not have to comply with the license for elements of the material in the
  public domain or where your use is permitted by an applicable exception or limitation.

  No warranties are given. The license may not give you all of the permissions necessary
  for your intended use. For example, other rights such as publicity, privacy, or moral r
  ights may limit how you use the material.

  I know this software is "not ok" but I am doing my best to improve it and learn at the same time.
  Parts have been use from Pierre-Loup Martin "structure" branch, but, by no means this compares to his works.
  https://github.com/troisiemetype/openSX70-Arduino-main/tree/structure

  Nevertheless I am trying to learn, step by step from him.

*/
#include "Arduino.h"
#include "open_SX70.h"


//
ClickButton sw_S1(PIN_S1, S1Logic, CLICKBTN_PULLUP);
//ClickButton sw_S1(PIN_S1, HIGH);


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
int myISO;

//long oldMillis = 0;


void setup() {//setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup setup
  //#if SIMPLEDEBUG
  Serial.begin (9600);
  //#endif
  myDongle.initDS2408();
  init_EEPROM();

  // OPTION RED BUTTON (S1) (These are default if not set, but changeable for convenience)
  sw_S1.debounceTime   = 15;   // Debounce timer in ms
  sw_S1.multiclickTime = 250;  // Time limit for multi clicks
  sw_S1.longClickTime  = 300; // time until "held-down clicks" register

  io_init();

  //  Serial.println ("reset happened");
  if (digitalRead(PIN_S5) != LOW)
  {
    openSX70.mirrorDOWN();
#if SIMPLEDEBUG
    Serial.println ("mirrorDOWN");
#endif
  }

}

void loop() {//loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop loop

  static int metercount = 0;
  //  sw_S1.Update();

  ///    while (sw_S1.depressed)

  selector = myDongle.selector();
  switch1 = myDongle.switch1();
  switch2 = myDongle.switch2();
  prevDongle = nowDongle;

  nowDongle = myDongle.checkDongle();

  currentPicture = ReadPicture();

  myISO = ReadISO();
  //  Serial.print ("ReadISO: ");
  //  Serial.println (myISO);

  //WHAT TO DO WHEN POWER-UP:
  //  S8     S9
  //closed  open  --> EJECT DARKSLIDE (DEFAULT)
  // open  closed --> FILM REACH 0 (NO FLASH)
  // open   open  --> NORMAL OPERATION 10 TO 1

  BlinkISO(); //check if dongle inserted read the default ISO and blink once for SX70 and twice for 600.


  // STATE 1: EJECT DARKSLIDE:*************************************************************************************************************************************************

  if (digitalRead(PIN_S8) == HIGH && digitalRead(PIN_S9) == LOW)
    //EJECT DARK SLIDE
  {
    currentPicture = 0;
    WritePicture(currentPicture);
    //OPTION TURN ON AND OFF LED WHILE DARKSLIDE EJECT
    if (nowDongle != 0) myDongle.dongleLed (6, HIGH); //green uDongle LED on while
    
    openSX70.darkslideEJECT();
    delay (150); //sometimes ejects twice the dark slide
    if (nowDongle != 0) myDongle.dongleLed (6, LOW); //green uDongle LED on while

#if SIMPLEDEBUG
    Serial.println("STATE1: EJECT DARK SLIDE");
#endif
    return;
  }

  //STATE 2: PACK IS EMPTY--> NO WASTE OF FLASH *********************************************************************************************************************************
  //    if ((digitalRead(S8) == LOW && digitalRead(S9) == HIGH) || (CurrentPicture >= 8))

  // changed this to allow shooting until counter is actually 0, in case "something" happens and I loose count!

  if ((digitalRead(PIN_S8) == LOW && digitalRead(PIN_S9) == HIGH) && (currentPicture >= 8))
  {
    if  (nowDongle != 0) myDongle.dongleLed (RED, HIGH);
    //CurrentPicture = 0;
    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE

#if SIMPLEDEBUG
    Serial.println("STATE2: PACK IS EMPTY");
#endif

    // FOR THE MOMENT I JUST TURN ON THE LED ON DONGLE
    // KEEP IN MIND THAT THIS **THE CAMERA** SAYING IT HAS ALREADY MADE 10 SHOTS.
    // I COULD SET MY OWN COUNTER (UP TO 8) AND MAKE IT MORE IMPOSSIBLE-8-SHOTS-FRIENDLY
    myDongle.dongleLed (7, HIGH); //red uDongle LED on
  }

  //STATE 3: NORMAL OPERATION *************************************************************************************************************************************************
  if (digitalRead(PIN_S8) == LOW && digitalRead(PIN_S9) == LOW) //S8 and S9 are OPEN
  {
    currentPicture =  ReadPicture();
    //Serial.println (millis());

    // ///////////////////////////////////PICTURE TAKING OPERATION//////////////////////////////////////////////////
    //    FOUR CASES:
    //   *   CASE 1 NORMAL OPERATION: FULL CYCLE
    //   *  SELECTOR = NORMAL (LOW)
    //   *  SHOTS = 0
    //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
    //   *
    //   *  CASE 2 DOUBLE EXPOSURE FIRST SHOT: MIRROR DOWN AND FIRST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
    //   *  SELECTOR = DOUBLE (HIGH)
    //   *  SHOTS = 0
    //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
    //   *
    //   *  CASE 3 DOUBLE EXPOSURE ULTERIOR SHOTS: NO MOTOR OPERATION JUST PICTURE (CLICK: SHUTTER OPERATION REMAINING CLOSED)
    //   *  SELECTOR = DOUBLE (HIGH)
    //   *  SHOTS >= 1
    //   *  PIN_S1 = LOW (RED BUTTON PRESSED)
    //   *  CASE 4 PICTURE EXPULSION AFTER DOUBLE EXPOSURE: MIRROR DOWN AND SHUTTER OPENING (NO PICTURE TAKEN)
    //   *
    //   *  SELECTOR = NORMAL (LOW)
    //   *  SHOTS >= 1

    sw_S1.Update();

    //if (takePicture == true && switch1 == 0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
    //if (((sw_S1.clicks == -1) || (sw_S1.clicks == 1)) && (shots >= 1) && (myDongle.switch1() == 0 ) )
    if (((sw_S1.clicks == -1) || (sw_S1.clicks == 1)) && (shots >= 1) && (switch1 == 0 ) )
    {
      sw_S1.Reset();
      //sw_S1.Update();
#if SIMPLEDEBUG
      Serial.println("last pict");
#endif

      shots = 0;
      openSX70.mirrorDOWN();
      delay(50);                             //AGAIN is this delay necessary? 100-->50
      openSX70.shutterOPEN();
      return;

    }// end of if (takePicture == true && Read_DS2408_PIO(1) ==  0 && shots >= 1)  //Was in DOUBLE EXPOSURE MODE but NOW is back to NORMAL
    //=========================================================================================================================================================


    /*
          if (sw_S1.clicks == 1)
          { Serial.println ("one click");
            return;

          }
          if (sw_S1.clicks == 2)
          { Serial.println ("two clicks");
            return;
          }
          if (sw_S1.clicks == 3)
          { Serial.println ("three clicks");
            return;

          }
          if (sw_S1.clicks == 4)
          { Serial.println ("four clicks?");
            return;

          }
          if (sw_S1.clicks == -1)
          { Serial.println ("long click");
            return;
          }*/

    /*
        selector = myDongle.selector();
        switch1 = myDongle.switch1();
        switch2 = myDongle.switch2();
    */
    if ((selector == 200) && (myDongle.checkDongle() == 0))
    {
      myISO = ReadISO();

      if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1))
      {
        switch1 = 0;
        openSX70.AutoExposure(myISO);
        sw_S1.Reset();
        //sw_S1.Update();
        return;
      }

      if (sw_S1.clicks == 2)
      {
        switch1 = 0;
        delay (10000);
        openSX70.AutoExposure(myISO);
        sw_S1.Reset();
        return;
      }


      /*
        if (selector != prev_selector)                      //CASE NOTHING INSERTED CASE NOTHING INSERTED CASE NOTHING INSERTED CASE NOTHING INSERTED CASE NOTHING INSERTED CASE NOTHING INSERTED CASE NOTHING INSERTED CASE NOTHING
        {
          myISO = ReadISO();

          Serial.print ("NO DONGLE INSERTED PICTURE:  ");

          currentPicture = ReadPicture();
          Serial.print (currentPicture);
          Serial.print ("READ (DEFAUILT ISO:  ");
          Serial.println (myISO);
          prev_selector = selector;
          return;

        };
      */
    };
    if ((selector == 100) && (myDongle.checkDongle() == 0)) //CASE FLASH INSERTED CASE FLASH INSERTED CASE FLASH INSERTED CASE FLASH INSERTED CASE FLASH INSERTED CASE FLASH INSERTED CASE FLASH INSERTED CASE FLASH INSERTED
    {
      Serial.println ("FLASH INSERTED");
      /*      if (selector != prev_selector)
            {
              Serial.println ("FLASH INSERTED");
                      prev_selector = selector;
              return;
            };*/


      if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1))
      {
        openSX70.FlashBAR();
        sw_S1.Reset();
        //        sw_S1.Update();
        return;
      }

      if (sw_S1.clicks == 2)
      {
        //openSX70.BlinkTimerDelay (GREEN, RED,10);
        delay (10000);
        openSX70.FlashBAR();
        sw_S1.Reset();
        //        sw_S1.Update();
        return;
      };

    };



    if ((selector <= 15) && (myDongle.checkDongle() > 0))  //CASE DONGLE INSERTED CASE DONGLE INSERTED CASE DONGLE INSERTED CASE DONGLE INSERTED CASE DONGLE INSERTED CASE DONGLE INSERTED CASE DONGLE INSERTED CASE DONGLE INSERTED CASE DONGLE
    {
      selector = myDongle.selector();
      switch1 = myDongle.switch1();
      switch2 = myDongle.switch2();
      /*if ((selector != prev_selector) || (switch1 != prev_switch1) || (switch2 != prev_switch2))
        {
        //  Serial.print ("checkDongle:  ");
        //  Serial.print (dongle);
        Serial.print ("DONGLE INSERTED:  ");
        Serial.print ("Selector: ");
        Serial.print (selector);
        Serial.print ("     Switch1: ");
        Serial.print (switch1);
        Serial.print ("     Switch2: ");
        Serial.print (switch2);
        Serial.print ("        speed: ");
        Serial.println (ShutterSpeed[selector]);
        prev_selector = selector;
        prev_switch1 = switch1;
        prev_switch2 = switch2;
        return;
        };*/
      /////////////////////////////////////////////////////
      //=================================================================================================================================================================

      //    if ((sw_S1.depressed) && ((ShutterSpeed[selector] == (POSB)))) //////////////POSITION B
      if ((digitalRead(PIN_S1) == S1Logic) && ((ShutterSpeed[selector] == (POSB)))) //////////////POSITION B
        //    if (ShutterSpeed[selector] == (POSB)) //////////////POSITION B

      {
#if SIMPLEDEBUG
        Serial.println("POS B");
#endif
        digitalWrite(PIN_LED2, LOW);
        digitalWrite(PIN_LED1, LOW);

        openSX70.ShutterB();
      }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POSB))))
      //=================================================================================================================================================================
      if ((digitalRead(PIN_S1) == S1Logic) && ((ShutterSpeed[selector] == (POST)))) //////////////POSITION T
        //    if (ShutterSpeed[selector] == (POST)) //////////////POSITION T
      {
#if SIMPLEDEBUG
        Serial.println("POS T");
#endif
        digitalWrite(PIN_LED2, LOW);
        digitalWrite(PIN_LED1, LOW);
        openSX70.ShutterT();

      }; // END of if ((digitalRead(S1) == LOW)  && ((ShutterSpeed[ActualSlot] == (POST))))


      //==================================================================================================================================================================

      // sw_S1.Update();

      if ((selector >= 0) && (selector < 12)) //Manual Exposure Manual Exposure Manual Exposure Manual Exposure Manual Exposure Manual Exposure Manual Exposure Manual Exposure Manual Exposure Manual Exposure
      {

#if LIGHMETER_HELPER
        if (metercount == 5)
        {
          meter_led(selector, true);
          metercount = 0;

        }
        else
        {
          metercount++;
        }
#endif
        if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0))

        {
          if ((switch2 == 1) || (sw_S1.clicks == 2))
          {
            digitalWrite(PIN_LED2, LOW);
            digitalWrite(PIN_LED1, LOW);
            openSX70.BlinkTimerDelay (GREEN, RED, 10);
          }
          sw_S1.Reset();
          openSX70.ManualExposure();
          return;
        };
      };

      if (((ShutterSpeed[selector]) == AUTO600)) //AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL - AUTO600 WHEEL
      {

        myISO = ISO_600;


#if LIGHMETER_HELPER 
        if (metercount == 5)
        {
          meter_led(selector, false);
          metercount = 0;

        }
        else
        {
          metercount++;
        }
#endif
        if  ((switch2 == 1) && (switch1 == 1))
        {
          if (ReadISO() != ISO_600)
          {
            myISO = ISO_600;
            myDongle.simpleBlink (2, 6);
            WriteISO (myISO);
          }
          //BlinkISO ();
        };
        //        if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1))

        if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0))
        {
          digitalWrite(PIN_LED2, LOW);
          digitalWrite(PIN_LED1, LOW);
          //if (myDongle.switch2() == 1)
          if ((switch2 == 1) || (sw_S1.clicks == 2))
          {
            openSX70.BlinkTimerDelay (GREEN, RED, 10);
          }
          sw_S1.Reset();
          openSX70.AutoExposure(myISO);
          return;
        };
      };

      if (((ShutterSpeed[selector]) == AUTO100)) //AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL - AUTO100 WHEEL
      {

        myISO = ISO_SX70;
#if LIGHMETER_HELPER 
        if (metercount == 5)
        {
         meter_led(selector, false);
          metercount = 0;

        }
        else
        {
          metercount++;
        }
#endif
        if  ((switch2 == 1) && (switch1 == 1))
          //if ((myDongle.switch1() == 1) && (myDongle.switch2() == 1))
        {
          if (ReadISO() != ISO_SX70)
          {
            myISO = ISO_SX70;
            myDongle.simpleBlink (1, 6);
            WriteISO (myISO);
          }
          //BlinkISO ();
        };
        //        if ((sw_S1.clicks == -1) || (sw_S1.clicks == 1))

        if ((sw_S1.clicks == -1) || (sw_S1.clicks > 0))
        {
          digitalWrite(PIN_LED2, LOW);
          digitalWrite(PIN_LED1, LOW);
          if ((switch2 == 1) || (sw_S1.clicks == 2))
          {
            openSX70.BlinkTimerDelay (GREEN, RED, 10);
          }
          sw_S1.Reset();
          openSX70.AutoExposure(myISO);
          return;
        };
      };

    };

  };

};


void BlinkISO () {
  if  ((nowDongle != 0) && (prevDongle == 0)) //read the default ISO and blink once for SX70 and twice for 600.
  {
    myISO = ReadISO();
    if (myISO == ISO_600)
      myDongle.simpleBlink(2, 6);
    else if (myISO == ISO_SX70)
      myDongle.simpleBlink( 1, 6);
#if SIMPLEDEBUG
    Serial.print ("EEPROM READ ISO:   ");
    Serial.println (myISO);
#endif
    prevDongle = nowDongle;
    return;

  }
}
