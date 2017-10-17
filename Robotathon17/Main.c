#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/adc.h>

// Blink the LED to show we're on
tBoolean blink_on = true;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}


// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);
    //tMotor *left = InitializeServoMotor(PIN_D0, true);
    //tMotor *right = InitializeServoMotor(PIN_D1, false);
    tLineSensor *line = InitializeGPIOLineSensor(PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_E1, PIN_E2, PIN_E3);
    float linevals[8];
    tADC *dist = InitializeADC(PIN_B0);
    float distval;
    tADC *dist2 = InitializeADC(PIN_B1);
    float distval2;
    while (1) {
        // Runtime code can go here
        //SetMotor(left, 1.0);
        //SetMotor(right, -1.0);
        /*LineSensorReadArray(line, linevals);
        Printf(“%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t”, value[0], value[1], value[2],
          value[3], value[4], value[5], value[6], value[7]);*/
        distval = ADCRead(dist);
        Printf("IR sensor value is %f\n", distval);
        distval2 = ADCRead(dist2);
        Printf("IR sensor value is %f\n", distval2);
    }
}
