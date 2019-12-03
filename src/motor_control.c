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
#include "inc/hw_gpio.h"


/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define TEST


/*  G L O B A L   V A R I A B L E S   */
extern eState printer_state;
extern QueueHandle_t motor_instruction_queue;
extern TaskHandle_t xMotorTask;


/*  P R I V A T E   V A R I A B L E S   */
static Motor_t x_motor;
static Motor_t y_motor;
//static Motor_t z_motor;
//static Motor_t ex_motor;

static uint32_t x_pwm_count = 0;
static uint32_t y_pwm_count = 0;
//static uint32_t z_pwm_count = 0;
//static uint32_t ex_pwm_count = 0;


/*  T A S K S   */
void prv_Motor(void *pvParameters) {
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 1000 );  // TODO: switch to max port delay
    uint32_t ulNotificationValue;

//    // init motors
    init_all_motors();
    bool do_it = false;

    for( ;; ) {
        /* Wait for current instruction to be completed */
//        ulNotificationValue = ulTaskNotifyTake( pdFALSE, xMaxBlockTime );

//        if( (ulNotificationValue == 1)  && (printer_state == Printing)) {
            // pop from queue
        if(do_it == true) {
            Motor_Instruction_t * current_instruction;
            xQueueReceive(motor_instruction_queue, (void *)&current_instruction, (TickType_t)5);

//            // set up motors
            find_direction(current_instruction->x_pos, x_motor);
            find_direction(current_instruction->y_pos, y_motor);
//            find_direction(current_instruction->z_pos, z_motor);
//            find_direction(current_instruction->x_pos, ex_motor);

            // update positions
            x_motor.position = current_instruction->x_pos;
            y_motor.position = current_instruction->y_pos;
//            z_motor.position = current_instruction->z_pos;
//            ex_motor.position = current_instruction->extruder_pos;

            // find step counts
            x_pwm_count = dist_to_steps(current_instruction->x_pos);
            y_pwm_count = dist_to_steps(current_instruction->y_pos);
//            z_pwm_count = dist_to_steps(current_instruction->z_pos);
//            ex_pwm_count = dist_to_steps(current_instruction->ex_pos);


            // start PWM on all motors
            motor_enable(x_motor);
            motor_change_pwm_duty_cycle(x_motor, 50);

            motor_enable(y_motor);
            motor_change_pwm_duty_cycle(y_motor, 50);

            do_it = false;
        }
        else if(do_it == false) {
            motor_disable(x_motor);
            motor_disable(y_motor);
            motor_change_pwm_duty_cycle(x_motor, 0);
            motor_change_pwm_duty_cycle(y_motor, 0);
            do_it = true;
        }
//        else {  // taking notification timed out, indicate error occurred
//            printer_state = Error;
//        }
        vTaskDelay(xMaxBlockTime);
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
uint32_t steps_to_dist(uint32_t stepCount) {
    return stepCount*DIST_PER_USTEP;
}

//This is used to convert the desired distance into a step count.
uint32_t dist_to_steps(uint32_t distance) {
    return distance*USTEP_PER_DIST;
}


/*  F U N C T I O N S   */

void init_x_motor(void) {
    // TODO: assign values to the x_motor struct
    motor_init_x_gpio();
    motor_init_x_pwm();
    motor_disable(x_motor);
//    motor_change_pwm_duty_cycle(x_motor, 50);
//    motor_enable(x_motor);
}

void init_y_motor(void) {
    // TODO: assign values to the y_motor struct
    motor_init_y_gpio();
    motor_init_y_pwm();
    motor_disable(y_motor);
}

#ifndef TEST

void init_z_motor(void) {
    // TODO: assign values to the z_motor struct
    motor_init_z_gpio();
    motor_init_z_pwm();
    motor_disable(z_motor);
}

void init_ex_motor(void) {
    // TODO: assign values to the y_motor struct
    motor_init_ex_gpio();
    motor_init_ex_pwm();
    motor_disable(ex_motor);
}

#endif

void init_all_motors(void) {
    init_x_motor();
    init_y_motor();

    // Commented for POC testing
#ifndef TEST
    init_z_motor();
    init_ex_motor();
#endif
}

/*  M O T O R   P W M   */
void motor_init_x_pwm(void) {

    x_motor.PWM_Base = X_MOTOR_PWM_BASE;
    x_motor.PWM_Channel = X_MOTOR_PWM_CHANNEL;
    x_motor.PWM_Block = X_PWM_BLOCK;
    x_motor.PWM_Pin_Map = X_MOTOR_PWM_OUT;


    // TODO: need to see if there's a way to make this more generic
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);         // The PWM peripheral must be enabled for use.

    /* init GPIO pin */
    SysCtlPeripheralEnable(x_motor.STEP.base);            // enable GPIO port if not already enabled

    /* setup and enable clock */
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);                 // Set the PWM clock to the system clock.

    HWREG(GPIO_PORTB_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTE_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;

    // Port B pins that are locked are 3 and 2, so unlock them by writing 1100 into the CR reg
    HWREG(GPIO_PORTB_BASE + GPIO_O_CR)  |= 0xC;


    GPIOPinConfigure(x_motor.PWM_Pin_Map);                // configure pin for PWM
    GPIOPinTypePWM(x_motor.STEP.base, x_motor.STEP.pin);

    /* Count down without synchronization */
    PWMGenConfigure(x_motor.PWM_Base, X_PWM_BLOCK, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    /* Set PWM period to: 0.02ms or 50kHz */
    PWMGenPeriodSet(x_motor.PWM_Base, x_motor.PWM_Block, CALC_PERIOD(PWM_FREQUENCY));

    /* initialize to no output */
    PWMPulseWidthSet(x_motor.PWM_Base, X_PWM_OUT, 0);

    /* Enable Interrupts */
    PWMGenIntRegister(x_motor.PWM_Base, x_motor.PWM_Block, PWM0IntHandler);

    /* Enable the generator block to start timer */
    PWMGenEnable(x_motor.PWM_Base, x_motor.PWM_Block);

    PWMOutputState(x_motor.PWM_Base, (1 << x_motor.PWM_Channel), true);

}


void motor_init_y_pwm(void) {

    y_motor.PWM_Base = Y_MOTOR_PWM_BASE;
    y_motor.PWM_Channel = Y_MOTOR_PWM_CHANNEL;
    y_motor.PWM_Block = Y_PWM_BLOCK;
    y_motor.PWM_Pin_Map = Y_MOTOR_PWM_OUT;

    /* setup and enable clock */
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);                 // Set the PWM clock to the system clock.

    // TODO: need to see if there's a way to make this more generic
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);         // The PWM peripheral must be enabled for use.

    /* init GPIO pin */
    /* Should be done in gpio_init */
    SysCtlPeripheralEnable(y_motor.STEP.base);            // enable GPIO port if not already enabled

    GPIOPinConfigure(y_motor.PWM_Pin_Map);                // configure pin for PWM rather than for GPIO
    GPIOPinTypePWM(y_motor.STEP.base, y_motor.STEP.pin);

    /* Count down without synchronization */
    PWMGenConfigure(y_motor.PWM_Base, y_motor.PWM_Block, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    /* Set PWM period to: 0.02ms or 50kHz */
    PWMGenPeriodSet(y_motor.PWM_Base, y_motor.PWM_Block, CALC_PERIOD(PWM_FREQUENCY));

    /* initialize to no output */
    PWMPulseWidthSet(y_motor.PWM_Base, Y_PWM_OUT, 0);

    /* Enable Interrupts */
    PWMGenIntRegister(y_motor.PWM_Base, y_motor.PWM_Block, PWM0IntHandler);

    /* Enable the generator block to start timer */
    PWMGenEnable(y_motor.PWM_Base, y_motor.PWM_Block);
}

