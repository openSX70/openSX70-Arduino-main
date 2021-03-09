/*
 * openSX70 : opensource project to take the SX70 instant camera beyond its original functionnalities
 * This file is part of openSX70 project.
 *
 *
 * This project is licensed under the terms of CreactiveCommons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
 *
 * You are free to :
 * Share — copy and redistribute the material in any medium or format
 *
 * Adapt — remix, transform, and build upon the material 
 * The licensor cannot revoke these freedoms as long as you follow the license terms.
 *
 *
 * Under the following terms :
 * Attribution — You must give appropriate credit, provide a link to the license,
 * and indicate if changes were made.
 * You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use. 
 *
 * NonCommercial — You may not use the material for commercial purposes.
 *
 * No additional restrictions — You may not apply legal terms or technological measures that legally restrict
 * others from doing anything the license permits.
 * 
 *
 * Notices:
 * You do not have to comply with the license for elements of the material in the public domain or
 * where your use is permitted by an applicable exception or limitation.
 *
 * No warranties are given. The license may not give you all of the permissions necessary for your intended use.
 * For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.
 *
 * Complete license terms are available at : https://creativecommons.org/licenses/by-nc/4.0/legalcode
 */

#include "open_sx70.h"
#if TSL235R
volatile bool integrationFinished = 0;

bool measuring = false;
uint32_t startMillis;
uint32_t endMillis;

uint16_t outputCompare = A100;

const uint8_t PIN_OE = 9;

void meter_init(){
  tsl235_init();
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

void meter_integrate(){
	tsl235_start_integration();
}

bool meter_update(){
	if(integrationFinished){
		integrationFinished = 0;
		return 1;
	}
	return 0;
}

void lmTimer_stop(){ //Stop the Timer1
    #if ADVANCEDEBUG
      Serial.println("Stop Timer");
    #endif
    //cli();  // One way to disable the timer, and all interrupts
    TCCR1B &= ~(1<< CS12);  // turn off the clock altogether
    TCCR1B &= ~(1<< CS11);
    TCCR1B &= ~(1<< CS10);
    //TIMSK1 &= ~(1 << OCIE1A); // turn off the timer interrupt
}



// initialise Timer 1 for light sensor integration.
void tsl235_init(){
//	pinMode(PIN_OE, OUTPUT);
//	digitalWrite(PIN_OE, LOW);

	cli();
	// Clear all interrupts flags
	TIFR1 = (1 << ICF1) | (1 << OCF1B) | (1 << OCF1A) | (1 << TOV1);
	// Set timer 1 for normal operation, clocked by rising edge on T1 (port D5 / pin 5)
	TCCR1A = 0;
	TCCR1B = (1 << CS10) | (1 << CS11) | (1 << CS12);
	TCCR1C = 0;
	// set current value to 0 (needs to be done, maybe, on each new conversion)
	TCNT1 = 0;
	sei();										// Restart interrupts
}

// Start a new measure for pose.
void tsl235_start_integration(){
	cli();
	// Clear interrupts flags we are using
//	TIFR1 = (1 << OCF1A) | (1 << TOV1);
	TIFR1 = (1 << OCF1A);
	// set compare value given sensivity
	OCR1A = outputCompare;
	// clear counter value.
	TCNT1 = 0;
	// Set interrupt vectors for compare match A.
//	TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);
	TIMSK1 = (1 << OCIE1A);

	sei();
}

// ISR for complete conversion. Should set a flag read by the main loop.
ISR(TIMER1_COMPA_vect){
	TIMSK1 = 0;
	integrationFinished = 1;

	// function / flag.
}

int meter_compute(byte _selector,int _activeISO){
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

        /*
        #if LMDEBUG
          Serial.print("PREDMILLI: ");
          Serial.println(pred_milli);
        #endif
        */

        // pred_milli is how many ms the meter will take to reach the set 
        // magic number. Every scene should generally have a linear increase
        // of the counter, therefore we can use basic algebra to extrapolate
        // when the counter will hit the magic number.
        return pred_milli; 
      }
    }
    return -1;
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
