/*
 * motor_control.c
 *
 *  Created on: Oct 6, 2019
 *      Author: jack
 */

/*  D E V I C E   I N C L U D E S   */


/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"


/*  A P P L I C A T I O N   I N C L U D E S   */
#include "driver_motor_control.h"


/*  T A S K S   */
void prvX_Motor(void *pvParameters) {
    static TickType_t delay_time = pdMS_TO_TICKS(100);     // 1000ms
    static uint8_t dc = 0;  // 0-100%
    static uint8_t direction = 1;
    init_x_motor_pwm();
    enable_x_motor();

    for( ;; ) {
        if(direction == 0) { // count down
            dc--;
            if(dc == 0) {
                direction = 1;
            }
        }
        else if(direction == 1) {  // count up
            dc++;
            if(dc == 100) {
                direction = 0;
            }
        }

        change_x_motor_pwm_duty_cycle(dc);
        vTaskDelay(delay_time);
    }
}


/*  F U N C T I O N S   */
