/*
 * bsp.h
 *
 *  Created on: Nov 19, 2019
 *      Author: jack
 */

#ifndef BSP_H_
#define BSP_H_

/*  D R I V E R   L I B   */
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"

typedef enum {
    Error = -1,
    Idle,
    Printing,
    MemoryTransfer,
} eState;

/*
 * TODO: Change these to real pins and ports
 */

/*  X - MOTOR   */
#define X_M1_PORT           GPIO_PORTJ_BASE
#define X_M1_PIN            GPIO_PIN_2      // output

#define X_M0_PORT           GPIO_PORTE_BASE
#define X_M0_PIN            GPIO_PIN_3      // output

#define X_DIR_PORT          GPIO_PORTE_BASE
#define X_DIR_PIN           GPIO_PIN_2      // output

#define X_ENABLE_PORT       GPIO_PORTE_BASE
#define X_ENABLE_PIN        GPIO_PIN_1      // output

#define X_NSLEEP_PORT       GPIO_PORTE_BASE
#define X_NSLEEP_PIN        GPIO_PIN_0      // output

#define X_NFAULT_PORT       GPIO_PORTH_BASE
#define X_NFAULT_PIN        GPIO_PIN_1      // input

#define X_STEP_PORT         GPIO_PORTH_BASE
#define X_STEP_PIN          GPIO_PIN_0      // output

#define X_MOTOR_PWM_BASE    PWM0_BASE       //  M0PWM0
#define X_MOTOR_PWM_CHANNEL 0

#define X_MOTOR_STEP        GPIOH_PH0_M0PWM0

/*  Y - MOTOR   */
#define Y_M1_PORT           GPIO_PORTH_BASE
#define Y_M1_PIN            GPIO_PIN_3      // output

#define Y_M0_PORT           GPIO_PORTC_BASE
#define Y_M0_PIN            GPIO_PIN_7      // output

#define Y_DIR_PORT          GPIO_PORTC_BASE
#define Y_DIR_PIN           GPIO_PIN_6      // output

#define Y_ENABLE_PORT       GPIO_PORTC_BASE
#define Y_ENABLE_PIN        GPIO_PIN_5      // output

#define Y_NSLEEP_PORT       GPIO_PORTA_BASE
#define Y_NSLEEP_PIN        GPIO_PIN_0      // output

#define Y_NFAULT_PORT       GPIO_PORTA_BASE
#define Y_NFAULT_PIN        GPIO_PIN_1      // input

#define Y_STEP_PORT         GPIO_PORTC_BASE
#define Y_STEP_PIN          GPIO_PIN_4      // output

#define Y_MOTOR_PWM_BASE    PWM0_BASE       //  M0PWM6
#define Y_MOTOR_PWM_CHANNEL 6

#define Y_MOTOR_STEP        GPIOH_PC4_M0PWM6


/*  Z - MOTOR   */
#define Z_M1_PORT           GPIO_PORTF_BASE
#define Z_M1_PIN            GPIO_PIN_7      // output

#define Z_M0_PORT           GPIO_PORTG_BASE
#define Z_M0_PIN            GPIO_PIN_2      // output

#define Z_DIR_PORT          GPIO_PORTG_BASE
#define Z_DIR_PIN           GPIO_PIN_1      // output

#define Z_ENABLE_PORT       GPIO_PORTG_BASE
#define Z_ENABLE_PIN        GPIO_PIN_0      // output

#define Z_NSLEEP_PORT       GPIO_PORTB_BASE
#define Z_NSLEEP_PIN        GPIO_PIN_2      // output

#define Z_NFAULT_PORT       GPIO_PORTB_BASE
#define Z_NFAULT_PIN        GPIO_PIN_3      // input

#define Z_STEP_PORT         GPIO_PORTG_BASE
#define Z_STEP_PIN          GPIO_PIN_3      // output

#define Z_MOTOR_PWM_BASE    PWM1_BASE       //  M0PWM6
#define Z_MOTOR_PWM_CHANNEL 1

#define Z_MOTOR_STEP        GPIOH_PG3_M0PWM6


/*  EXTRUDER - MOTOR   */
#define EX_M1_PORT           GPIO_PORTB_BASE
#define EX_M1_PIN            GPIO_PIN_5      // output

#define EX_M0_PORT           GPIO_PORTE_BASE
#define EX_M0_PIN            GPIO_PIN_4      // output

#define EX_DIR_PORT          GPIO_PORTD_BASE
#define EX_DIR_PIN           GPIO_PIN_4      // output

#define EX_ENABLE_PORT       GPIO_PORTD_BASE
#define EX_ENABLE_PIN        GPIO_PIN_5      // output

#define EX_NSLEEP_PORT       GPIO_PORTD_BASE
#define EX_NSLEEP_PIN        GPIO_PIN_6      // output

#define EX_NFAULT_PORT       GPIO_PORTD_BASE
#define EX_NFAULT_PIN        GPIO_PIN_7      // input

#define EX_STEP_PORT         GPIO_PORTE_BASE
#define EX_STEP_PIN          GPIO_PIN_5      // output

#define EX_MOTOR_PWM_BASE    PWM0_BASE       //  M0PWM5
#define EX_MOTOR_PWM_CHANNEL 5

#define EX_MOTOR_STEP        GPIOH_PE5_M0PWM5

#endif /* BSP_H_ */
