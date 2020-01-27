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
extern eState printer_state;
extern TaskHandle_t xErrorTask;

//bool error_list[NUM_ERROR_SOURCES];
////uint8_t error_count = 0;
//static Error_CircBuf_t error_buffer;
//
///*  T A S K S   */
//void prv_ErrorCheck(void *pvParameters) {
////    error_circ_init();
//    error_list_init();
//    for( ;; ) {
//        // wait for task notification
//        ulTaskNotifyTake(pdTrue, portMAX_DELAY);
////        int i;
////        for(i = 0; i < error_buffer.size; i++) {
////
////        }
//
//    }
//}

//void error_list_init(void) {
//    int i;
//    for(i = 0; i < NUM_ERROR_SOURCES; i++) {
//        error_list[i] = false;
//    }
//}

//void add_error_to_list(eError_Source error) {
//    error_list[error] = true;
//}
//
//void delete_error_from_list(eError_Source error) {
//    error_list[error] = false;
//}
//
//void get_error_from_id(eError_Source error) {
//
//}

