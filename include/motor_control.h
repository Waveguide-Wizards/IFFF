/*
 * motor_control.h
 *
 *  Created on: Oct 25, 2019
 *      Author: jack
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdint.h>

#define STEP_FULL           0U
#define STEP_16             1U
#define STEP_2              2U
#define STEP_4              3U
#define STEP_8              4U


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
    uint32_t        PWM_Block;
    uint32_t        PWM_Base;
    uint32_t        PWM_Channel;
    uint32_t        PWM_Pin_Map;
} Motor_t;

///* Motor structs, defined in motor_control.c */
//extern Motor_t x_motor;
//extern Motor_t y_motor;
//extern Motor_t z_motor;
//extern Motor_t ex_motor;

/*  T A S K S   */
void prv_Motor(void *pvParameters);

/*  F U N C T I O N S   */
void init_x_motor(void);
void init_y_motor(void);
//void init_z_motor(void);
void init_all_motors(void);

/*  M O T O R   P W M   */
void motor_init_x_pwm();
void motor_init_y_pwm();
//void motor_init_z_pwm();
//void motor_init_ex_pwm();
void motor_change_pwm_duty_cycle(Motor_t motor, uint8_t duty_cycle);

/*  X   M O T O R   G P I O   */
void motor_init_x_gpio(void);
void motor_init_y_gpio(void);
//void motor_init_z_gpio(void);
//void motor_init_ex_gpio(void);
void motor_enable(Motor_t motor);
void motor_disable(Motor_t motor);
void motor_set_to_sleep(Motor_t motor);
void motor_set_direction(Motor_t motor, eMotor_Direction direction);
void set_motor_step_size(Motor_t motor, uint8_t direction);

void PWM0IntHandler(void);


#endif /* MOTOR_CONTROL_H_ */
