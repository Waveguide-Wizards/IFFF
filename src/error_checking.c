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
extern TaskHandle_t xBlinkyTask;
extern TaskHandle_t xErrorTask;

extern eState printer_state;

Error_Sources_t error_source = {
                                .X_nFAULT = false,
                                .Y_nFAULT =false
};

/*  T A S K S   */
void prv_ErrorCheck(void *pvParameters) {
    static TickType_t delay_time = pdMS_TO_TICKS(1000);
    static TickType_t flash_delay_time = pdMS_TO_TICKS(200);
    static TickType_t use_delay_time;
    use_delay_time = delay_time;
    for( ;; ) {
       if(printer_state == Error) {
           vTaskSuspend( xBlinkyTask );     // Stop the Blinky task from controlling LED
           // TODO: LCD screen error
           // LED indicator error
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
           vTaskDelay(use_delay_time);
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
           vTaskDelay(use_delay_time);
       }
       else {   // Check nFAULT pins, if any are low, set printer_state = Error
            uint32_t fault_status = 0;

            // Check X Motor
            fault_status = GPIOPinRead(X_NFAULT_PORT, X_NFAULT_PIN);
            if(!(fault_status & X_NFAULT_PIN)) {
                printer_state = Error;      // X Motor has a Fault
                use_delay_time = flash_delay_time;
            }

            // Check Y Motor
            fault_status = GPIOPinRead(Y_NFAULT_PORT, Y_NFAULT_PIN);
            if(!(fault_status & Y_NFAULT_PIN)) {
                printer_state = Error;      // Y Motor has a Fault
                use_delay_time = flash_delay_time;
            }
            vTaskDelay(use_delay_time);
       }
    }
}
