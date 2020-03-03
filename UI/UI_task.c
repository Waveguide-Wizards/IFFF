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
extern TaskHandle_t thMemoryTask;

extern bool begin_usb_connect;
extern bool begin_file_transfer;

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
        // signal uC to connect to USB
        if(printer_state == Idle && begin_usb_connect == true)
        {
            begin_usb_connect = false;
            printer_state = Storage_Device_Inserted;
            xTaskNotifyGive(thMemoryTask);
        }

        // wait for notification that device is connected to USB device
        else if(printer_state == Storage_Device_Inserted)
        {
            // TODO change this to a message queue Notify Wait
            ret = xTaskNotifyWait(UI_CLEAR_BITS_ON_ENTRY, UI_CLEAR_BITS_ON_EXIT, &error_code, UI_NOTIFY_WAIT_TIME);
            if(ret == pdPASS)
            {
                printer_state = Select_Print;
            }
        }
        // signal uC to begin memory transfer
        else if(printer_state == Select_Print && begin_file_transfer == true)
        {
            begin_file_transfer = false;
            // TODO change this so it sends filename
            xTaskNotifyGive(thMemoryTask);
        }
        // wait for notification that transfer is complete
        else if(printer_state == USB_Transfer)
        {
            ret = xTaskNotifyWait(UI_CLEAR_BITS_ON_ENTRY, UI_CLEAR_BITS_ON_EXIT, &error_code, UI_NOTIFY_WAIT_TIME);
            if(ret == pdPASS)
            {
                UI_MemTestComplete(error_code);
            }
        }

        //continue normal UI operation
        WidgetMessageQueueProcess();
        vTaskDelay(delay_time);
    }
}

