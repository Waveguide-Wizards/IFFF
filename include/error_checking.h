/*
 * error_checking.h
 *
 *  Created on: Nov 21, 2019
 *      Author: jack
 */

#ifndef ERROR_CHECKING_H_
#define ERROR_CHECKING_H_

#define NUM_ERROR_SOURCES       9

typedef enum {
    X_nFAULT = 0,
    Y_nFAULT,
    Z_nFAULT,
    Ex_nFAULT,
    X_Bumper,
    Y_Bumper,
    Z_Bumper,
    PWR_eFuse,  // redundant if there is only 24V eFuse
    Ex_Heater,
    Bed_Heater,
    USB,
    SD_Card,
}eError_Source;

typedef struct {
    eError_Source * cbuf;
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t size;
    int max_size;
} Error_CircBuf_t;

void prv_ErrorCheck(void *pvParameters);

#endif /* ERROR_CHECKING_H_ */
