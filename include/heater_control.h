/*
 * heater_control.h
 *
 *  Created on: Feb 7, 2020
 *      Author: jack
 */

#ifndef HEATER_CONTROL_H_
#define HEATER_CONTROL_H_

#define ADC_MAX_VALUE       4095    // 12-bit
#define ADC_MAX_VALUE_F     4095.0  // 12-bit floating point
#define VREF_MV             3300U   // 3.3V in millivolts
#define VREF_MV_F           3.3     // 3.3V

#include <stdint.h>

/*  T A S K S */
void prvExtruderHeaterControl(void * prvParameters);
void prvBedHeaterControl(void * prvParameters);

/*  A D C   */
void init_extruder_heater_adc(void);
void init_bed_heater_adc(void);
void ex_heater_get_adc(uint32_t * put_data);
void ex_bed_get_adc(uint32_t * put_data);
void init_extruder_heater_pwm(void);
void init_bed_heater_pwm(void);

/***    C O N V E R S I O N S   ***/
uint32_t adc_convert_to_mv(uint32_t adc_result);
float adc_convert_to_v(uint32_t adc_result);

#endif /* HEATER_CONTROL_H_ */
