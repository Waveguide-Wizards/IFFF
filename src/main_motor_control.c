/*
 * main_motor_control.c
 *
 *  Created on: Oct 26, 2019
 *      Author: jack
 */

/*  I N C L U D E S   */
#include <stdint.h>

/*  D R I V E R L I B   */

/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*  A P P L I C A T I O N    I N C L U D E S   */
#include "main_motor_control.h"
#include "x_motor_control.h"

/* P R I V A T E   V A R I A B L E S   */
static uint32_t x_steps;
static uint32_t y_steps;
static uint32_t z_steps;


/*  F U N C T I O N S   */

void initialize_motors(void) {
    init_x_motor();
//    init_y_motor();
//    init_z_motor();
}


/*  T A S K S   */

void prvPrint(void *pvParameters) {
    // schedule prvFindBounds
    // wait for task notification from prvFindBounds

    for( ;; ) {
        // fetch instruction(s) form queue
        // do instructions
    }
}

// determine number of steps possible in each direction
void prvFindBounds(void *pvParameters) {
    static TickType_t delay_time = pdMS_TO_TICKS(500);     // 0.5s

    for( ;; ) {
        // X
        // set in forward direction until bumper is hit
        vTaskDelay(delay_time);
        // set in backward direction until bumper is hit
        vTaskDelay(delay_time);

        // Y
        // set in forward direction until bumper is hit
        vTaskDelay(delay_time);
        // set in backward direction until bumper is hit
        vTaskDelay(delay_time);
        // Z
        // set in downward direction until bumper is hit
        vTaskDelay(delay_time);
        // set in upward direction until bumper is hit
        vTaskDelay(delay_time);

        // delete/suspend task using task handle
//        if( xHandle != NULL )
//        {
//            vTaskDelete( xHandle );
//        }
    }
}
