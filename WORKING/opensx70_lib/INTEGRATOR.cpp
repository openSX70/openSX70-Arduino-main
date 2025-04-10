#include "open_sx70.h"
#if INTEGRATOR
  volatile bool integrationFinished = 0;

  bool measuring = false;
  uint32_t startMillis;
  uint32_t endMillis;
  
  uint16_t outputCompare = A100;

  void meter_init(){
    integrator_init();
  }

  void integrator_init(){
    pinMode(PIN_LM, INPUT);
    //pinMode(PIN_LM_RST, OUTPUT);
    //digitalWrite(PIN_LM_RST, LOW);
  }

  void integrator_reset(){
    //!TODO, See how much time a cap reset takes and add a delay if required.
    //digitalWrite(PIN_LM_RST, HIGH);
    //digitalWrite(PIN_LM_RST, LOW);
  }

  void meter_set_iso(const uint16_t& iso){ //set the output Compare Value for Timer1 -- Magicnumber for ISO
      #if LMDEBUG
      Serial.print(F("meter_set_iso : "));
      Serial.println(iso);
      #endif
      if (iso == ISO_600) {
        outputCompare = A600;
      } 
      else if (iso == ISO_SX70) {
        outputCompare = A100;
      }
      else{
        outputCompare = iso; //FF Delay Magicnumber as well as      
      }
      #if LMDEBUG
      Serial.print(F("outputcompare set to : "));
      Serial.println(outputCompare);
      #endif
  }
  
  int meter_compute(byte _selector,int _activeISO){
    /*
    !TODO! adjust this for new metering mode
    int _myISO = _activeISO;

    if(measuring == false){
      meter_set_iso(_activeISO);
      measuring = true;
      meter_init();
      startMillis = millis();

      #if LMHELPERDEBUG
        Serial.print("Metering started at: ");
        Serial.print(startMillis);
        Serial.println(" ms");
      #endif
    }
    else{
      endMillis = millis();
      uint32_t timeElapsed =  endMillis - startMillis;
      if((timeElapsed) >= METER_INTERVAL){
        uint32_t counter = TCNT1;
        measuring = false;

        float slope = (float(counter)/float(timeElapsed)) + METER_SLOPE_HANDICAP;
        int pred_milli; 
        if(slope == 0){
          pred_milli = 9999;
        }
        else{
          pred_milli = round(float(outputCompare)/float(slope)); 
        }
        
        #if LMHELPERDEBUG
          Serial.print("Metering ended at ");
          Serial.print(endMillis);
          Serial.println(" ms");

          Serial.print("Metering time elapsed: ");
          Serial.print(timeElapsed);
          Serial.println(" ms");

          Serial.print("Magic number used: ");
          Serial.println(outputCompare);

          Serial.print("Meter counter ended at ");
          Serial.println(counter);

          Serial.print("Magic number hit at ");
          Serial.print(pred_milli);
          Serial.println(" ms");

          Serial.print("slope: ");
          Serial.println(slope);
        #endif

        
        //#if LMDEBUG
          //Serial.print("PREDMILLI: ");
          //Serial.println(pred_milli);
        //#endif
        

        // pred_milli is how many ms the meter will take to reach the set 
        // magic number. Every scene should generally have a linear increase
        // of the counter, therefore we can use basic algebra to extrapolate
        // when the counter will hit the magic number.
        return pred_milli; 
      }
    }
    return -1;  
    */  

    // !TODO! THIS IS A PLACEHOLDER VALUE. REMOVE LATER.
    return 0;
  }

  void meter_integrate(){
    integrator_reset();
  }
  
  bool meter_update(){
    /*
    uint8_t integration_value = analogRead(PIN_LM);
    if( integration_value >= outputCompare){
        #if LMDEBUG
            Serial.print(F("Integrated counter value : "));
            Serial.println(integration_value);
        #endif
        return 1;
    }
    else{
        return 0;
    }
    */
    return 1;
  }

  void meter_led(byte _selector, byte _type){
    if(_type == 0){ //OFF
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, LOW);
      return;
    }

    int predictedMillis;
    int activeISO;

    if((ShutterSpeed[_selector]) == AUTO600){
      activeISO = ISO_600;
    }
    else if((ShutterSpeed[_selector] == AUTO100)){
      activeISO = ISO_SX70;
    }
    else{
      activeISO = ReadISO();
    }

    predictedMillis = meter_compute(_selector, activeISO);

    if(predictedMillis == -1){ // Still measuring!
      return;
    }

    //int meterRange = round(ShutterSpeed[_selector] * MeterRange[_selector]);
    int meterDifference = abs(predictedMillis - ShutterSpeed[_selector]);


    #if LMHELPERDEBUG
      Serial.print(F("meter range at Selector: "));
      Serial.print(_selector);
      Serial.print(F(" "));
      Serial.print(ShutterSpeed[_selector]);
      Serial.print(F(" min: "));
      Serial.println(MinRange[_selector]);
      Serial.print(F(" max "));
      Serial.println(MaxRange[_selector]);
      Serial.print(F("Predictedmillis: "));
      
      Serial.println(predictedMillis);
    #endif
    /*
    if(_type ==2){ // Manual mode
      predictedMillis = predictedMillis + METER_PREDICTION_OFFSET;
      // Within range
      if((predictedMillis <= (ShutterSpeed[_selector] + meterRange)) && (predictedMillis >= (ShutterSpeed[_selector] - meterRange))){
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, HIGH);
        #if LMHELPERDEBUG
          Serial.println(F("Selector within meter range"));
        #endif
        return;
      }
      // Lower speed required
      else if((predictedMillis < (ShutterSpeed[_selector] - meterRange))){
        digitalWrite(PIN_LED1, LOW);
        digitalWrite(PIN_LED2, HIGH);
        #if LMHELPERDEBUG
          Serial.println(F("Selector under meter range"));
        #endif
        return;
      }
      // Higher speed needed
      else{
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, LOW);
        #if LMHELPERDEBUG
          Serial.println(F("Selector over meter range"));
        #endif
        return;
      }
    }
    */
    if(_type ==2){ // Manual mode
      predictedMillis = predictedMillis + METER_PREDICTION_OFFSET;
      // Within range
      if((predictedMillis <= MaxRange[_selector])  && (predictedMillis >= MinRange[_selector])){
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, HIGH);
        #if LMHELPERDEBUG
          Serial.println(F("Selector within meter range"));
        #endif
        return;
      }
      // Lower speed required
      else if(predictedMillis < MinRange[_selector]){
        digitalWrite(PIN_LED1, LOW);
        digitalWrite(PIN_LED2, HIGH);
        #if LMHELPERDEBUG
          Serial.println(F("Selector under meter range"));
        #endif
        return;
      }
      // Higher speed needed
      else{
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, LOW);
        #if LMHELPERDEBUG
          Serial.println(F("Selector over meter range"));
        #endif
        return;
      }
    }
    else if(_type == 1){ // Automode
      if(predictedMillis >= METER_AUTO_WARNING){ //Low light warning
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, LOW);
        #if LMHELPERDEBUG
          Serial.println(F("Auto mode low light warning"));
        #endif
      }
      else{ //Low light warning off
        digitalWrite(PIN_LED1, LOW);
        digitalWrite(PIN_LED2, LOW);
        #if LMHELPERDEBUG
          Serial.println(F("Enough Light Detected"));
        #endif
      }
      /*
      else{
        digitalWrite(PIN_LED1, LOW);
        digitalWrite(PIN_LED2, HIGH);
        #if LMHELPERDEBUG
          Serial.println("Auto mode Enough light");
        #endif
      }
      */
    }

  }
#endif
