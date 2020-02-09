/*
 * IFFF
 *  Waveguide Wizards
 *      Avery Anderson, Jack Danielski, Mike Fruge, Riley Hadjis, Mark Hinkle, Noa Margalit
 */

/*  I N C L U D E S   */
#include <stdint.h>
#include <stdbool.h>

/*  D R I V E R L I B   */
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*  A P P L I C A T I O N    I N C L U D E S   */
#include "bsp.h"
#include "bumpers.h"
#include "calibration.h"
#include "error_checking.h"
#include "heater_control.h"
#include "led.h"
#include "motor_control.h"


/*  F R E E R T O S   H O O K S   */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vPreSleepProcessing( uint32_t ulExpectedIdleTime );
void vApplicationTickHook( void);
void *malloc( size_t xSize );


/*  G L O B A L S   */
volatile eState printer_state;
QueueHandle_t motor_instruction_queue;

/*  T A S K   H A N D L E S   */
TaskHandle_t thConfig = NULL;
TaskHandle_t thBlinkyTask = NULL;
TaskHandle_t thCalibration = NULL;
TaskHandle_t thErrorTask = NULL;
TaskHandle_t thExtruderTask = NULL;
TaskHandle_t thExtruderHeaterTask = NULL;
TaskHandle_t thBedHeaterTask = NULL;
TaskHandle_t thMotorTask = NULL;


/*  C O N F I G   T A S K   */
void configTask(void * prvParameter) {
    /* create tasks */

    // Priority 1
    BaseType_t XMotorReturned = xTaskCreate(prv_Motor, "Motor Control", 500, (void *)NULL, 1, &thMotorTask);
    vTaskSuspend(thMotorTask);

    BaseType_t ExHeaterReturned = xTaskCreate(prvExtruderHeaterControl, "ExtruderHeater", 500, (void *)NULL, 1, &thExtruderHeaterTask);
    vTaskSuspend(thExtruderHeaterTask);

    BaseType_t BedHeaterReturned = xTaskCreate(prvBedHeaterControl, "BedHeater", 500, (void *)NULL, 1, &thBedHeaterTask);
    vTaskSuspend(thBedHeaterTask);

    BaseType_t ErrorCheckReturned = xTaskCreate(prv_ErrorCheck, "ErrorChecking", configMINIMAL_STACK_SIZE, (void *)NULL, 1, &thErrorTask);

    // Priority 2
    BaseType_t CalibrationReturned = xTaskCreate(prvCalibration, "Calibration", configMINIMAL_STACK_SIZE, (void *)NULL, 2, &thCalibration);
    vTaskSuspend(thCalibration);

    // Priority 3
    // Priority 4
    // Priority 5
    BaseType_t BlinkyReturned = xTaskCreate(prvLED_Heartbeat, "HeartbeatLED", configMINIMAL_STACK_SIZE, (void *)NULL, 5, &thBlinkyTask);

    /* check that tasks were created successfully */
    configASSERT(XMotorReturned == pdPASS);
    configASSERT(ExHeaterReturned == pdPASS);
    configASSERT(BedHeaterReturned == pdPASS);
    configASSERT(ErrorCheckReturned == pdPASS);
    configASSERT(BlinkyReturned == pdPASS);

#ifdef TEST_PREHEATING
    printer_state = Preheating;
    vTaskResume(thExtruderHeaterTask);
    vTaskResume(thBedHeaterTask);
#endif
#ifdef TEST_CALIBRATION
    printer_state = Calibration;
    vTaskResume(thCalibration);
#else
    printer_state = Idle;
#endif
    vTaskDelete(thConfig);
}

/*   --- M A I N ---   */
void main(void)
{
    // set clock source to 16MHz external oscillator, use PLL and divide by 10 to get 20MHz
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    /* I N I T */
    init_all_motors();
    init_bumper_gpio();

    /* Q U E U E S */
//    motor_instruction_queue = xQueueCreate(10, sizeof(Motor_Instruction_t));

    /* create first task */
    BaseType_t configReturned = xTaskCreate(configTask, "Config", configMINIMAL_STACK_SIZE, (void *)NULL, 1, &thConfig);
    configASSERT(configReturned == pdPASS);

    /* start scheduler */
    vTaskStartScheduler();
    for( ;; );
}


void vApplicationMallocFailedHook( void )
{
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    for( ;; ) {
        // sleep (stop the CPU clock) when the opportunity is given
    }
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

void vPreSleepProcessing( uint32_t ulExpectedIdleTime ) {

}

void vApplicationTickHook( void ) {
//    for( ;; );
}

void *malloc( size_t xSize ) {
    for( ;; );
}
