
#include "bsp.h"

/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "motorUITest.h"



/* G L O B A L S */
extern eState printer_state;

extern TaskHandle_t thMotorTask;


void prv_MotorUITest(void  *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 10000 );  // TODO: switch to max port delay
    BaseType_t NotifReceived;
    uint32_t ulNotificationValue;
    uint8_t motorNotif = 0;


    while(1)
    {
//        NotifReceived = ulTaskNotifyWait( pdFALSE, ULONG_MAX, &ulNotificationValue, xMaxBlockTime );

        /* Send a notification to the UI to show the initial test screen */
//        xTaskNotify(thUI_Task, MUI_TEST_SCREEN, eSetBits);
        /* The task has been notified */
//        if( NotifReceived == pdPASS)
//        {
            // Sync. with UI thread to publish an update to the user and wait for a button press
            /*
            if(ulNotificationValue == Notif_from_UI)
            {
                // Send notification to motor task with value to run test
                xTaskNotify(thMotorTask, MUI_TEST_PROCEDURE, eSetBits);

            }   */

//            if( ulNotificationValue == )
//        }

        if(!motorNotif)
        {
            xTaskNotify(thMotorTask, MUI_TEST_PROCEDURE, eSetBits);
        }

        /* The task has not received a notification */

//        else{
//            vTaskDelay( xMaxBlockTime );
//        }
        vTaskDelay( xMaxBlockTime );




    }


}
