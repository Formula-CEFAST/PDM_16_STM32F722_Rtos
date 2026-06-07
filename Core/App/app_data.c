/*
 * app_data.c
 *
 *  Created on: Jan 25, 2026
 *      Author: diogo
 */


#include "app_data.h"

// ========== DEFINE GLOBALS ==========

ADC_Data_t adcData = {0};

uint8_t canOutStatus[8] = {0};
uint8_t genericPWMDuty[8] = {0};
uint16_t flagCan=0;
uint8_t genericPIDDuty[8]={0};
output_mode_t outputMode[8] = {    OUTPUT_MODE_GENERIC_PWM,
	    OUTPUT_MODE_GENERIC_PWM,
	    OUTPUT_MODE_GENERIC_PWM,
	    OUTPUT_MODE_GENERIC_PWM,
	    OUTPUT_MODE_GENERIC_PWM,
	    OUTPUT_MODE_GENERIC_PWM,
	    OUTPUT_MODE_GENERIC_PWM,
	    OUTPUT_MODE_GENERIC_PWM
};
uint8_t newAngle = 0;

uint16_t tensaoBatMega = 0;
uint16_t megacan13_raw = 0;
float megacan13_bar = 0.0f;
