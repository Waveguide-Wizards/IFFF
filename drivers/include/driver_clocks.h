/*
 * clocks.h
 *
 *  Created on: Oct 5, 2019
 *      Author: jack
 */

#ifndef DRIVERS_INCLUDE_DRIVER_CLOCKS_H_
#define DRIVERS_INCLUDE_DRIVER_CLOCKS_H_

/* set the system clock divider to 10 (200MHz PLL / 10 = 20MHz)
 * Main Osc. clock source is 16MHz
 */
void configure_pll_clock_source(void);

#endif /* DRIVERS_INCLUDE_DRIVER_CLOCKS_H_ */
