/*
 * driver_cfg.c
 *
 *  Created on: 1 de fev. de 2026
 *      Author: diogo
 *      REVISOR
 */
#include "driver_cfg.h"

driver_CfgType driver_CfgParam[MAX_INDICE_DRIVERS] = {
    {// Driver 0 Configuration-->PDMES16 BUCK SAIDA 1
        .indice_can_out = NO_CANBUS_ACTIVATION,
        .generic_pwm_index = 3,
        .output_enable = true,
        .force_zero = false,
        .error_flag = false,
        .on = false,
        .soft_start = false,//servo dont need softstart
        .max_retry_count = 15,//good ammount until servo gets too hot
        .max_current = 6.0f,//stall current 45kg 9imod servo
        .counterPeriod = 100,
        .TIM_Handle = &htim3,
        .TIM_Channel = TIM_CHANNEL_1,
        .current_sensor_value=&adcData.smartswitch1_current[0],
        .state_retry = RETRY_IDLE,

        .current_duty = 0,
		.max_time_overcurrent=20,
		.retry_count=0



    },
    {// Driver 1 Configuration-->PDMES16 BUCK SAIDA 2
        .indice_can_out = NO_CANBUS_ACTIVATION,
        .generic_pwm_index = 3,
        .output_enable = true,
        .force_zero = true,
        .error_flag = false,
        .on = false,
        .soft_start = false,
        .max_retry_count = 15,
        .max_current = 6.0f,
        .counterPeriod = 100,
        .TIM_Handle = &htim12,
        .TIM_Channel = TIM_CHANNEL_2,
        .current_sensor_value=&adcData.smartswitch2_current[1],
        .state_retry = RETRY_IDLE,
        .current_duty= 0,
		.timer_wait_overcurrent=1000,
		.max_time_overcurrent=20,
		.retry_count=0

    },
    {// Driver 2 Configuration -->PDMES16 SAIDA 1 HIGH CURRENT,OPAMP3,VOPAMPCHANNEL HTIM2 CH1
        .indice_can_out = 0,
        .generic_pwm_index = 4,
        .output_enable = true,
        .force_zero = false,
        .error_flag = false,
        .on = false,
        .soft_start = false,
        .max_retry_count = 15,
        .max_current = 20.0f,//based on the maximun teoric current the driver can take
        .counterPeriod = 100,
        .TIM_Handle = &htim12,
        .TIM_Channel = TIM_CHANNEL_1,
        .current_sensor_value=&adcData.smartswitch3_current[0],
        .state_retry = RETRY_IDLE,
        .current_duty = 0,
		.timer_wait_overcurrent=1000,
		.max_time_overcurrent=20,
		.retry_count=0

    },
    {// Driver 3 Configuration -->PDMES16 SAIDA 2 HIGH CURRENT,OPAMP6,VOPAMPCHANNEL HTIM2 CH2
        .indice_can_out = 1,
        .generic_pwm_index = 2,
        .output_enable = true,
        .force_zero = false,
        .error_flag = false,
        .on = false,
        .soft_start = false,
        .max_retry_count = 15,
        .max_current = 20.0f,
        .counterPeriod = 100,
        .TIM_Handle = &htim3,
        .TIM_Channel = TIM_CHANNEL_2,
        .current_sensor_value=&adcData.smartswitch4_current[0],
        .state_retry = RETRY_IDLE,
        .current_duty = 0,
		.timer_wait_overcurrent=1000,
		.max_time_overcurrent=20,
		.retry_count=0

    },
    {// Driver 4 Configuration
         //-->PDMES16 Medium CURRENT,OPAMP1,VOPAMPCHANNEL_0 HTIM3 CH1
        .indice_can_out = 2,
        .generic_pwm_index = 5,
        .output_enable = true,
        .force_zero = false,
        .error_flag = false,
        .on = false,
        .soft_start = false,
        .max_retry_count = 15,
        .max_current = 10.0f,
        .counterPeriod = 100,
        .TIM_Handle = &htim3,
        .TIM_Channel = TIM_CHANNEL_3,
        .current_sensor_value=&adcData.smartswitch2_current[0],
        .state_retry = RETRY_IDLE,
        .current_duty = 0,
		.timer_wait_overcurrent=1000,
		.max_time_overcurrent=20,
		.retry_count=0

    },
    {// Driver 5 Configuration
         //-->PDMES16 Medium CURRENT,OPAMP1,VOPAMPCHANNEL_4 HTIM1 CH1
    		.indice_can_out = 3,
            .generic_pwm_index = 7,
            .output_enable = false,
            .force_zero = false,
			.error_flag = false,
			.on = false,
			.soft_start = false,
			.max_retry_count = 15,
			.max_current = 10.0f,
			.counterPeriod = 100,
			.TIM_Handle = &htim3,
			.TIM_Channel = TIM_CHANNEL_2,
			.current_sensor_value=&adcData.smartswitch2_current[1],
			.state_retry = RETRY_IDLE,
			.current_duty = 0,
			.timer_wait_overcurrent=1000,
			.max_time_overcurrent=20,
			.retry_count=0

    }
};

