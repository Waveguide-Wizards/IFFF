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

void prv_UI(void *pvParameters)
{
    const TickType_t delay_time = pdMS_TO_TICKS( 100 );

    //Initialize Variables
    UI_Init(SysCtlClockGet());

    // UI Main
    for(ever)
    {
        WidgetMessageQueueProcess();
        vTaskDelay(delay_time);
    }
}

