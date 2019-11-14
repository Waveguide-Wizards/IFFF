/* Need to set GPIO pins M1 and M0 to control the indexer inside the DRV888
 *      M1,`M0
 * {    0,  0   }   -   Full Step
 * {    0,  1,  }   -   1/16
 * {    1,  0   }   -   1/2
 * {    1,  1   }   -   1/4
 * {    0,  Z   }   -   1/8
 *
 */


/*
 * Indexer will advance on each rising edge of the STEP pin, so generate a true square wave
 */


/*      API: DRV888
 *
 *
 *      typedef struct DRV888_t
 *      {
 *          GPIO_Pin_t  M1
 *          GPIO_Pin_t  M0
 *          GPIO_Pin_t  ENABLE
 *          GPIO_Pin_t  DIR
 *          GPIO_Pin_t  nSLEEP
 *          PWM_Pin_t   STEP        // freq of STEP should be <100kHz
 *      } DRV888_t;
 *
 *      Initialize clocks and gpio pins for the structure
 *      MD_Init(struct MotorDriver, uint32_t freq)
 *              // Write to USPWMDIV bit in RCC register to select clock, and PWMDIV to prescale the clock
 *              // Initialize the GPIO pins that control the indexer and driver inputs
 *
 *
 *       Change the GPIO pin output to M1 and M0
 *      MD_ChangeStepSize(struct MotorDriver)
 *
 *       Set the output of the GPIO pin that controlls direction
 *      MD_SetDirection(struct MotorDriver, bool Direction)
 *
 *      Toggle Direction
 *      MD_ToggleDirection(struct MotorDriver)
 *
 *
 *
 *
 *
 */

#include<stdint.h>
#include<stdbool.h>


typedef struct GPIO_Pin_t
{
    uint32_t    Port;
    uint8_t     Pin;
} GPIO_Pin_t;



typedef struct DRV888_t
{
    GPIO_Pin_t     M1;
    GPIO_Pin_t     M0;
    GPIO_Pin_t     ENABLE;
    GPIO_Pin_t     DIR;
    GPIO_Pin_t     nSLEEP;
//    PWM_Pin_t   STEP;        // freq of STEP should be <100kHz
} DRV888_t;






typedef enum DRV888_Ret_t
{
    MD_SUCCESS      = 0,
    MD_FAIL         = 1,
    MD_DISCONNECT   = 2
} DRV888_Ret_t;


DRV888_Ret_t DRV888_Init(DRV888_t * driver);
DRV888_Ret_t DRV888_ChangeStepSize(DRV888_t * driver);
DRV888_Ret_t DRV888_SetDirection(DRV888_t * driver, bool direction);
DRV888_Ret_t DRV888_ToggleDirection(DRV888_t * driver);
