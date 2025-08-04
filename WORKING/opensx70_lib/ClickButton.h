#ifndef ClickButton_H
  #define ClickButton_H
  
  #if (ARDUINO <  100)
    #include <WProgram.h>
  #else
    #include <Arduino.h>
  #endif
  
  #define CLICKBTN_PULLUP HIGH

  extern HardwareSerial DEBUG_OUTPUT;
  
  class ClickButton
  {
    public:
      ClickButton(uint8_t buttonPin);
      ClickButton(uint8_t buttonPin, bool active);
      ClickButton(uint8_t buttonPin, bool active, bool internalPullup);
      void Update();
      void Reset();
      int clicks;                   // button click counts to return
      bool depressed;            // the currently debounced button (press) state (presumably it is not sad :)
      long debounceTime;
      long multiclickTime;
      long longClickTime;
      bool changed;
    private:
      uint8_t _pin;                 // Arduino pin connected to the button
      bool _activeHigh;          // Type of button: Active-low = 0 or active-high = 1
      bool _btnState;            // Current appearant button state
      bool _lastState;           // previous button reading
      int _clickCount;              // Number of button clicks within multiclickTime milliseconds
      long _lastBounceTime;         // the last time the button input pin was toggled, due to noise or a press
  };

#endif
