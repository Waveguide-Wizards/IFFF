/*
 * error_checking.h
 *
 *  Created on: Nov 21, 2019
 *      Author: jack
 */

#ifndef ERROR_CHECKING_H_
#define ERROR_CHECKING_H_

#include <stdbool.h>

typedef struct {
    bool X_nFAULT;
    bool Y_nFAULT;
} Error_Sources_t;

void prv_ErrorCheck(void *pvParameters);


#endif /* ERROR_CHECKING_H_ */