#ifndef TEST
void motor_init_z_pwm(void) {
    z_motor.PWM_Base = Z_MOTOR_PWM_BASE;
    z_motor.PWM_Channel = Z_MOTOR_PWM_CHANNEL;
    z_motor.PWM_Pin_Map = Z_MOTOR_STEP;

    /* setup and enable clock */
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);                 // Set the PWM clock to the system clock.

    // TODO: need to see if there's a way to make this more generic
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);         // The PWM peripheral must be enabled for use.

    /* init GPIO pin */
    SysCtlPeripheralEnable(z_motor.STEP.base);            // enable GPIO port if not already enabled
    GPIOPinConfigure(z_motor.PWM_Pin_Map);                // configure pin for PWM
    GPIOPinTypePWM(z_motor.STEP.base, z_motor.STEP.pin);

    /* Count down without synchronization */
    PWMGenConfigure(z_motor.PWM_Base, Z_PWM_BLOCK, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    /* Set PWM period to: 0.02ms or 50kHz */
    PWMGenPeriodSet(z_motor.PWM_Base, Z_PWM_BLOCK, CALC_PERIOD(PWM_FREQUENCY));

    /* initialize to no output */
    PWMPulseWidthSet(z_motor.PWM_Base, Z_PWM_OUT, 0);

    /* Enable Interrupts */
    PWMGenIntRegister(z_motor.PWM_Base, Z_PWM_BLOCK, PWM0IntHandler);
}

