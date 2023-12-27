#ifndef AC_H
#define AC_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

namespace ac {

    // a void function to call when the interrupt occurs.
    typedef void (*onInterrupt)();

    // Enumeration for the two reference voltage sources
    // for AIN0 (Arduino D6, AVR PD6).
    typedef enum refVoltage : uint8_t {
        VREF_INTERNAL,
        VREF_EXTERNAL
    } refVoltage_t;

    // Enumeration of the interrupt triggers.
    typedef enum trigger : uint8_t {
        TRGR_CHANGE = 0,
        TRGR_FALLING = (1 << ACIS1),
        TRGR_RISING = ((1 << ACIS1) | (1 << ACIS0))
    } trigger_t ;

    // Enumeration of the pin where the voltage to compare
    // with the reference voltage.
    typedef enum testVoltage : uint8_t {
        VTEST_A0_PC0 = 0,
        VTEST_A1_PC1,
        VTEST_A2_PC2,
        VTEST_A3_PC3,
        VTEST_A4_PC4,
        VTEST_A5_PC5,
        VTEST_A6,
        VTEST_A7,
        VTEST_D7_AIN1
    } testVoltage_t ;

    // Enumeration of the desired Timer/counter 1 input
    // capture unit. This requires that ICIE7 in TMISK1
    // is set, and we have an ISR for it.
    typedef enum inputCapture : uint8_t {
        INPUTCAPT_OFF = 0,
        INPUTCAPT_ON
    } inputCapture_t;


class AVR_ac {

    public:


        // Constructor. There's no destructor as the one
        // AVRac object will never be destructed.
        AVR_ac();

        // Execute the AC interrupt code. This should be called
        // from the ISR.
        void interrupt() {
            if (interruptFunction) {
                interruptFunction();
            }
        }

        // Call here to configure and start the AC.
        void begin(trigger_t trigger,
                refVoltage_t vReference = ac::VREF_EXTERNAL,
                testVoltage_t vTest = ac::VTEST_D7_AIN1,
                inputCapture_t inpCapture = ac::INPUTCAPT_OFF);

        // Enable the CA - use only after disabling it.
        void enable() {
            // Clear the interrupt flag and power off.
            ACSR |= (1 << ACI);
            ACSR &= ~(1 << ACD);
            isEnabled = true;
        }

        // Disable the CA. Use to temporarily halt any
        // further processing.
        void disable() {
            ACSR |= (1 << ACD);
            isEnabled = false;
        }

        // Allow a user function to be attached to the interrupt.
        void onInterruptTriggered(onInterrupt functionName);


    private:

        // (User) function to call when triggered.
        onInterrupt interruptFunction;

        // Are we still enabled?
        bool isEnabled;
    };

} // End of namespace

//There will be one of these in the cpp file for this class.
extern ac::AVR_ac AVRac;

#endif // AC_H
