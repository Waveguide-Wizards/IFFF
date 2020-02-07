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
    Error,
    Bumper_Error,
    Idle,
    Printing,
    MemoryTransfer,
} eState;

/*
 * TODO: Change these to real pins and ports
 */
#define POC

#ifdef POC
/* X motor Uses PWM Generator block 0 and the output is PWM out 0 */
#define X_PWM_BLOCK         PWM_GEN_0
#define X_PWM_OUT           PWM_OUT_0

#define X_M1_PORT           GPIO_PORTB_BASE
#define X_M1_PIN            GPIO_PIN_0      // output

#define X_M0_PORT           GPIO_PORTB_BASE
#define X_M0_PIN            GPIO_PIN_5      // output

#define X_DIR_PORT          GPIO_PORTB_BASE
#define X_DIR_PIN           GPIO_PIN_1      // output

#define X_ENABLE_PORT       GPIO_PORTE_BASE
#define X_ENABLE_PIN        GPIO_PIN_4      // output

#define X_NSLEEP_PORT       GPIO_PORTE_BASE
#define X_NSLEEP_PIN        GPIO_PIN_5      // output

#define X_NFAULT_PORT       GPIO_PORTE_BASE
#define X_NFAULT_PIN        GPIO_PIN_1      // input

#define X_STEP_PORT         GPIO_PORTB_BASE
#define X_STEP_PIN          GPIO_PIN_6      // output

#define X_MOTOR_PWM_BASE    PWM0_BASE       //  M0PWM0 (PB6)
#define X_MOTOR_PWM_CHANNEL 0

#define X_MOTOR_PWM_OUT     GPIO_PB6_M0PWM0


/*  Y - MOTOR   */
#define Y_PWM_BLOCK         PWM_GEN_1
#define Y_PWM_OUT           PWM_OUT_6

#define Y_M1_PORT           GPIO_PORTF_BASE
#define Y_M1_PIN            GPIO_PIN_3      // output

#define Y_M0_PORT           GPIO_PORTB_BASE
#define Y_M0_PIN            GPIO_PIN_3      // output

#define Y_DIR_PORT          GPIO_PORTC_BASE
#define Y_DIR_PIN           GPIO_PIN_4      // output

#define Y_ENABLE_PORT       GPIO_PORTC_BASE
#define Y_ENABLE_PIN        GPIO_PIN_5      // output

#define Y_NSLEEP_PORT       GPIO_PORTC_BASE
#define Y_NSLEEP_PIN        GPIO_PIN_6      // output

#define Y_NFAULT_PORT       GPIO_PORTC_BASE
#define Y_NFAULT_PIN        GPIO_PIN_7      // input

#define Y_STEP_PORT         GPIO_PORTB_BASE
#define Y_STEP_PIN          GPIO_PIN_4      // output (same as Y_MOTOR_PWM_OUT)

#define Y_MOTOR_PWM_BASE    PWM0_BASE
#define Y_MOTOR_PWM_CHANNEL 2

#define Y_MOTOR_PWM_OUT     GPIO_PB4_M0PWM2

/*
 * TODO:
 * Note, Z and Extruder values are placeholders not actual values
 */

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
#define EX_PWM_BLOCK         PWM_GEN_1
#define EX_PWM_OUT           PWM_OUT_6

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

// TODO: update this, placeholder
#define EX_MOTOR_STEP        GPIO_PA7_M1PWM3

/*  EXTRUDER HEATER */
#define EX_HEATER_PWM_BLOCK     PWM_GEN_3
#define EX_HEATER_PWM_OUT       PWM_OUT_6

#define EX_HEATER_PWM_PORT      GPIO_PORTD_BASE
#define EX_HEATER_PWM_PIN       GPIO_PIN_1

#define EX_HEATER_PWM_BASE      PWM0_BASE
#define EX_HEATER_PWM_CHANNEL   7

#define EX_HEATER_PWM_PIN_MAP   GPIO_PD0_M0PWM6

#define EX_HEATER_ADC_SEQUENCER 3

#define EX_HEATER_ADC_BASE      ADC0_BASE
#define EX_HEATER_ADC_CHANNEL   ADC_CTL_CH21

#define EX_HEATER_ADC_PORT      GPIO_PORTD_BASE
#define EX_HEATER_ADC_PIN       GPIO_PIN_0

/*  BED HEATER   */
#define BED_HEATER_PWM_PIN_MAP   GPIO_PA7_M1PWM3

#define BED_HEATER_PWM_BLOCK     PWM_GEN_1
#define BED_HEATER_PWM_OUT       PWM_OUT_6

#define BED_HEATER_PWM_PORT      GPIO_PORTD_BASE
#define BED_HEATER_PWM_PIN       GPIO_PIN_1

#define BED_HEATER_PWM_BASE      PWM0_BASE
#define BED_HEATER_PWM_CHANNEL   7

#define BED_HEATER_ADC_SEQUENCER    3

#define BED_HEATER_ADC_BASE         ADC1_BASE
#define BED_HEATER_ADC_CHANNEL      ADC_CTL_CH0

#define BED_HEATER_ADC_PORT         GPIO_PORTE_BASE
#define BED_HEATER_ADC_PIN          GPIO_PIN_3

#else


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

#define EX_MOTOR_PWM_BASE    PWM0_BASE       //  M0PWM5
#define EX_MOTOR_PWM_CHANNEL 5

#define EX_MOTOR_STEP        GPIOH_PE5_M0PWM5
#define EX_PWM_BLOCK         PWM_GEN_3

#define EX_PWM_OUT           PWM_OUT_6

#endif // POC
#endif /* BSP_H_ */
