/*
 * clocks.c
 *
 *  Created on: Oct 4, 2019
 *      Author: jack
 */


#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

/* set the system clock divider to 10 (200MHz PLL / 10 = 20MHz)
 * Main Osc. clock source is 16MHz
 * This is a TM4C123 device so use SysCtlClockSet()
 */
void configure_pll_clock_source(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}
