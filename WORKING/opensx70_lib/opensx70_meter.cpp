#include "open_sx70.h"
#include "logging.h"

volatile bool integrationFinished = 0;

bool measuring = false;
uint32_t startMillis;
uint32_t endMillis;
uint32_t timeElapsed;

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
  int _myISO = _activeISO;
  uint16_t adcValue;
  if(measuring == false){
    measuring = true;
    meter_reset();
    startMillis = millis();
  }
  else{
    endMillis = millis();
    timeElapsed =  endMillis - startMillis;
    if((timeElapsed) >= METER_INTERVAL){
      adcValue = analogRead(PIN_LM);
      measuring = false;

      float slope = (float(adcValue)/float(timeElapsed));
      int pred_milli; 
      if(slope == 0){
        pred_milli = 9999;
      }
      else{
        pred_milli = round(float(outputCompare)/float(slope)); 
      }
      return pred_milli; 
    }
  }
  return -1;   
}

bool meter_update(){
  uint16_t adcValue = analogRead(PIN_LM);
  if( adcValue >= outputCompare){
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

  else if(_type == 1){ // Automode
    if(predictedMillis >= ShutterSpeed[7]){ //Low light warning
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
  }

}

