/*
 * memory_task.c
 *
 *  Created on: Mar 2, 2020
 *      Author: margalitnoa
 */


#include "memory_task.h"
#incldue "usb_host"
#include "bsp.h"
//
///*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/*  G L O B A L   V A R I A B L E S   */
extern eState printer_state;
extern TaskHandle_t thMotorTask;
extern TaskHandle_t thExtruderTask;
extern TaskHandle_t thUITask;
extern TaskHandle_t thMemoryTask;


void prv_MEMORY(void *pvParameters){
    static TickType_t delay_time = pdMS_TO_TICKS( 100 );
    static uint32_t error_code = 0;

    // these init's might need to be moved to main if it crashes the OS
    usbInit();
    uartInit(); //only necessary for integration tests

    for(ever) {
        /* When in the Select_Printer state and a notification is RX'd,
         *  the available files should be shown on the UI */
        if(printer_state == Storage_Device_Inserted) {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    // wait until notification is RX'd

            /*
             * Get and display files here
             */
        }

        /* Intermediary state, so we do not receive two notifications
         *  in the same state */
        if(printer_state == Select_Print) {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    // wait until notification is RX'd
            printer_state = USB_Transfer;
        }

        /* Where the fun stuff happens ;) */
        if(printer_state == USB_Transfer) {
            // Suspends all tasks, prevents context switching
            vTaskSuspendAll();

            /*
             * Do the file transfer stuff here
             */

            // Once complete, change states
            printer_state == Printing;

            /* The operation is complete.  Set the scheduler back into the Active
            state. */
            if( !xTaskResumeAll() ) {
                vTaskSuspend(thMemoryTask);
                taskYIELD ();
            }
        }

        /* Don't let this task run anymore once the file has been transferred */
        if(printer_state == Printing) {
            vTaskSuspend(thMemoryTask);
        }
        vTaskDelay(delay_time);
    }

}
