#include "open_sx70.h"
#include "logging.h"

volatile bool integrationFinished = 0;

bool measuring = false;
uint32_t startMillis;
uint32_t endMillis;

uint16_t outputCompare = A100;

void meter_init(){
  pinMode(PIN_LM, INPUT);
  pinMode(PIN_LM_RST, OUTPUT);
  digitalWrite(PIN_LM_RST, LOW);
}

void meter_reset(){
  digitalWrite(PIN_LM_RST, HIGH);
  delay(METER_RESET_DELAY);
  digitalWrite(PIN_LM_RST, LOW);
}

void meter_set_iso(const uint16_t& iso){ //set the output Compare Value for Timer1 -- Magicnumber for ISO
    if (iso == ISO_600) {
      outputCompare = A600;
    } 
    else if (iso == ISO_SX70) {
      outputCompare = A100;
    }
    else{
      outputCompare = iso; //FF Delay Magicnumber as well as      
    }
}

int meter_compute(byte _selector,int _activeISO){
  int _myISO = _activeISO;
  bool _sampleTaken = false;
  uint16_t counter;

  if(measuring == false){
    //meter_set_iso(_activeISO);
    measuring = true;
    meter_reset();
    startMillis = millis();

    //initial blocking sample for high EVs
    while(millis() <= (startMillis + METER_DURATION)){
      if(meter_update()){
        counter = analogRead(PIN_LM);
        endMillis = millis();
        _sampleTaken = true;
        break;
      }
    }
  }
  else{ 
    uint32_t timeElapsed =  endMillis - startMillis;
    if((timeElapsed) >= METER_INTERVAL){
      counter = analogRead(PIN_LM);
      endMillis = millis();
      _sampleTaken = true;
  }

  if(_sampleTaken){
    measuring = false;

    float slope = (float(counter)/float(timeElapsed)) + METER_SLOPE_HANDICAP;
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
  else{
    return -1; 
  }
}

bool meter_update(){
  uint16_t integration_value = analogRead(PIN_LM);
  if( integration_value >= outputCompare){
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

  if(_type ==2){ // Manual mode
    predictedMillis = predictedMillis + METER_PREDICTION_OFFSET;
    // Within range
    if((predictedMillis <= MaxRange[_selector])  && (predictedMillis >= MinRange[_selector])){
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, HIGH);
      return;
    }
    // Lower speed required
    else if(predictedMillis < MinRange[_selector]){
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, HIGH);
      return;
    }
    // Higher speed needed
    else{
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, LOW);
      return;
    }
  }
  else if(_type == 1){ // Automode
    if(predictedMillis >= METER_AUTO_WARNING){ //Low light warning
      digitalWrite(PIN_LED1, HIGH);
      digitalWrite(PIN_LED2, LOW);
    }
    else{ //Low light warning off
      digitalWrite(PIN_LED1, LOW);
      digitalWrite(PIN_LED2, LOW);
    }
  }

}

