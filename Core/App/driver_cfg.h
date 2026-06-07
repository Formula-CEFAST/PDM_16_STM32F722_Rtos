/*
 * driver_cfg.h
 *
 *  Created on: 1 de fev. de 2026
 *      Author: diogo
 */

#ifndef APP_DRIVER_CFG_H_
#define APP_DRIVER_CFG_H_

#define MAX_INDICE_DRIVERS 8

#define MAX_INDICE_SS 16

#define NO_CANBUS_ACTIVATION -1





#include "app_data.h"
#include "adc.h"
#include "tim.h"
#include "stdbool.h"



typedef enum{
	RETRY_IDLE,
	RETRY_WAIT,
	RETRY_TRYING,
	RETRY_SUCESS,
	RETRY_FAIL
}retry_state_t;

typedef struct
{
	uint8_t indice_can_out;
	uint8_t generic_pwm_index;
	bool output_enable;
	bool force_zero;
	bool error_flag;
	bool on;
	bool soft_start;
	uint8_t retry_count;
	uint8_t max_retry_count;
	float max_current;
	uint8_t counterPeriod; //never used duty over 8 bytes so capped the variable
	TIM_HandleTypeDef *TIM_Handle;
	uint8_t TIM_Channel;
	float* current_sensor_value;//pointer to where the current is stored after conversion
	retry_state_t state_retry;
	uint8_t current_duty;
	uint8_t max_time_overcurrent;
	uint16_t timer_wait_overcurrent;
    // ---- software timers ----
    uint32_t overcurrent_tick;
    uint32_t retry_tick;


	// Self-check configuration
	bool self_check_enabled;
	bool self_check_running;
	bool self_check_done;
	bool self_check_result;
	uint32_t self_check_start_tick;
	float self_check_pressure_start;
	float self_check_current_start;
	uint8_t self_check_drive_percent; // percent to drive during self-check
	uint16_t self_check_ramp_ms; // time to ramp from 0 to drive percent
	uint16_t self_check_hold_ms; // time to hold at drive percent before evaluation
	float self_check_pressure_threshold; // bar
	float self_check_current_threshold; // if >= 65535 then ignore current check
	}driver_CfgType;


extern driver_CfgType driver_CfgParam[MAX_INDICE_DRIVERS];

#endif /* APP_DRIVER_CFG_H_ */
