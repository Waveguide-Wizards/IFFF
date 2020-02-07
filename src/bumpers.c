/*
<<<<<<< HEAD
 * bumpers.c
 *
 * Created on: Jan 9, 2020
 *      Author: Avery
 */


#include "bumpers.h"
#include "bsp.h"
#include "error_checking.h"
#include "motor_control.h"

#include "FreeRTOS.h"
#include "task.h"

#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

extern eState printer_state;
extern TaskHandle_t xErrorTask;


void init_bumper_gpio(void){
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //Initialize the X, Y and Z bumper input pins consecutively
    GPIOPinTypeGPIOInput(X_BUMPER_PORT, X_BUMPER_PIN);
    GPIOPinTypeGPIOInput(Y_BUMPER_PORT, Y_BUMPER_PIN);
    GPIOPinTypeGPIOInput(Z_BUMPER_PORT, Z_BUMPER_PIN);

    //Set all pins to falling edge interrupts
    GPIOIntTypeSet(X_BUMPER_PORT, X_BUMPER_PIN,GPIO_FALLING_EDGE);
    GPIOIntTypeSet(Y_BUMPER_PORT, Y_BUMPER_PIN,GPIO_RISING_EDGE);
    GPIOIntTypeSet(Z_BUMPER_PORT, Z_BUMPER_PIN,GPIO_FALLING_EDGE);

    //Enable all of the interrupts
    GPIOIntEnable(X_BUMPER_PORT, X_BUMPER_PIN);//Pin Probably needs to be swapped out.
    GPIOIntEnable(Y_BUMPER_PORT, Y_BUMPER_PIN);//Pin Probably needs to be swapped out.
    GPIOIntEnable(Z_BUMPER_PORT, Z_BUMPER_PIN);//Pin Probably needs to be swapped out.

    IntEnable(INT_GPIOA);
}

void GPIO_A_IntHandler(void) {
    // Read the flags
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    configASSERT( xErrorTask != NULL );

    uint32_t flags = GPIOIntStatus(GPIO_PORTA_BASE,1);

    /* Disable all motors */
    emergency_disable_motors();

    if(flags && X_BUMPER_PIN){
        add_error_to_list(X_Bumper);
    }
    else if(flags && Y_BUMPER_PIN){
        add_error_to_list(Y_Bumper);
    }
    else if(flags && Z_BUMPER_PIN){
        add_error_to_list(Z_Bumper);
    }

    vTaskNotifyGiveFromISR( xErrorTask, &xHigherPriorityTaskWoken );
    GPIOIntClear(GPIO_PORTA_BASE, flags);
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
