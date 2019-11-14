/*
 * motor_control.c
 *
 *  Created on: Oct 6, 2019
 *      Author: jack
 */

/*  I N C L U D E S   */
#include <stdbool.h>
#include <stdint.h>

/*  D R I V E R   L I B   */
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"


/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"


/*  A P P L I C A T I O N   I N C L U D E S   */
#include "x_motor_control.h"


/*  T A S K S   */
void prvX_Motor(void *pvParameters) {
    static TickType_t delay_time = pdMS_TO_TICKS(1000);     // 1s
    static uint8_t duty_cycle = 0;  // 0-100%
    static eMotor_Direction direction = Forward;
    init_x_motor_pwm();
    enable_x_motor();

    for( ;; ) {
        if(direction == Forward) {
            change_x_motor_pwm_duty_cycle(0);
            direction = Backward;
        }
        if(direction == Backward) {
            change_x_motor_pwm_duty_cycle(90);
            direction = Forward;
        }
        vTaskDelay(delay_time);
    }
}


/*  F U N C T I O N S   */

void init_x_motor(void) {
    init_x_motor_pwm();
    init_x_motor_gpio();
}

/*  X   M O T O R   P W M   */
void init_x_motor_pwm(void) {
    /* setup and enable clock */
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);             // Set the PWM clock to the system clock.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);     // The PWM peripheral must be enabled for use.

    /* init GPIO pin - PB6 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);    // enable GPIOB if not already enabled
    GPIOPinConfigure(GPIO_PB6_M0PWM0);              // configure PB6 for PWM0
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);    // select PWM function for PB6

    /* Count down without synchronization */
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    /* Set PWM period to: 0.02ms or 50kHz */
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, CALC_PERIOD(PWM0_FREQUENCY));

    /* initialize to no output */
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 0);
}

/* @param uint8_t duty_cycle: 0-100 */
void change_x_motor_pwm_duty_cycle(uint8_t duty_cycle) {
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ((duty_cycle * CALC_PERIOD(PWM0_FREQUENCY))/100));
}


/*  X   M O T O R   G P I O   */

/* Configure GPIO pins for DRV8886
 *
 *  DIR - direction input,
 *  ENABLE - input, HIGH is enabled, LOW is disabled
 *  M0 and M1 - inputs, microstepping mode, M1=0 and M0=1 for 1/16 step
 *  nFAULT - Fault indication output from DRV8886
 *  nSLEEP - sleep mode input, LOW to enter low-power
 *  DECAY - ?
 *  TRQ - ?
 */
void init_x_motor_gpio(void) {
    // Enable GPIO port(s) for motor driver control
    MAP_SysCtlPeripheralEnable(X_MOTOR_GPIO_BASE);

    /* Set GPIOs to OUTPUT direction */
    MAP_GPIODirModeSet(X_MOTOR_GPIO_BASE, (X_M1 | X_M0 | X_DIR | X_ENABLE | X_NSLEEP), GPIO_DIR_MODE_OUT);

    /* Set GPIO to INPUT direction */
    MAP_GPIODirModeSet(X_MOTOR_GPIO_BASE, X_NFAULT, GPIO_DIR_MODE_IN);

//    MAP_GPIOPadConfigSet(); // used to set drive strength
}

void enable_x_motor(void) {
    GPIOPinWrite(X_MOTOR_GPIO_BASE, X_ENABLE, X_ENABLE);    // set ENABLE pin HIGH
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);         // enables PWM output
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);                     // enables PWM
}

void disable_x_motor(void) {
    GPIOPinWrite(X_MOTOR_GPIO_BASE, X_ENABLE, 0);           // set ENABLE pin LOW
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, false);        // disables PWM output
    PWMGenDisable(PWM0_BASE, PWM_GEN_0);                    // disable PWM
}

void set_x_motor_to_sleep(void) {
    GPIOPinWrite(X_MOTOR_GPIO_BASE, X_NSLEEP, 0);   // set nSLEEP pin LOW
}

void x_motor_set_direction(eMotor_Direction direction) {
    if(direction == Forward) {
        GPIOPinWrite(X_MOTOR_GPIO_BASE, X_DIR, X_DIR);
    }
    else if(direction == Backward) {

    }
}

// if needed
void PWM0IntHandler(void) {}

