#include "open_sx70.h"
#include "logging.h"

volatile bool integrationFinished = 0;

bool measuring = false;
uint32_t startMillis;
uint32_t endMillis;

uint16_t outputCompare = A100;

void meter_init(){
  integrator_init();
}

void meter_reset(){
  integrator_reset();
}

void integrator_init(){
  pinMode(PIN_LM, INPUT);
  pinMode(PIN_LM_RST, OUTPUT);
  digitalWrite(PIN_LM_RST, LOW);
}

void integrator_reset(){
  //TODO, See how much time a cap reset takes and add a delay if required.
  digitalWrite(PIN_LM_RST, HIGH);
  delay(METER_RESET_DELAY);
  digitalWrite(PIN_LM_RST, LOW);
}

void meter_set_iso(const uint16_t& iso){ //set the output Compare Value for Timer1 -- Magicnumber for ISO
    #if LMDEBUG
    output_serial(F("meter_set_iso : "));
    output_line_serial(iso);
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
    output_serial(F("outputcompare set to : "));
    output_line_serial(outputCompare);
    #endif
}

int meter_compute(byte _selector,int _activeISO){
  // TODO If possible, change this to use an integrated timer and interrupt rather than checking time manually.
  // Doing this on a new microcontroller would probably be more worthwhile time wise.
  int _myISO = _activeISO;
  uint16_t counter;
  if(measuring == false){
    //meter_set_iso(_activeISO);
    measuring = true;
    meter_reset();
    startMillis = millis();

    #if LMHELPERDEBUG
      output_serial("Metering started at: ");
      output_serial(startMillis);
      output_line_serial(" ms");
    #endif
  }
  else{
    endMillis = millis();
    uint32_t timeElapsed =  endMillis - startMillis;
    if((timeElapsed) >= METER_INTERVAL){
      counter = analogRead(PIN_LM);
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
        output_serial("Metering ended at ");
        output_serial(endMillis);
        output_line_serial(" ms");

        output_serial("Metering time elapsed: ");
        output_serial(timeElapsed);
        output_line_serial(" ms");

        output_serial("Magic number used: ");
        output_line_serial(outputCompare);

        output_serial("Meter counter ended at ");
        output_line_serial(counter);

        output_serial("Magic number hit at ");
        output_serial(pred_milli);
        output_line_serial(" ms");

        output_serial("slope: ");
        output_line_serial(slope);
      #endif

      
      //#if LMDEBUG
        //output_serial("PREDMILLI: ");
        //output_line_serial(pred_milli);
      //#endif
      

      // pred_milli is how many ms the meter will take to reach the set 
      // magic number. Every scene should generally have a linear increase
      // of the counter, therefore we can use basic algebra to extrapolate
      // when the counter will hit the magic number.
      return pred_milli; 
    }
  }
  return -1;   
}

void meter_integrate(){
  //all we need to do when starting a new metering session is reset the capacitor
  integrator_reset();
}

bool meter_update(){
  uint16_t integration_value = analogRead(PIN_LM);
  #if LMDEBUG
      output_serial(F("Integrated counter value : "));
      output_line_serial(integration_value);
  #endif
  if( integration_value >= outputCompare){
      #if LMDEBUG
          output_serial(F("Final integrated counter value : "));
          output_line_serial(integration_value);
      #endif
      return 1;
  }
  else{
      return 0;
  }
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

  int meterDifference = abs(predictedMillis - ShutterSpeed[_selector]);


  #if LMHELPERDEBUG
    output_serial(F("meter range at Selector: "));
    output_serial(_selector);
    output_serial(F(" "));
    output_serial(ShutterSpeed[_selector]);
    output_serial(F(" min: "));
    output_line_serial(MinRange[_selector]);
    output_serial(F(" max "));
    output_line_serial(MaxRange[_selector]);
    output_serial(F("Predictedmillis: "));
    
    output_line_serial(predictedMillis);
  #endif
  /*
  if(_type ==2){ // Manual mode
    predictedMillis = predictedMillis + METER_PREDICTION_OFFSET;
    // Within range
    if((predictedMillis <= (ShutterSpeed[_selector] + meterRange)) && (predictedMillis >= (ShutterSpeed[_selector] - meterRange))){
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, HIGH);
      #if LMHELPERDEBUG
        output_line_serial(F("Selector within meter range"));
      #endif
      return;
    }
    // Lower speed required
    else if((predictedMillis < (ShutterSpeed[_selector] - meterRange))){
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, HIGH);
      #if LMHELPERDEBUG
        output_line_serial(F("Selector under meter range"));
      #endif
      return;
    }
    // Higher speed needed
    else{
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, LOW);
      #if LMHELPERDEBUG
        output_line_serial(F("Selector over meter range"));
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
        output_line_serial(F("Selector within meter range"));
      #endif
      return;
    }
    // Lower speed required
    else if(predictedMillis < MinRange[_selector]){
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, HIGH);
      #if LMHELPERDEBUG
        output_line_serial(F("Selector under meter range"));
      #endif
      return;
    }
    // Higher speed needed
    else{
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, LOW);
      #if LMHELPERDEBUG
        output_line_serial(F("Selector over meter range"));
      #endif
      return;
    }
  }
  else if(_type == 1){ // Automode
    if(predictedMillis >= METER_AUTO_WARNING){ //Low light warning
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, LOW);
      #if LMHELPERDEBUG
        output_line_serial(F("Auto mode low light warning"));
      #endif
    }
    else{ //Low light warning off
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, LOW);
      #if LMHELPERDEBUG
        output_line_serial(F("Enough Light Detected"));
      #endif
    }
    /*
    else{
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, HIGH);
      #if LMHELPERDEBUG
        output_line_serial("Auto mode Enough light");
      #endif
    }
    */
  }

}