void motor_init_ex_pwm(void) {

    ex_motor.PWM_Base = EX_MOTOR_PWM_BASE;
    ex_motor.PWM_Channel = EX_MOTOR_PWM_CHANNEL;
    ex_motor.PWM_Pin_Map = EX_MOTOR_STEP;

    /* setup and enable clock */
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);                 // Set the PWM clock to the system clock.

    // TODO: need to see if there's a way to make this more generic
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);         // The PWM peripheral must be enabled for use.

    /* init GPIO pin */
    SysCtlPeripheralEnable(ex_motor.STEP.base);            // enable GPIO port if not already enabled
    GPIOPinConfigure(ex_motor.PWM_Pin_Map);                // configure pin for PWM
    GPIOPinTypePWM(ex_motor.STEP.base, ex_motor.STEP.pin);

    /* Count down without synchronization */
    PWMGenConfigure(ex_motor.PWM_Base, EX_PWM_BLOCK, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    /* Set PWM period to: 0.02ms or 50kHz */
    PWMGenPeriodSet(ex_motor.PWM_Base, EX_PWM_BLOCK, CALC_PERIOD(PWM_FREQUENCY));

    /* initialize to no output */
    PWMPulseWidthSet(ex_motor.PWM_Base, EX_PWM_OUT, 0);

    /* Enable Interrupts */
    PWMGenIntRegister(ex_motor.PWM_Base, EX_PWM_BLOCK, PWM0IntHandler);
}
#endif


/* @param uint8_t duty_cycle: 0-100 */
void motor_change_pwm_duty_cycle(Motor_t motor, uint8_t duty_cycle) {
    PWMPulseWidthSet(motor.PWM_Base, motor.PWM_Block, ((duty_cycle * CALC_PERIOD(PWM_FREQUENCY))/100));
}

