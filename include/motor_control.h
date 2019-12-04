/*
 * motor_control.h
 *
 *  Created on: Oct 25, 2019
 *      Author: jack
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdint.h>

/* STEP SIZE TO PASS INTO motor_set_step_size */
#define STEP_FULL           0U
#define STEP_16             1U
#define STEP_2              2U
#define STEP_4              3U
#define STEP_8              4U

#define SOURCE_FREQUENCY    20000000        // 20MHz
#define PWM_FREQUENCY       5000       // 5kHz, DRV8886 f_pwm range is 0-100kHz
#define CALC_PERIOD(X)          (SOURCE_FREQUENCY / X)

//STEP CONVERSION PARAMETERS
//Steps in the motor 
#define STEPS_PER_ROTATION (200)
#define SELECTED_MICROSTEP ( 16)

//Distance per full revolution in micrometeres (780 mm => 780 000 um) (Hoefully wrong?!?!?!?)
#define DIST_PER_REV       (780000)

//Functional relationships:
#define DIST_PER_USTEP	   ((DIST_PER_REV/(STEPS_PER_ROTATION*SELECTED_MICROSTEP)))
#define USTEP_PER_DIST	   (1/DIST_PER_USTEP)

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
    uint32_t x_pos;
    uint32_t y_pos;
    uint32_t z_pos;
    uint32_t extruder_pos;
    uint8_t speed; // 0-100
} Motor_Instruction_t;

typedef struct {
    Motor_Pin_t         M1;
    Motor_Pin_t         M0;
    Motor_Pin_t         DIR;
    Motor_Pin_t         ENABLE;
    Motor_Pin_t         NSLEEP;
    Motor_Pin_t         NFAULT;
    Motor_Pin_t         STEP;
    uint32_t            PWM_Block;
    uint32_t            PWM_Base;
    uint32_t            PWM_Channel;
    uint32_t            PWM_Pin_Map;
    uint32_t            position;
    eMotor_Direction    direction;
} Motor_t;

//Struct to indicate the current status of each of the motor tasks
typedef struct {
    uint8_t x_done;
    uint8_t y_done;
    uint8_t z_done;
} Motor_Status_t;

/*  T A S K S   */
void prv_Motor(void *pvParameters);

/*  F U N C T I O N S   */
void find_direction(uint32_t instruction, Motor_t motor);
uint32_t steps_to_dist(uint32_t stepCount);
uint32_t dist_to_steps(uint32_t distance);
void init_x_motor(void);
void init_y_motor(void);
void init_z_motor(void);
void init_all_motors(void);
void init_motor_status(uint8_t x_init_status,uint8_t y_init_status,uint8_t z_init_status);

/*  M O T O R   P W M   */
void motor_init_x_pwm();
void motor_init_y_pwm();
void motor_init_z_pwm();
void motor_init_ex_pwm();
void motor_change_pwm_duty_cycle(Motor_t motor, uint8_t duty_cycle);
void motor_x_start(uint32_t distance, uint32_t direction);
uint32_t motor_steps_to_dist(uint32_t stepCount);
uint32_t motor_dist_to_steps(uint32_t distance);

/*  X   M O T O R   G P I O   */
void motor_init_x_gpio(void);
void motor_init_y_gpio(void);
void motor_init_z_gpio(void);
void motor_init_ex_gpio(void);

/* C O N F I G U R A T I O N */
void motor_enable(Motor_t motor);
void motor_disable(Motor_t motor);
void motor_set_to_sleep(Motor_t motor);
void motor_set_direction(Motor_t motor, eMotor_Direction direction);
void motor_set_step_size(Motor_t motor, uint8_t direction);
uint8_t update_x_status();
uint8_t update_y_status();
uint8_t update_z_status();

void PWM0IntHandler(void);

#endif /* MOTOR_CONTROL_H_ */
