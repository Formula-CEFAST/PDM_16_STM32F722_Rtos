/*
 * app_data.h
 *
 *  Created on: Jan 25, 2026
 *      Author: diogo
 */

#ifndef APP_APP_DATA_H_
#define APP_APP_DATA_H_

#include "stm32f7xx_hal.h"
#include <stdint.h>
#include "cmsis_os.h"
#include "task.h"

// ========== ADC PROCESSED DATA STRUCT ==========

typedef struct {

    float smartswitch1_current[2];
    float smartswitch1_voltage;
    float smartswitch1_temp;

    float smartswitch2_current[2];
    float smartswitch2_voltage;
    float smartswitch2_temp;

    float smartswitch3_current[2];
    float smartswitch3_voltage;
    float smartswitch3_temp;

    float smartswitch4_current[2];
    float smartswitch4_voltage;
    float smartswitch4_temp;

    float smartswitch5[4];
    float smartswitch6[4];

    float temperature;
    float vbat;
    float vref;

    float analogSpare1;
    float analogSpare2;
    float sensorCurrent;

} ADC_Data_t;


// ========== GLOBAL VARIABLES ==========

extern ADC_Data_t adcData;

// CAN related globals
extern uint8_t canOutStatus[8];
extern uint8_t genericPWMDuty[8];
typedef enum {
    OUTPUT_MODE_CAN = 0,
    OUTPUT_MODE_GENERIC_PWM = 1
} output_mode_t;

extern output_mode_t outputMode[8];
extern uint8_t newAngle;
extern uint16_t flagCan;

// Other system values
extern uint16_t tensaoBatMega;



#endif /* APP_APP_DATA_H_ */
