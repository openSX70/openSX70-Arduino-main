#include "ClickButton.h"


ClickButton::ClickButton(uint8_t buttonPin, bool activeType)
{
    _pin           = buttonPin;
    _activeHigh    = activeType;
    _btnState      = !_activeHigh;  // initial button state in active-high logic
    _lastState     = _btnState;
    _clickCount    = 0;
    clicks         = 0;
    depressed      = 0;
    _lastBounceTime= 0;
    debounceTime   = 50;            // Debounce timer in ms
    multiclickTime = 250;           // Time limit for multi clicks
    longClickTime  = 1000;          // time until long clicks register
    changed        = false;
    pinMode(_pin, INPUT);
}


void ClickButton::Reset()
{
    _btnState      = !_activeHigh;  // initial button state in active-high logic
    _lastState     = _btnState;
    _clickCount    = 0;
    clicks         = 0;
    depressed      = 0;
    _lastBounceTime= 0;
    changed        = false;

}

void ClickButton::Update()
{
    long now = (long)millis();      // get current time
    _btnState = digitalRead(_pin);  // current appearant button state
    // Make the button logic active-high in code
    if (!_activeHigh) _btnState = !_btnState;
    // If the switch changed, due to noise or a button press, reset the debounce timer
    if (_btnState != _lastState) 
    {
        _lastBounceTime = now;
    }

    // debounce the button (Check if a stable, changed state has occured)
    if (now - _lastBounceTime > debounceTime && _btnState != depressed)
    {
        depressed = _btnState;
        if (depressed) _clickCount++;
    }

    if(_lastState == _btnState) {
        changed = false;
    }
    _lastState = _btnState;
    
    // Check for "long click"
    if (depressed && (now - _lastBounceTime > longClickTime))
    {
        clicks = 0 - _clickCount;
        //DEBUG_OUTPUT.print("Clickcount = ");
        //DEBUG_OUTPUT.println(clicks); 
        _clickCount = 0;
        if(clicks != 0){
            changed = true;
        }
    }
}
