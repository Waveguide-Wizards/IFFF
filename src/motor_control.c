/*
 * motor_control.c
 *
 *  Created on: Oct 6, 2019
 *      Author: jack
 */

/*  I N C L U D E S   */
#include <stdbool.h>
#include <stdint.h>

/*  A P P L I C A T I O N   I N C L U D E S   */
#include "bsp.h"
#include "motor_control.h"

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
#include "queue.h"


/*  G L O B A L   V A R I A B L E S   */
extern eState printer_state;
extern QueueHandle_t motor_instruction_queue;
extern TaskHandle_t xMotorTask;


/*  P R I V A T E   V A R I A B L E S   */
static Motor_t x_motor;
static Motor_t y_motor;
static Motor_t z_motor;
static Motor_t ex_motor;


/*  T A S K S   */
void prv_Motor(void *pvParameters) {
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );  // TODO: switch to max port delay
    uint32_t ulNotificationValue;

    // init motors
    init_all_motors();

    for( ;; ) {
        /* Wait for current instruction to be completed */
        ulNotificationValue = ulTaskNotifyTake( pdFALSE, xMaxBlockTime );

        if( (ulNotificationValue == 1)  && (printer_state == Printing)) {
            // pop from queue
            Motor_Instruction_t * current_instruction;
            xQueueReceive(motor_instruction_queue, (void *)&current_instruction, (TickType_t)5);

            // set up motors
            find_direction(current_instruction->x_pos, x_motor);
            find_direction(current_instruction->y_pos, y_motor);
            find_direction(current_instruction->z_pos, z_motor);
            find_direction(current_instruction->x_pos, ex_motor);

            // update positions
            x_motor.position = current_instruction->x_pos;
            y_motor.position = current_instruction->y_pos;
            z_motor.position = current_instruction->z_pos;
            ex_motor.position = current_instruction->extruder_pos;

            // find step counts

            // load step counts into PWM registers

            // start PWM on all motors

        }
        else {  // taking notification timed out, indicate error occurred
            printer_state = Error;
        }

    }
}


/*  F U N C T I O N S   */
void find_direction(uint32_t instruction, Motor_t motor) {
    if(instruction >= motor.position) {
        motor.direction = Backward;
    }
    else {
        motor.direction = Forward;
    }
}

//This is used to convert the numer of steps taken into a distance in micrometers.
uint32_t StepsToDist(uint32_t stepCount){
    return stepCount*DIST_PER_USTEP;
}

//This is used to convert the desired distance into a step count.
uint32_t DistToSteps(uint32_t  distance){
    return distance*USTEP_PER_DIST;
}


void init_x_motor(void) {
    // TODO: assign values to the x_motor struct
    motor_init_gpio(x_motor);
    motor_init_pwm(x_motor);
    motor_disable(x_motor);
}

void init_y_motor(void) {
    // TODO: assign values to the y_motor struct
    motor_init_gpio(y_motor);
    motor_init_pwm(y_motor);
    motor_disable(y_motor);
}

void init_z_motor(void) {
    // TODO: assign values to the z_motor struct
    motor_init_gpio(z_motor);
    motor_init_pwm(z_motor);
    motor_disable(z_motor);
}

void init_ex_motor(void) {
    // TODO: assign values to the y_motor struct
    motor_init_gpio(ex_motor);
    motor_init_pwm(ex_motor);
    motor_disable(ex_motor);
}

void init_all_motors(void) {
    init_x_motor();
    init_y_motor();
    init_z_motor();
    init_ex_motor();
}

/*  M O T O R   P W M   */
void motor_init_pwm(Motor_t motor) {
    /* setup and enable clock */
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);                 // Set the PWM clock to the system clock.

    // TODO: need to see if there's a way to make this more generic
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);         // The PWM peripheral must be enabled for use.

    /* init GPIO pin */
    SysCtlPeripheralEnable(motor.STEP.base);            // enable GPIO port if not already enabled
    GPIOPinConfigure(motor.PWM_Pin_Map);                // configure pin for PWM
    GPIOPinTypePWM(motor.STEP.base, motor.STEP.pin);

    /* Count down without synchronization */
    PWMGenConfigure(motor.PWM_Base, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    /* Set PWM period to: 0.02ms or 50kHz */
    PWMGenPeriodSet(motor.PWM_Base, PWM_GEN_0, CALC_PERIOD(PWM_FREQUENCY));

    /* initialize to no output */
    PWMPulseWidthSet(motor.PWM_Base, PWM_OUT_0, 0);
}

