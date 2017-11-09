/*
   This is a sample sketch to show how to use the OneButtonLibrary
   to detect double-click events on a button.
 */

#include "OneButton.h"

// Setup a new OneButton on pin D1.
OneButton button(D1, false);

// setup code here, to run once:
void setup() {
    // enable the standard led on pin D7.
    pinMode(D7, OUTPUT);      // sets the digital pin as output

    // link the doubleclick function to be called on a doubleclick event.
    button.attachDoubleClick(doubleclick);
}

// main code here, to run repeatedly:
void loop() {
    button.loop();
}

// this function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick() {
    static int m = LOW;
    // reverse the LED
    m = !m;
    digitalWrite(D7, m);
}
