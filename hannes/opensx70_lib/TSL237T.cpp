#include "open_sx70.h"
#if defined (TSL237T)
  volatile bool integrationFinished = 0;
  
  //uint16_t outputCompare = A100;
  uint16_t outputCompare ;
  
  const uint8_t PIN_OE = 9;
  
  void meter_init() {
    tsl237_init();
  }
  void test_init() {
    pinMode(PIN_OE, OUTPUT);
    digitalWrite(PIN_OE, LOW);
  
    cli();
    // Clear all interrupts flags
    TIFR1 = (1 << ICF1) | (1 << OCF1B) | (1 << OCF1A) | (1 << TOV1);
    // Set timer 1 for normal operation, clocked by rising edge on T1 (port D5 / pin 5)
    TCCR1A = 0;
    TCCR1B = (1 << CS10) | (1 << CS11) | (1 << CS12);
    TCCR1C = 0;
    // set current value to 0 (needs to be done, maybe, on each new conversion)
    //TCNT1 = 0;
    sei();                    // Restart interrupts
  }
  // I like to call them magic number
  
  void meter_set_iso(const uint16_t& iso) {
    if (iso == ISO_600) {
      outputCompare = A600;
    } else if (iso == ISO_SX70) {
      outputCompare = A100;
    } else if (iso == ISO_600BW){
      outputCompare = A400;
    }
  }
  
  int meter_compute(unsigned int _interval) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  {
    int _myISO = ReadISO(); //Read ISO from EEPROM
    //int _myISO = activeISO;
    #if SIMPLEDEBUG
      //Serial.print("Meter Compute: Uses this ISO for metering: ");
      //Serial.println(_myISO);
    #endif
    static unsigned long previousMillis = 0;
    static bool measuring = false;
    //long interval = 200;
    //unsigned long counter; defined elsewhere
  
    //unsigned long PredExp;
    float PredExp;
  
    //  unsigned long currentMillis = millis();
    //  unsigned long timeMillis;
    meter_set_iso(_myISO);
  
    if (!measuring)
    {
      meter_init();
      previousMillis = millis();
      measuring = true;
    }
    else
    {
      unsigned long myMillis = millis() - previousMillis;
      if (myMillis  >= _interval )
      {
        unsigned long counter = TCNT1;
        measuring = false;
        PredExp = (((float)myMillis) / ((float) counter)) * (float)outputCompare;
        //Serial.print ("pr mil: ");
        //Serial.println (previousMillis);
        //Serial.print ("mil: ");
        //Serial.println (myMillis);
        //Serial.print ("_interval: ");
        //Serial.println (_interval);
        //Serial.print ("counter: ");
        //Serial.println (counter);
        //Serial.print ("output compare: ");
        //Serial.println (outputCompare);
        //Serial.print("                      PredExp: ");
        //Serial.println(PredExp);
        PredExp = PredExp + ShutterConstant;
        /*
        if (PredExp > 1100)
        {
          //    PredExp = 333;
          return 1100;
        }
        else if (PredExp < 18)
        {
          //    PredExp = 16;
          return 16;
        }*/
        return PredExp;
      }
    }
    return -1;
  }

  int meter_compute(unsigned int _interval,int _activeISO) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  {
    //int _myISO = ReadISO(); //Read ISO from EEPROM
    int _myISO = _activeISO;
    #if SIMPLEDEBUG
      //Serial.print("Meter Compute: Uses this ISO for metering: ");
      //Serial.println(_myISO);
    #endif
    static unsigned long previousMillis = 0;
    static bool measuring = false;
    //long interval = 200;
    //unsigned long counter; defined elsewhere
  
    //unsigned long PredExp;
    float PredExp;
  
    //  unsigned long currentMillis = millis();
    //  unsigned long timeMillis;
    meter_set_iso(_myISO);
  
    if (!measuring)
    {
      meter_init();
      previousMillis = millis();
      measuring = true;
    }
    else
    {
      unsigned long myMillis = millis() - previousMillis;
      if (myMillis  >= _interval )
      {
        unsigned long counter = TCNT1;
        measuring = false;
        PredExp = (((float)myMillis) / ((float) counter)) * (float)outputCompare;
        //Serial.print ("pr mil: ");
        //Serial.println (previousMillis);
        //Serial.print ("mil: ");
        //Serial.println (myMillis);
        //Serial.print ("_interval: ");
        //Serial.println (_interval);
        //Serial.print ("counter: ");
        //Serial.println (counter);
        //Serial.print ("output compare: ");
        //Serial.println (outputCompare);
        //Serial.print("                      PredExp: ");
        //Serial.println(PredExp);
        PredExp = PredExp + ShutterConstant;
        /*
        if (PredExp > 1100)
        {
          //    PredExp = 333;
          return 1100;
        }
        else if (PredExp < 18)
        {
          //    PredExp = 16;
          return 16;
        }*/
        return PredExp;
      }
    }
    return -1;
  }
  
  void meter_integrate() {
    tsl237_start_integration();
  }
  
  bool meter_update() {
    if (integrationFinished) {
      integrationFinished = 0;
      return 1;
    }
    return 0;
  }
  
  // initialise Timer 1 for light sensor integration.
  void tsl237_init() {
    pinMode(PIN_OE, OUTPUT);
    digitalWrite(PIN_OE, LOW);
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
  void tsl237_start_integration() {
    cli(); //Hold Interrupt
    // Clear interrupts flags we are using
    //	TIFR1 = (1 << OCF1A) | (1 << TOV1);
    TIFR1 = (1 << OCF1A);
    // set compare value given sensivity - active selected ISO
    OCR1A = outputCompare;
    // clear counter value.
    TCNT1 = 0;
    // Set interrupt vectors for compare match A.
    //	TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);
    TIMSK1 = (1 << OCIE1A);
    sei(); //Start Interupt
  }
  
  // ISR for complete conversion. Should set a flag read by the main loop.
  ISR(TIMER1_COMPA_vect) {
    TIMSK1 = 0;
    integrationFinished = 1;
    // ExposureFinish()();
    // function / flag.
  }
  
  nearest(int x, int myArray[], int elements, bool sorted)
  // int nearest(int x, int myArray[], int elements, bool sorted)
  {
    int idx = 0; // by default near first element
    int distance = abs(myArray[idx] - x);
    for (int i = 1; i < elements; i++)
    {
      int d = abs(myArray[i] - x);
      if (d < distance)
      {
        idx = i;
        distance = d;
      }
      else if (sorted) return idx;
    }
    return idx;
  }
  
  void meter_led(byte _selector, byte _type)
  {
    int PredictedExposure;
    //original int PredictedExposure = meter_compute(200);
    if((ShutterSpeed[_selector]) == AUTO600)
    {
      PredictedExposure = meter_compute(200,ISO_600);
    }
    else if((ShutterSpeed[_selector]) == AUTO600BW)
    {
      PredictedExposure = meter_compute(200,ISO_600BW);
    }else if((ShutterSpeed[_selector] == AUTO100))
    {
      PredictedExposure = meter_compute(200,ISO_SX70);
    }

    if (PredictedExposure == -1)
    {
      return;
    }
    if (_type == 1) //Manual Mode
    {
      int slot = nearest(PredictedExposure, ShutterSpeed, 11, false);
  /*
      Serial.print ("PredictedExposure: ");
      Serial.println (PredictedExposure);
  
      Serial.print ("Estimated SLOT: ");
      Serial.println (slot);
      Serial.print ("Actual SLOT: ");
      Serial.println (_selector);
  */
        if (_selector < slot)
        {
          //Serial.println ("_selector < slot");
          digitalWrite(PIN_LED1, HIGH);
          digitalWrite(PIN_LED2, LOW);
          return;
        }
        else if (_selector > slot)
        {
          //Serial.println ("_selector > slot");
          digitalWrite(PIN_LED1, LOW);
          digitalWrite(PIN_LED2, HIGH);
          return;
        }
        else if (_selector == slot)
        {
          //Serial.println ("_selector == slot");
          digitalWrite(PIN_LED1, LOW);
          digitalWrite(PIN_LED2, LOW);
          return;
        }
    }
    else if (_type == 0) //Automode
    {
      #if ADVANCEDEBUG
      Serial.print ("Exposure Helper on Auto Mode ");
      Serial.print (", PredictedExposure: ");
      Serial.println (PredictedExposure);
      #endif
      if (PredictedExposure > 100)
      {
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, HIGH);
        //Serial.println ("LOW LIGHT");
        return;
      } else
      {
        digitalWrite(PIN_LED1, LOW);
        digitalWrite(PIN_LED2, LOW);
        return;
      }
    }
  }
#endif
