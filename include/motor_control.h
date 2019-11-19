/*
 * motor_control.h
 *
 *  Created on: Oct 25, 2019
 *      Author: jack
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdint.h>

#define SOURCE_FREQUENCY    20000000    // 20MHz
#define PWM0_FREQUENCY      50000       // 50kHz, DRV8886 f_pwm range is 0-100kHz
#define CALC_PERIOD(X)      (SOURCE_FREQUENCY / X)

/*  E N U M S   */
typedef enum {
    Forward = 0,
    Backward = 1
} eMotor_Direction;

typedef struct {
    uint32_t        base;
    uint8_t         pin;
} Motor_Pin_t;

typedef struct {
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t z_pos;
    uint8_t speed; // 0-100
} Motor_Instruction_t;

typedef struct {
    Motor_Pin_t     M1;
    Motor_Pin_t     M0;
    Motor_Pin_t     DIR;
    Motor_Pin_t     ENABLE;
    Motor_Pin_t     NSLEEP;
    Motor_Pin_t     NFAULT;
    Motor_Pin_t     STEP;
    uint32_t        PWM_Channel;
} Motor_t;

/*  T A S K S   */
void prv_Motor(void *pvParameters);

/*  F U N C T I O N S */
void init_motor(void);
void init_all_motors(void);

/*  M O T O R   P W M   */

void init_motor_pwm(Motor_t motor);
void change_motor_pwm_duty_cycle(Motor_t motor, uint8_t duty_cycle);

/*  M O T O R   G P I O   */

void init_motor_gpio(void);
void enable_motor(Motor_t motor);
void enable_motor(Motor_t motor);
void set_motor_to_sleep(void);


#endif /* MOTOR_CONTROL_H_ */
