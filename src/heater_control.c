/*
 * heater_control.c
 *
 *  Created on: Feb 7, 2020
 *      Author: jack
 */

/*  I N C L U D E S   */
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/*  A P P L I C A T I O N   I N C L U D E S   */
#include "bsp.h"
#include "heater_control.h"
#include "pid.h"

/*  D R I V E R   L I B   */
//#include "tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_adc.h"


/*  F R E E R T O S   I N C L U D E S   */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*  P R I V A T E   V A R I A B L E S   */
static PID_t extruder_heater_pid;
static PID_t bed_heater_pid;

/*  T A S K S */
void prvExtruderHeaterControl(void * prvParameters) {
    uint8_t delay_time_ms = 10;  // 10ms intervals
    TickType_t delay_time = pdMS_TO_TICKS( delay_time_ms );
    PID_init(&extruder_heater_pid, 3.2, 2.1, 2.3, 1.250, delay_time_ms);   // initialize PID controller

    // local variables
    static uint32_t adc_value = 0;
    static float ex_temperature_v = 0.0;

    init_extruder_heater_adc();
//    init_extruder_heater_pwm();

    for( ;; ) {
        // 1. Get latest ADC reading
        ex_heater_get_adc(&adc_value);

        // 2. Convert to voltage
        ex_temperature_v = adc_convert_to_v(adc_value);

        // 3-4. Calculate PID and Change PWM duty cycle based on PID calculation
        float ex_dc = PID_calculate(&extruder_heater_pid, ex_temperature_v);
        // change_pwm_duty_cycle(PWM_BASE, PID_calculate(&extruder_heater_pid, ex_temperature_v));
        vTaskDelay(delay_time);
    }
}

void prvBedHeaterControl(void * prvParameters) {
    TickType_t delay_time = pdMS_TO_TICKS( 10 );                // 10ms intervals
    PID_init(&bed_heater_pid, 3.2, 2.1, 2.3, 1.250, 10);        // initialize PID controller

    // local variables
    static uint32_t adc_value = 0;
    static float bed_temperature_v = 0.0;

    init_bed_heater_adc();
//    init_bed_heater_pwm();

    for( ;; ) {
        // 1. Get latest ADC reading
        ex_bed_get_adc(&adc_value);

        // 2. Convert to voltage
        bed_temperature_v = adc_convert_to_v(adc_value);

        // 3-4. Calculate PID and Change PWM duty cycle based on PID calculation
        float bed_dc = PID_calculate(&bed_heater_pid, bed_temperature_v);
        //  change_pwm_duty_cycle(PWM_BASE, PID_calculate(&bed_heater_pid, bed_temperature_v));
        vTaskDelay(delay_time);
    }
}

/*  A D C   */
void init_extruder_heater_adc(void) {
    /* Configure GPIO pin */
    GPIOPinTypeADC(EX_HEATER_ADC_PORT, EX_HEATER_ADC_PIN);

    /* Configure ADC peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER, 0,
                             EX_HEATER_ADC_CHANNEL | ADC_CTL_END);
    ADCSequenceEnable(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER);

    ADCIntClear(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER);
}

void init_bed_heater_adc(void) {
    /* Configure GPIO pin */
    GPIOPinTypeADC(EX_BED_ADC_PORT, EX_BED_ADC_PIN);

    /* Configure ADC peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(EX_BED_ADC_BASE, EX_HEATER_ADC_SEQUENCER, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(EX_BED_ADC_BASE, EX_HEATER_ADC_SEQUENCER, 0,
                             EX_BED_ADC_CHANNEL | ADC_CTL_END);

    ADCSequenceEnable(EX_BED_ADC_BASE, EX_HEATER_ADC_SEQUENCER);
    ADCIntClear(EX_BED_ADC_BASE, EX_HEATER_ADC_SEQUENCER);
}

void ex_heater_get_adc(uint32_t * put_data) {
    ADCProcessorTrigger(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER);
    while(!ADCIntStatus(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER, false));
    ADCIntClear(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER);
    ADCSequenceDataGet(EX_HEATER_ADC_BASE, EX_HEATER_ADC_SEQUENCER, put_data);
}

void ex_bed_get_adc(uint32_t * put_data) {
    ADCProcessorTrigger(EX_BED_ADC_BASE, EX_BED_ADC_SEQUENCER);
    while(!ADCIntStatus(EX_BED_ADC_BASE, EX_BED_ADC_SEQUENCER, false));
    ADCIntClear(EX_BED_ADC_BASE, EX_BED_ADC_SEQUENCER);
    ADCSequenceDataGet(EX_BED_ADC_BASE, EX_BED_ADC_SEQUENCER, put_data);
}

/*  P W M */
void init_extruder_heater_pwm(void) {
    return;
}

void init_bed_heater_pwm(void) {
    return;
}

/*   C O N V E R S I O N S   */
/**
 * @brief Converts the ADC result to millivolts
 *
 * @details Multiplies the ADC results by the number of mV
 *      in VREF and then divides by the maximum value of the ADC.
 *
 * @return Value in mV
 */
uint32_t adc_convert_to_mv(uint32_t adc_result) {
    return ((VREF_MV * adc_result) / ADC_MAX_VALUE);
}

/**
 * @brief Converts the ADC result to volts
 *
 * @details Multiplies the ADC results by the value of VREF
 *      and then divides by the maximum value of the ADC.
 *
 * @return Value in Volts
 */
float adc_convert_to_v(uint32_t adc_result) {
    return (((float)adc_result * VREF_MV_F) / ADC_MAX_VALUE_F);
}

