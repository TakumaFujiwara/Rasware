#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>

// Blink the LED to show we're on
tBoolean blink_on = true;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}


// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
    //CallEvery(blink, 0, 0.5);
    tMotor *left = InitializeServoMotor(PIN_xx, true);
    tMotor *right = InitializeServoMotor(PIN_xx, false);
    while (1) {
        // Runtime code can go here
        SetMotor(left, 1.0);
        SetMotor(right, -1.0);

    }
}
