//============================================================
// AVRac Demonstration.
//============================================================
// The Analog Comparator's reference voltage is 1.1V taken 
// from the internally generated bandgap reference.
//
// Attach a potentiometer (value can be almost anything) to
// VCC and GND, the wiper connects to AIN1/D7.
//
// Attach an LED to D13 and toggle it in the loop.
//
// Another LED on D8 will be toggled according to whether 
// the potentiometer voltage is higher or lower than 1.1V.
//
// See docs/AVR_ac_INTERNAL_test.png for an image of the
// circuit on a breadboard.
//
// Norman Dunbar
// 27th October 2020.
//============================================================

#include "AVR_ac.h"


// The function to call when the AC interrupt triggers.
void toggleLED() {
    // Read the ACO bit and if on, light the LED on PB0
    // otherwise, extinguish it.
    digitalWrite(8,!!(ACSR & (1 << ACO)));
}


void setup() {
    // D8 and D13 are outputs. Turn both LEDs off.
    pinMode(8, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(8, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    
    // Configure the Analog Comparator. Do this before
    // calling begin().
    AVRac.onInterruptTriggered(toggleLED);
    
    AVRac.begin(ac::TRGR_CHANGE,
                ac::VREF_INTERNAL,
                ac::VTEST_D7_AIN1,
                ac::INPUTCAPT_OFF);

}

void loop() {
    // Toggle built in LED every 5 seconds.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
}
