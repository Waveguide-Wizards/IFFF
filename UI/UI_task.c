/*
 * UI_task.c
 *
 *  Created on: Feb 24, 2020
 *      Author: marka
 */


// File specific header
#include "UI_task.h"

//Yeak idk other includes
#include "bsp.h"

/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*  G L O B A L   V A R I A B L E S   */
extern eState printer_state;
extern TaskHandle_t thMotorTask;
extern TaskHandle_t thExtruderTask;
extern bool begin_motor_IT;

void prv_UI(void *pvParameters)
{
    static const TickType_t delay_time = pdMS_TO_TICKS( 100 );
    static uint32_t error_code = 0;
    static BaseType_t ret;

    //Initialize Variables
    UI_Init(SysCtlClockGet());

    // UI Main
    for(ever)
    {
        if(printer_state == Idle && begin_motor_IT == true)
        {
            begin_motor_IT = false;
            printer_state = Printing;
            xTaskNotify(thMotorTask, MUI_TEST_PROCEDURE, eSetBits);
        }
        else if(printer_state == Printing)
        {
            ret = xTaskNotifyWait(UI_CLEAR_BITS_ON_ENTRY, UI_CLEAR_BITS_ON_EXIT, &error_code, UI_NOTIFY_WAIT_TIME);
            if(ret == pdPASS)
            {
                UI_HandleErrors(error_code);
            }
        }

        //check for notifications
//        ret = xTaskNotifyWait(UI_CLEAR_BITS_ON_ENTRY, UI_CLEAR_BITS_ON_EXIT, &error_code, UI_NOTIFY_WAIT_TIME);
//        if(ret == pdPASS)
//        {
//            UI_HandleErrors(error_code);
//        }

        //continue normal UI operation
        WidgetMessageQueueProcess();
        vTaskDelay(delay_time);
    }
}