void set_pwm_load_reg(uint32_t val)
{

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

void motor_init_x_gpio(void)
{
    // Assign Struct members to definitions
    x_motor.M0.base   =    X_M0_PORT;
    x_motor.M0.pin    =    X_M0_PIN;

    x_motor.M1.base   =    X_M1_PORT;
    x_motor.M1.pin    =    X_M1_PIN;

    x_motor.DIR.base  =    X_DIR_PORT;
    x_motor.DIR.pin   =    X_DIR_PIN;

    x_motor.ENABLE.base =  X_ENABLE_PORT;
    x_motor.ENABLE.pin  =  X_ENABLE_PIN;

    x_motor.NSLEEP.base =  X_NSLEEP_PORT;
    x_motor.NSLEEP.pin  =  X_NSLEEP_PIN;

    x_motor.NFAULT.base =  X_NFAULT_PORT;
    x_motor.NFAULT.pin  =  X_NFAULT_PIN;

    x_motor.STEP.base   =  X_STEP_PORT;
    x_motor.STEP.pin    =  X_STEP_PIN;

    // Enable Ports

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    /* Set GPIO output pins */
    MAP_GPIODirModeSet(x_motor.M0.base, x_motor.M0.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(x_motor.M1.base, x_motor.M1.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(x_motor.DIR.base, x_motor.DIR.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(x_motor.ENABLE.base, x_motor.ENABLE.pin, GPIO_DIR_MODE_OUT);
//    MAP_GPIODirModeSet(x_motor.STEP.base, x_motor.STEP.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(x_motor.NSLEEP.base, x_motor.NSLEEP.pin, GPIO_DIR_MODE_OUT);

    /* Set GPIO Input pins */
    MAP_GPIODirModeSet(x_motor.NFAULT.base, x_motor.NFAULT.pin, GPIO_DIR_MODE_IN);

    /* Set Drive Strength */
    MAP_GPIOPadConfigSet(x_motor.M0.base, x_motor.M0.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD); // used to set drive strength
    MAP_GPIOPadConfigSet(x_motor.M1.base, x_motor.M1.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(x_motor.DIR.base, x_motor.DIR.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(x_motor.ENABLE.base, x_motor.ENABLE.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
//    MAP_GPIOPadConfigSet(x_motor.STEP.base, x_motor.STEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(x_motor.NSLEEP.base, x_motor.NSLEEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(x_motor.NFAULT.base, x_motor.NFAULT.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
}

void motor_init_y_gpio(void)
{
    // Assign Struct members to definitions
    y_motor.M0.base   =    Y_M0_PORT;
    y_motor.M0.pin    =    Y_M0_PIN;

    y_motor.M1.base   =    Y_M1_PORT;
    y_motor.M1.pin    =    Y_M1_PIN;

    y_motor.DIR.base  =    Y_DIR_PORT;
    y_motor.DIR.pin   =    Y_DIR_PIN;

    y_motor.ENABLE.base =  Y_ENABLE_PORT;
    y_motor.ENABLE.pin  =  Y_ENABLE_PIN;

    y_motor.NSLEEP.base =  Y_NSLEEP_PORT;
    y_motor.NSLEEP.pin  =  Y_NSLEEP_PIN;

    y_motor.NFAULT.base =  Y_NFAULT_PORT;
    y_motor.NFAULT.pin  =  Y_NFAULT_PIN;

    y_motor.STEP.base   =  Y_STEP_PORT;
    y_motor.STEP.pin    =  Y_STEP_PIN;

    // Enable Ports

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  // Port B
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // Port F
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);  // Port C

    /* Set GPIO output pins */
    MAP_GPIODirModeSet(y_motor.M0.base, y_motor.M0.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(y_motor.M1.base, y_motor.M1.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(y_motor.DIR.base, y_motor.DIR.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(y_motor.ENABLE.base, y_motor.ENABLE.pin, GPIO_DIR_MODE_OUT);
//    MAP_GPIODirModeSet(y_motor.STEP.base, y_motor.STEP.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(y_motor.NSLEEP.base, y_motor.NSLEEP.pin, GPIO_DIR_MODE_OUT);

    /* Set GPIO Input pins */
    MAP_GPIODirModeSet(y_motor.NFAULT.base, y_motor.NFAULT.pin, GPIO_DIR_MODE_IN);

    /* Set Drive Strength */
    MAP_GPIOPadConfigSet(y_motor.M0.base, y_motor.M0.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD); // used to set drive strength
    MAP_GPIOPadConfigSet(y_motor.M1.base, y_motor.M1.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(y_motor.DIR.base, y_motor.DIR.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(y_motor.ENABLE.base, y_motor.ENABLE.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
//    MAP_GPIOPadConfigSet(y_motor.STEP.base, y_motor.STEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(y_motor.NSLEEP.base, y_motor.NSLEEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(y_motor.NFAULT.base, y_motor.NFAULT.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
}


/* Commented for Test */
#ifndef TEST
void motor_init_z_gpio(void)
{
    // Assign Struct members to definitions
    z_motor.M0.base   =    Z_M0_PORT;
    z_motor.M0.pin    =    Z_M0_PIN;

    z_motor.M1.base   =    Z_M1_PORT;
    z_motor.M1.pin    =    Z_M1_PIN;

    z_motor.DIR.base  =    Z_DIR_PORT;
    z_motor.DIR.pin   =    Z_DIR_PIN;

    z_motor.ENABLE.base =  Z_ENABLE_PORT;
    z_motor.ENABLE.pin  =  Z_ENABLE_PIN;

    z_motor.NSLEEP.base =  Z_NSLEEP_PORT;
    z_motor.NSLEEP.pin  =  Z_NSLEEP_PIN;

    z_motor.NFAULT.base =  Z_NFAULT_PORT;
    z_motor.NFAULT.pin  =  Z_NFAULT_PIN;

    z_motor.STEP.base   =  Z_STEP_PORT;
    z_motor.STEP.pin    =  Z_STEP_PIN;

    // Enable Ports

    MAP_SysCtlPeripheralEnable(z_motor.M0.base);      // Port E
    MAP_SysCtlPeripheralEnable(z_motor.M1.base);      // Port J
    MAP_SysCtlPeripheralEnable(z_motor.NFAULT.base);  // Port H

    /* Set GPIO output pins */
    MAP_GPIODirModeSet(z_motor.M0.base, z_motor.M0.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(z_motor.M1.base, z_motor.M1.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(z_motor.DIR.base, z_motor.DIR.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(z_motor.ENABLE.base, z_motor.ENABLE.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(z_motor.STEP.base, z_motor.STEP.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(z_motor.NSLEEP.base, z_motor.NSLEEP.pin, GPIO_DIR_MODE_OUT);

    /* Set GPIO Input pins */
    MAP_GPIODirModeSet(z_motor.NFAULT.base, z_motor.NFAULT.pin, GPIO_DIR_MODE_IN);

    /* Set Drive Strength */
    MAP_GPIOPadConfigSet(z_motor.M0.base, z_motor.M0.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD); // used to set drive strength
    MAP_GPIOPadConfigSet(z_motor.M1.base, z_motor.M1.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(z_motor.DIR.base, z_motor.DIR.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(z_motor.ENABLE.base, z_motor.ENABLE.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(z_motor.STEP.base, z_motor.STEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(z_motor.NSLEEP.base, z_motor.NSLEEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(z_motor.NFAULT.base, z_motor.NFAULT.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
}

void motor_init_ex_gpio(void)
{
    // Assign Struct members to definitions
    ex_motor.M0.base   =    EX_M0_PORT;
    ex_motor.M0.pin    =    EX_M0_PIN;

    ex_motor.M1.base   =    EX_M1_PORT;
    ex_motor.M1.pin    =    EX_M1_PIN;

    ex_motor.DIR.base  =    EX_DIR_PORT;
    ex_motor.DIR.pin   =    EX_DIR_PIN;

    ex_motor.ENABLE.base =  EX_ENABLE_PORT;
    ex_motor.ENABLE.pin  =  EX_ENABLE_PIN;

    ex_motor.NSLEEP.base =  EX_NSLEEP_PORT;
    ex_motor.NSLEEP.pin  =  EX_NSLEEP_PIN;

    ex_motor.NFAULT.base =  EX_NFAULT_PORT;
    ex_motor.NFAULT.pin  =  EX_NFAULT_PIN;

    ex_motor.STEP.base   =  EX_STEP_PORT;
    ex_motor.STEP.pin    =  EX_STEP_PIN;

    // Enable Ports

    MAP_SysCtlPeripheralEnable(ex_motor.M0.base);      // Port E
    MAP_SysCtlPeripheralEnable(ex_motor.M1.base);      // Port J
    MAP_SysCtlPeripheralEnable(ex_motor.NFAULT.base);  // Port H

    /* Set GPIO output pins */
    MAP_GPIODirModeSet(ex_motor.M0.base, ex_motor.M0.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(ex_motor.M1.base, ex_motor.M1.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(ex_motor.DIR.base, ex_motor.DIR.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(ex_motor.ENABLE.base, ex_motor.ENABLE.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(ex_motor.STEP.base, ex_motor.STEP.pin, GPIO_DIR_MODE_OUT);
    MAP_GPIODirModeSet(ex_motor.NSLEEP.base, ex_motor.NSLEEP.pin, GPIO_DIR_MODE_OUT);

    /* Set GPIO Input pins */
    MAP_GPIODirModeSet(ex_motor.NFAULT.base, ex_motor.NFAULT.pin, GPIO_DIR_MODE_IN);

    /* Set Drive Strength */
    MAP_GPIOPadConfigSet(ex_motor.M0.base, ex_motor.M0.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD); // used to set drive strength
    MAP_GPIOPadConfigSet(ex_motor.M1.base, ex_motor.M1.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(ex_motor.DIR.base, ex_motor.DIR.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(ex_motor.ENABLE.base, ex_motor.ENABLE.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(ex_motor.STEP.base, ex_motor.STEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(ex_motor.NSLEEP.base, ex_motor.NSLEEP.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
    MAP_GPIOPadConfigSet(ex_motor.NFAULT.base, ex_motor.NFAULT.pin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
}

#endif

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
