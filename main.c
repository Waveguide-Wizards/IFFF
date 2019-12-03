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
#include "motor_control.h"
#include "led.h"

/*  F R E E R T O S   H O O K S   */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vPreSleepProcessing( uint32_t ulExpectedIdleTime );
void vApplicationTickHook( void);
void *malloc( size_t xSize );


/*  G L O B A L S   */
QueueHandle_t motor_instruction_queue;

/* main.c */
void main(void)
{
    // set clock source to 16MHz external oscillator, use PLL and divide by 10 to get 20MHz
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    /* I N I T */
//    init_all_motors();

    /* Q U E U E S */
    motor_instruction_queue = xQueueCreate(10, sizeof(Motor_Instruction_t));

    /* T A S K S */
    BaseType_t BlinkyReturned = xTaskCreate(prvLED_Heartbeat, "HeartbeatLED", configMINIMAL_STACK_SIZE, (void *)NULL, 3, NULL);
    BaseType_t XMotorReturned = xTaskCreate(prv_Motor, "Motor Control", configMINIMAL_STACK_SIZE, (void *)NULL, 3, NULL); // PB6

    /* check that tasks were created successfully */
    configASSERT(BlinkyReturned == pdPASS);
    configASSERT(XMotorReturned == pdPASS);

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
