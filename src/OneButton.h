// -----
// OneButton.h - Library for detecting button clicks, doubleclicks and long press pattern on a single button.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by robin, http://www.intoyun.com
// -----
// 5.11.2017 created by Robin
// -----

#ifndef __ONEBUTTON_H__
#define __ONEBUTTON_H__

#include "Arduino.h"

// ----- Callback function types -----
extern "C" {
    typedef void (*callbackClickFunc)(void);
    typedef void (*callbackPressFunc)(uint32_t ms);
}

class OneButton
{
    public:
        // ----- Constructor -----
        OneButton(uint8_t pin, bool invert);

        // ----- Set runtime parameters -----
        // set # millisec after safe click is assumed.
        void setDebounceTime(uint32_t ms);
        // set # millisec after single click is assumed.
        void setClickTime(uint32_t ms);
        // set # millisec after press is assumed.
        void setPressTime(uint32_t ms);;

        // attach functions that will be called when button was pressed in the specified way.
        void attachClick(callbackClickFunc cbFunc);
        void attachDoubleClick(callbackClickFunc cbFunc);
        void attachLongPressStart(callbackPressFunc cbFunc);
        void attachLongPressStop(callbackPressFunc cbFunc);
        void attachDuringLongPress(callbackPressFunc cbFunc);

        // ----- State machine functions -----
        // call this function every some milliseconds for handling button events.
        bool isLongPressed();
        void loop(void);

    private:
        uint8_t _pin;          // hardware pin number.
        uint32_t _debounceTime; // number of ticks for debounce times.
        uint32_t _clickTime;    // number of ticks that have to pass by before a click is detected
        uint32_t _pressTime;    // number of ticks that have to pass by before a long button press is detected

        uint8_t _buttonReleased;
        uint8_t _buttonPressed;

        bool _isLongPressed;

        // These variables will hold functions acting as event source.
        callbackClickFunc _clickFunc;
        callbackClickFunc _doubleClickFunc;
        callbackPressFunc _longPressStartFunc;
        callbackPressFunc _longPressStopFunc;
        callbackPressFunc _duringLongPressFunc;

        // These variables that hold information across the upcoming tick calls.
        // They are initialized once on program start and are updated every time the tick function is called.
        uint8_t _state;
        system_tick_t _startTime; // will be set in state 1
        system_tick_t _stopTime; // will be set in state 2
};

#endif

