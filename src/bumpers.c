/*
 * bumpers.c
 *
 * Created on: Jan 9, 2020
 *      Author: Avery
 */


#include "bumpers.h"
#include "bsp.h"
#include "error_checking.h"

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

// Interrupt handler for port A, currently used for the port of all three pins. May need to break into many handlers depending on pin mapping
void GPIO_A_IntHandler(void) {
    // Read the flags
    // If motor i hit the bumper.  Stop motor, move 10mm towards center for base position
    // Clear flags
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    configASSERT( xErrorTask != NULL );

    uint32_t flags = GPIOIntStatus(GPIO_PORTA_BASE,1);
    if(flags && X_BUMPER_PIN){
        add_error_to_list(X_Bumper);
//        printer_state = X_Bumper;

    }
    else if(flags && Y_BUMPER_PIN){
        printer_state = Y_Bumper_Error;
    }
    else if(flags && Z_BUMPER_PIN){
        printer_state = Z_Bumper_Error;
    }
    vTaskNotifyGiveFromISR( xErrorTask, &xHigherPriorityTaskWoken );
    GPIOIntClear(GPIO_PORTA_BASE, flags);

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}
