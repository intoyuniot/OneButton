// -----
// OneButton.cpp - Library for detecting button clicks, doubleclicks and long press pattern on a single button.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Robin, http://www.intoyun.com
// -----
// Changelog: see OneButton.h
// -----

#include "OneButton.h"

// ----- Initialization and Default Values -----
OneButton::OneButton(uint8_t pin, bool invert)
{
    _pin = pin;

    _debounceTime = 50;      // number of millisec that have to pass by before a click is assumed as safe.
    _clickTime = 600;        // number of millisec that have to pass by before a click is detected.
    _pressTime = 1000;       // number of millisec that have to pass by before a long button press is detected.

    _state = 0;              // starting with state 0: waiting for button to be pressed
    _isLongPressed = false;  // Keep track of long press state

    if (!invert) {
        // the button connects the input pin to GND when pressed.
        _buttonReleased = HIGH; // notPressed
        _buttonPressed = LOW;
        // use the given pin as input and activate internal PULLUP resistor.
        pinMode( pin, INPUT_PULLUP );
    } else {
        // the button connects the input pin to VCC when pressed.
        _buttonReleased = LOW;
        _buttonPressed = HIGH;
        // use the given pin as input
        pinMode( pin, INPUT );
    }

    // no functions attached yet: clear all function pointers.
    _clickFunc = NULL;
    _doubleClickFunc = NULL;
    _longPressStartFunc = NULL;
    _longPressStopFunc = NULL;
    _duringLongPressFunc = NULL;
}

// explicitly set the number of millisec that have to pass by before a click is assumed as safe.
void OneButton::setDebounceTime(uint32_t ms) {
    _debounceTime = ms;
}

// explicitly set the number of millisec that have to pass by before a click is detected.
void OneButton::setClickTime(uint32_t ms) {
    _clickTime = ms;
}

// explicitly set the number of millisec that have to pass by before a long button press is detected.
void OneButton::setPressTime(uint32_t ms) {
    _pressTime = ms;
}

// save function for click event
void OneButton::attachClick(callbackClickFunc cbFunc)
{
    _clickFunc = cbFunc;
}

// save function for doubleClick event
void OneButton::attachDoubleClick(callbackClickFunc cbFunc)
{
    _doubleClickFunc = cbFunc;
}

// save function for press event
// save function for longPressStart event
void OneButton::attachLongPressStart(callbackPressFunc cbFunc)
{
    _longPressStartFunc = cbFunc;
}

// save function for longPressStop event
void OneButton::attachLongPressStop(callbackPressFunc cbFunc)
{
    _longPressStopFunc = cbFunc;
}

// save function for during longPress event
void OneButton::attachDuringLongPress(callbackPressFunc cbFunc)
{
    _duringLongPressFunc = cbFunc;
}

// function to get the current long pressed state
bool OneButton::isLongPressed(){
    return _isLongPressed;
}

void OneButton::loop(void)
{
    // Detect the input information
    int buttonLevel = digitalRead(_pin); // current button signal.
    system_tick_t now = millis(); // current (relative) time in msecs.

    // Implementation of the state machine
    if (_state == 0) {  // waiting for menu pin being pressed.
        if (buttonLevel == _buttonPressed) {
            _state = 1; // step to state 1
            _startTime = now; // remember starting time
        }
    } else if (_state == 1) { // waiting for menu pin being released.
        if ((buttonLevel == _buttonReleased) && ((system_tick_t)(now - _startTime) < _debounceTime)) {
            // button was released to quickly so I assume some debouncing.
            // go back to state 0 without calling a function.
            _state = 0;
        } else if (buttonLevel == _buttonReleased) {
            _state = 2; // step to state 2
            _stopTime = now; // remember stopping time
        } else if ((buttonLevel == _buttonPressed) && ((system_tick_t)(now - _startTime) > _pressTime)) {
            _isLongPressed = true;  // Keep track of long press state
            if (_longPressStartFunc) _longPressStartFunc(0);
            if (_duringLongPressFunc) _duringLongPressFunc(now - _startTime);
            _state = 6; // step to state 6
        } else {
            // wait. Stay in this state.
        }
    } else if (_state == 2) { // waiting for menu pin being pressed the second time or timeout.
        if (_doubleClickFunc == NULL || (system_tick_t)(now - _startTime) > _clickTime) {
            // this was only a single short click
            if (_clickFunc) _clickFunc();
            _state = 0; // restart.
        } else if ((buttonLevel == _buttonPressed) && ((system_tick_t)(now - _stopTime) > _debounceTime)) {
            _state = 3; // step to state 3
            _startTime = now; // remember starting time
        }
    } else if (_state == 3) { // waiting for menu pin being released finally.
        // Stay here for at least _debounceTime because else we might end up in state 1 if the
        // button bounces for too long.
        if (buttonLevel == _buttonReleased && ((system_tick_t)(now - _startTime) > _debounceTime)) {
            // this was a 2 click sequence.
            if (_doubleClickFunc) _doubleClickFunc();
            _state = 0; // restart.
        }
    } else if (_state == 6) { // waiting for menu pin being release after long press.
        if (buttonLevel == _buttonReleased) {
            _isLongPressed = false;  // Keep track of long press state
            if(_longPressStopFunc) _longPressStopFunc(now - _startTime);
            _state = 0; // restart.
        } else {
            // button is being long pressed
            _isLongPressed = true; // Keep track of long press state
            if (_duringLongPressFunc) _duringLongPressFunc(now - _startTime);
        }
    }
}

