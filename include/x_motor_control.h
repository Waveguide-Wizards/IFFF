/*
 * motor_control.h
 *
 *  Created on: Oct 25, 2019
 *      Author: jack
 */

#ifndef X_MOTOR_CONTROL_H_
#define X_MOTOR_CONTROL_H_

#include <stdint.h>

#define SOURCE_FREQUENCY    20000000    // 20MHz
#define PWM0_FREQUENCY      50000       // 50kHz, DRV8886 f_pwm range is 0-100kHz
#define CALC_PERIOD(X)      (SOURCE_FREQUENCY / X)

/*  E N U M S   */
typedef enum {
    Forward = 0,
    Backward = 1
} eMotor_Direction;


/*  X - MOTOR   */
#define X_MOTOR_GPIO_BASE       GPIO_PORTB_BASE

//#define X_M1_PORT       GPIO_PORTB_BASE
#define X_M1_PIN        GPIO_PIN_0      // output
#define X_M1        GPIO_PIN_0      // output
#define X_M0        GPIO_PIN_1      // output
#define X_DIR       GPIO_PIN_2      // output
#define X_ENABLE    GPIO_PIN_3      // output
#define X_NSLEEP    GPIO_PIN_4      // output
#define X_NFAULT    GPIO_PIN_5      // input
#define X_STEP      GPIO_PIN_6      // output


/*  T A S K S   */
void prvX_Motor(void *pvParameters);

/*  F U N C T I O N S */
void init_x_motor(void);

/*  X   M O T O R   P W M   */

void init_x_motor_pwm(void);
void change_x_motor_pwm_duty_cycle(uint8_t duty_cycle);

/*  X   M O T O R   G P I O   */

void init_x_motor_gpio(void);
void enable_x_motor(void);
void disable_x_motor(void);
void set_x_motor_to_sleep(void);


/*  Y - MOTOR   */


#endif /* X_MOTOR_CONTROL_H_ */
