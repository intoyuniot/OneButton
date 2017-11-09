/*
   This is a sample sketch to show how to use the OneButtonLibrary
   to detect click events on 2 buttons in parallel.
 */

#include "OneButton.h"

// Setup a new OneButton on pin D1.
OneButton button1(D1, false);
// Setup a new OneButton on pin D2.
OneButton button2(D2, false);

// setup code here, to run once:
void setup() {
    // Setup the Serial port. see http://arduino.cc/en/Serial/IfSerial
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    Serial.println("Starting TwoButtons...");

    // link the button 1 functions.
    button1.attachClick(click1);
    button1.attachDoubleClick(doubleclick1);
    button1.attachLongPressStart(longPressStart1);
    button1.attachLongPressStop(longPressStop1);
    button1.attachDuringLongPress(longPress1);

    // link the button 2 functions.
    button2.attachClick(click2);
    button2.attachDoubleClick(doubleclick2);
    button2.attachLongPressStart(longPressStart2);
    button2.attachLongPressStop(longPressStop2);
    button2.attachDuringLongPress(longPress2);
}

// main code here, to run repeatedly:
void loop() {
    // keep watching the push buttons:
    button1.loop();
    button2.loop();
}

// ----- button 1 callback functions
// This function will be called when the button1 was pressed 1 time (and no 2. button press followed).
void click1() {
    Serial.println("Button 1 click.");
}

// This function will be called when the button1 was pressed 2 times in a short timeframe.
void doubleclick1() {
    Serial.println("Button 1 doubleclick.");
}

// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1() {
    Serial.println("Button 1 longPress start");
}

// This function will be called often, while the button1 is pressed for a long time.
void longPress1() {
    Serial.println("Button 1 longPress...");
}

// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1() {
    Serial.println("Button 1 longPress stop");
}

// ... and the same for button 2:
void click2() {
    Serial.println("Button 2 click.");
}

void doubleclick2() {
    Serial.println("Button 2 doubleclick.");
}

void longPressStart2() {
    Serial.println("Button 2 longPress start");
}

void longPress2() {
    Serial.println("Button 2 longPress...");
}

void longPressStop2() {
    Serial.println("Button 2 longPress stop");
}