/* @param uint8_t duty_cycle: 0-100 */
void motor_change_pwm_duty_cycle(Motor_t motor, uint8_t duty_cycle) {
    PWMPulseWidthSet(motor.PWM_Base, PWM_OUT_0, ((duty_cycle * CALC_PERIOD(PWM_FREQUENCY))/100));
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
void motor_init_gpio(Motor_t motor) {
    // Enable GPIO port(s) for motor driver control
    MAP_SysCtlPeripheralEnable(motor.M0.base);
    MAP_SysCtlPeripheralEnable(motor.M1.base);
    MAP_SysCtlPeripheralEnable(motor.DIR.base);
    MAP_SysCtlPeripheralEnable(motor.ENABLE.base);
    MAP_SysCtlPeripheralEnable(motor.STEP.base);
    MAP_SysCtlPeripheralEnable(motor.NSLEEP.base);
    MAP_SysCtlPeripheralEnable(motor.NFAULT.base);

    /* Set GPIO output pins */
    MAP_GPIODirModeSet(motor.M0.base, motor.M0.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(motor.M1.base, motor.M1.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(motor.DIR.base, motor.DIR.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(motor.ENABLE.base, motor.ENABLE.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(motor.STEP.base, motor.STEP.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(motor.NSLEEP.base, motor.NSLEEP.pin, GPIO_DIR_MODE_OUT);

    /* Set GPIO Input pins */
    MAP_GPIODirModeSet(motor.NFAULT.base, motor.NFAULT.pin, GPIO_DIR_MODE_IN);

    /* Set Drive Strength */
    MAP_GPIOPadConfigSet(); // used to set drive strength

}

void motor_enable(Motor_t motor) {
    GPIOPinWrite(motor.ENABLE.base, motor.ENABLE.pin, motor.ENABLE.pin);    // set ENABLE pin HIGH
    PWMOutputState(motor.PWM_Base, (1 << motor.PWM_Channel), true);    // disables PWM output
    PWMGenEnable(motor.PWM_Base, PWM_GEN_0);                     // enables PWM
}

void motor_disable(Motor_t motor) {
    GPIOPinWrite(motor.ENABLE.base, motor.ENABLE.pin, 0);               // set ENABLE pin LOW
    PWMOutputState(motor.PWM_Base, (1 << motor.PWM_Channel), false);    // disables PWM output
    PWMGenDisable(motor.PWM_Base, PWM_GEN_0);                                // disable PWM
}

void motor_set_to_sleep(Motor_t motor) {
    GPIOPinWrite(motor.NSLEEP.base, motor.NSLEEP.pin, 0);   // set nSLEEP pin LOW
}

void motor_set_direction(Motor_t motor, eMotor_Direction direction) {
    if(direction == Forward) {
        GPIOPinWrite(motor.DIR.base, motor.DIR.pin, motor.DIR.pin);
    }
    else if(direction == Backward) {
        GPIOPinWrite(motor.DIR.base, motor.DIR.pin, 0);
    }
}

void set_motor_step_size(Motor_t motor, uint8_t direction){
    switch(direction)
    {
    case STEP_FULL:
        GPIOPinWrite(motor.M0.base, motor.M0.pin, 0);
        GPIOPinWrite(motor.M1.base, motor.M1.pin, 0);
        break;
    case STEP_16:
        GPIOPinWrite(motor.M0.base, motor.M0.pin, 1);
        GPIOPinWrite(motor.M1.base, motor.M1.pin, 0);
        break;
    case STEP_2:
        GPIOPinWrite(motor.M0.base, motor.M0.pin, 0);
        GPIOPinWrite(motor.M1.base, motor.M1.pin, 1);
        break;
    case STEP_4:
        GPIOPinWrite(motor.M0.base, motor.M0.pin, 1);
        GPIOPinWrite(motor.M1.base, motor.M1.pin, 1);
        break;
    case STEP_8:
        GPIOPinWrite(motor.M0.base, motor.M0.pin, 1);
        GPIOPinWrite(motor.M1.base, motor.M1.pin, 1);
        break;
    }
}

/*
 * TODO:
 *  - use LOAD register to count Pulses
 *  - set PWM duty cycle for X/Y/Z to 0 in this handler
 *  - notify motors task once an interrupt flag for each motor has been raised
 */

// static bool x_step_flag;
void PWM0IntHandler(void) {
    // TODO: if step count met for all motors execute this
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    configASSERT(xMotorTask != NULL);
    vTaskNotifyGiveFromISR(xMotorTask, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}


//This is used to convert the numer of steps taken into a distance in micrometers.
uint32_t StepsToDist(uint32_t stepCount){
    return stepCount*DIST_PER_USTEP;
}

//This is used to convert the desired distance into a step count.
uint32_t DistToSteps(uint32_t  distance){
    return distance*USTEP_PER_DIST;
}

