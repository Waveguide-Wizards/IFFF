/*
 * bsp.h
 *
 *  Created on: Nov 19, 2019
 *      Author: jack
 */

#ifndef BSP_H_
#define BSP_H_

/*
 * TODO: Change these to real pins and ports
 */

/*  X - MOTOR   */
#define X_MOTOR_GPIO_BASE       GPIO_PORTB_BASE

#define X_M1_PORT       GPIO_PORTB_BASE
#define X_M1_PIN        GPIO_PIN_0      // output
#define X_M1            GPIO_PIN_0      // output
#define X_M0            GPIO_PIN_1      // output
#define X_DIR           GPIO_PIN_2      // output
#define X_ENABLE        GPIO_PIN_3      // output
#define X_NSLEEP        GPIO_PIN_4      // output
#define X_NFAULT        GPIO_PIN_5      // input
#define X_STEP          GPIO_PIN_6      // output


/*  Y - MOTOR   */
#define Y_MOTOR_GPIO_BASE       GPIO_PORTB_BASE

#define Y_M1_PORT       GPIO_PORTB_BASE
#define Y_M1_PIN        GPIO_PIN_0      // output
#define Y_M1            GPIO_PIN_0      // output
#define Y_M0            GPIO_PIN_1      // output
#define Y_DIR           GPIO_PIN_2      // output
#define Y_ENABLE        GPIO_PIN_3      // output
#define Y_NSLEEP        GPIO_PIN_4      // output
#define Y_NFAULT        GPIO_PIN_5      // input
#define Y_STEP          GPIO_PIN_6      // output

/*  Z - MOTOR   */
#define Z_MOTOR_GPIO_BASE       GPIO_PORTB_BASE

#define Z_M1_PORT       GPIO_PORTB_BASE
#define Z_M1_PIN        GPIO_PIN_0      // output
#define Z_M1            GPIO_PIN_0      // output
#define Z_M0            GPIO_PIN_1      // output
#define Z_DIR           GPIO_PIN_2      // output
#define Z_ENABLE        GPIO_PIN_3      // output
#define Z_NSLEEP        GPIO_PIN_4      // output
#define Z_NFAULT        GPIO_PIN_5      // input
#define Z_STEP          GPIO_PIN_6      // output

#endif /* BSP_H_ */
