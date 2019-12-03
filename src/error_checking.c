/*
 * error_checking.c
 *
 *  Created on: Nov 21, 2019
 *      Author: jack
 */


/*  I N C L U D E S   */
#include <stdbool.h>
#include <stdint.h>

/*  A P P L I C A T I O N   I N C L U D E S   */
#include "bsp.h"
#include "error_checking.h"

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


/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"


/*  G L O B A L   V A R I A B L E S   */
extern eState printer_state;

/*  T A S K S   */
void prv_ErrorCheck(void *pvParameters) {
    static TickType_t delay_time = pdMS_TO_TICKS(1000);     // 500ms
    for( ;; ) {
        if(printer_state == Error) {
            // TODO: LCD screen error
            // TODO: LED indicator error
        }
        vTaskDelay(delay_time);
    }
}
