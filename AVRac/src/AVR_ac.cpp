#include "AVR_ac.h"


namespace ac {

// Constructor.
AVR_ac::AVR_ac() {
    // Initialise the Analog Comparator.
    // Disable everything.
    ACSR = 0;
    isEnabled = false;
    interruptFunction = nullptr;
}


// Configure the AC and start it up.
void AVR_ac::begin(trigger_t trigger,
                   refVoltage_t vReference,
                   testVoltage_t vTest,
                   inputCapture_t inpCapture) {

    // Enable the CA.
    ACSR &= ~(1 << ACD);

    // Reference voltage is 1.1V internal or external
    // on pin AIN1/D7/PD7.
    if (vReference == VREF_EXTERNAL) {
        // AIN0/D6/D6 as reference voltage.
        // Disable digital I/O for AIN0.
        DIDR1 |= (1 << AIN0D);
        ACSR &= ~(1 << ACBG);
    } else {
        // Internal 1.1 V bandgap reference.
        ACSR |= (1 << ACBG);
    }

    // Where will be find the pin with the voltage
    // to be tested against reference?
    if (vTest == VTEST_D7_AIN1) {
        // Pin AIN1/D7/PD7 is test voltage pin.
        // Disable digital I/O on AIN1.
        DIDR1 |= (1 << AIN1D);

        // Two choices now, we can either:
        // Clear ACME in ADCSRB
        // or
        // Set ACME in ADCSRB
        // Set ADEN in ADCSRA.
        //
        // I'm taking the easy option!
        ADCSRB &= ~(1 << ACME);
    } else {
        // Use the ADC MUX pins. (A0->A5 or A0->A7).
        // Power up the ADC.
        PRR &= ~(1 << PRADC);

        // Disable MUX from ADC.
        ADCSRA &= ~(1 << ADEN);

        // Enable MUX in AC.
        ADCSRB |= (1 << ACME);

        // Choose the Analog pin from MUX.
        ADMUX &= 0xF0;
        ADMUX |= vTest;
    }

    // Select interrupt trigger.
    ACSR |= trigger;

    // Do we need Timer/counter 1's input capture
    // unit?
    //
    // WARNING: Setting this requires that Timer/counter 1
    // has an ISR for the input capture unit, and that
    // the interrupt is enabled.
    if (inpCapture == INPUTCAPT_ON) {
        ACSR |= (1 << ACIC);
    }

    // Clear interrupt flag & enable the interrupt.
    ACSR |= (1 << ACI);
    ACSR |= (1 << ACIE);

    isEnabled = true;
}


// Make sure we have a function to call when the
// CA interrupt fires.
void AVR_ac::onInterruptTriggered(onInterrupt functionName) {
    // Probably best not to change this when enabled!
    if (!isEnabled) {
        interruptFunction = functionName;
    }
}

} // End of namespace

// Define our one instance of the AVR_ac class.
ac::AVR_ac AVRac;

// This is the ISR for the CA interrupt. It will
// call the required user function if there is one.
ISR(ANALOG_COMP_vect) {
    AVRac.interrupt();
}
