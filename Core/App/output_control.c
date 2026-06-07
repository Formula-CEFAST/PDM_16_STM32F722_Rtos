/*
 * output_control.c
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */

#include "tim.h"
#include "app_data.h"
#include "output_control.h"


// ===== PWM LIMIT =====

#define PWM_MAX 1000   // adjust to your ARR value
#define OC_MARGIN_HIGH   1.1f   // +10%
#define OC_MARGIN_LOW    0.9f   // -10%
#define PWM_RETRY_DUTY   (100 - 1)
#define SOFTSTART_STEP_PERCENT 1u
#define CAN_SOFTSTART_RAMP_MS 1000u

static uint32_t canSoftStartStartTick[MAX_INDICE_DRIVERS] = {0u};
static bool canSoftStartActive[MAX_INDICE_DRIVERS] = {false};

// ================= INIT =================

void OutputManager_Init(void)
{

    for (uint8_t i = 0u; i < MAX_INDICE_DRIVERS; ++i)
    {
        if (!driver_CfgParam[i].output_enable)
            continue;

        if (driver_CfgParam[i].TIM_Handle != NULL)
        {
            HAL_TIM_PWM_Start(driver_CfgParam[i].TIM_Handle,
                              driver_CfgParam[i].TIM_Channel);
        }
    }
    	Actuator_SelfCheck_Start(0);


}
static uint32_t DutyPercentToPwm(uint8_t duty_percent)
{
    if (duty_percent == 0u)
        return 0u;

    if (duty_percent > 100u)
        duty_percent = 100u;

    return 0u + ((uint32_t)(duty_percent - 1u) * (9999u)) / 99;
}
static uint8_t RampPercent(uint8_t current, uint8_t target)
{
    if (current < target)
    {
        uint8_t next = (uint8_t)(current + SOFTSTART_STEP_PERCENT);
        return (next > target) ? target : next;
    }

    return target;
}

static uint8_t SelfCheckRampPercent(const driver_CfgType *driver)
{
    uint8_t target = driver->self_check_drive_percent;
    uint32_t ramp_ms_total = driver->self_check_ramp_ms;

    if (target == 0u)
        return 0u;

    if (ramp_ms_total == 0u)
        return target;

    uint32_t elapsed = xTaskGetTickCount() - driver->self_check_start_tick;
    if (elapsed >= pdMS_TO_TICKS(ramp_ms_total))
        return target;

    uint32_t ramp_ms = elapsed * 1000u / configTICK_RATE_HZ;
    uint32_t ramp_percent = (uint32_t)target * ramp_ms / ramp_ms_total;

    if (ramp_percent > target)
        ramp_percent = target;

    return (uint8_t)ramp_percent;
}

static uint8_t CanSoftStartPercent(uint8_t ch, driver_CfgType *driver, uint8_t can_cmd)
{
    if (can_cmd == 0u)
    {
        canSoftStartActive[ch] = false;
        driver->current_duty = 0u;
        return 0u;
    }

    if (!driver->soft_start)
    {
        driver->current_duty = 100u;
        canSoftStartActive[ch] = false;
        return 100u;
    }

    if (!canSoftStartActive[ch])
    {
        canSoftStartActive[ch] = true;
        canSoftStartStartTick[ch] = xTaskGetTickCount();
        driver->current_duty = 0u;
    }

    uint32_t elapsed_ticks = xTaskGetTickCount() - canSoftStartStartTick[ch];
    uint32_t ramp_ticks = pdMS_TO_TICKS(CAN_SOFTSTART_RAMP_MS);

    if (ramp_ticks == 0u)
    {
        driver->current_duty = 100u;
        canSoftStartActive[ch] = false;
        return 100u;
    }

    if (elapsed_ticks >= ramp_ticks)
    {
        driver->current_duty = 100u;
        canSoftStartActive[ch] = false;
        return 100u;
    }

    uint32_t percent = (100u * elapsed_ticks) / ramp_ticks;
    if (percent > 100u)
        percent = 100u;

    driver->current_duty = (uint8_t)percent;
    return (uint8_t)percent;
}

static uint32_t GetCanSoftStartPwm(driver_CfgType *driver, uint8_t can_cmd)
{
    uint8_t duty_percent = CanSoftStartPercent((uint8_t)(driver - driver_CfgParam), driver, can_cmd);
    return DutyPercentToPwm(duty_percent);
}

uint16_t GetOutputPWM(uint8_t ch)
{
    if (ch >= MAX_INDICE_DRIVERS)
        return 0u;

    // If CAN output is on, use CAN soft start and ignore generic PWM
    if (canOutStatus[ch])
    {
        return (uint16_t)GetCanSoftStartPwm(&driver_CfgParam[ch], canOutStatus[ch]);
    }

    // If CAN is off, use generic PWM if in that mode
    if (outputMode[ch] == OUTPUT_MODE_GENERIC_PWM)
    {
        driver_CfgParam[ch].current_duty = genericPWMDuty[ch];
        return (uint16_t)DutyPercentToPwm(genericPWMDuty[ch]);
    }

    return (uint16_t)GetCanSoftStartPwm(&driver_CfgParam[ch], canOutStatus[ch]);
}

uint32_t GetDriverPWM(driver_CfgType *driver, uint32_t desired_pwm)
{
  /* if(driver->state_retry == RETRY_FAIL)
        return 0;

    if(driver->state_retry == RETRY_TRYING)
        return PWM_RETRY_DUTY;

    if(driver->state_retry == RETRY_WAIT)
        return desired_pwm;
*/
    // NORMAL
    return desired_pwm;
}
// NO overcurrent control just simple on and off
void Outputs_Update(void)
{
    uint32_t PW;

    for (uint8_t i = 0u; i < MAX_INDICE_DRIVERS; ++i)
    {
        if (!driver_CfgParam[i].output_enable)
            continue;

        if (driver_CfgParam[i].self_check_running)
        {
            PW = DutyPercentToPwm(SelfCheckRampPercent(&driver_CfgParam[i]));
        }
        else if (driver_CfgParam[i].force_zero)
        {
            PW = 0u;
        }
        else
        {
            PW = GetDriverPWM(&driver_CfgParam[i],
                              GetOutputPWM(driver_CfgParam[i].generic_pwm_index));
        }

        if (driver_CfgParam[i].TIM_Handle != NULL)
        {
            __HAL_TIM_SET_COMPARE(driver_CfgParam[i].TIM_Handle,
                                  driver_CfgParam[i].TIM_Channel,
                                  PW);
        }

        if (i == 2u)
        {
            HAL_GPIO_WritePin(BUCK_2_GPIO_Port,
                              BUCK_2_Pin,
                              (PW > 0u) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }

}

bool Actuator_SelfCheck_Start(uint8_t idx)
{
    if (idx >= MAX_INDICE_DRIVERS)
        return false;

    driver_CfgType *drv = &driver_CfgParam[idx];

    if (!drv->self_check_enabled)
        return true;

    drv->self_check_running = true;
    drv->self_check_done = false;
    drv->self_check_result = false;
    drv->self_check_start_tick = xTaskGetTickCount();
    drv->self_check_pressure_start = megacan13_bar;
    drv->self_check_current_start = drv->current_sensor_value ? *drv->current_sensor_value : 0.0f;

    return true;
}

void Actuator_SelfCheck_Service(void)
{
    for (uint8_t i = 0u; i < MAX_INDICE_DRIVERS; ++i)
    {
        driver_CfgType *drv = &driver_CfgParam[i];

        if (!drv->self_check_running)
            continue;

        uint32_t elapsed = xTaskGetTickCount() - drv->self_check_start_tick;
        uint32_t ramp_ticks = pdMS_TO_TICKS(drv->self_check_ramp_ms);
        uint32_t hold_ticks = pdMS_TO_TICKS(drv->self_check_hold_ms);

        if (elapsed < ramp_ticks)
            continue;

        if (elapsed < (ramp_ticks + hold_ticks))
            continue;

        float new_pressure = megacan13_bar;
        float new_current = drv->current_sensor_value ? *drv->current_sensor_value : 0.0f;

        bool pressure_ok = (new_pressure >= drv->self_check_pressure_threshold) &&
                           ((new_pressure - drv->self_check_pressure_start) > 0.0f);

        bool current_ok = true;
        if (drv->self_check_current_threshold < 65535.0f)
        {
            current_ok = (new_current >= drv->self_check_current_threshold);
        }

        drv->self_check_result = pressure_ok && current_ok;
        drv->self_check_done = true;
        drv->self_check_running = false;

        if (!drv->self_check_result)
        {
            drv->error_flag = true;
        }
    }
}
// ================= OUTPUT TASK =================


void OverCurrentCheck(driver_CfgType *driver)
{
    float current = *driver->current_sensor_value;

    // Detecta overcurrent SEM bloquear por estado
    if(driver->on && current > driver->max_current * OC_MARGIN_HIGH)
    {
        if(driver->state_retry == RETRY_IDLE ||
           driver->state_retry == RETRY_TRYING)
        {
            driver->overcurrent_tick = xTaskGetTickCount();
            driver->state_retry = RETRY_WAIT;
        }
    }

    // Cancela espera se corrente voltou ao normal
    if(driver->state_retry == RETRY_WAIT &&
       current < driver->max_current * OC_MARGIN_LOW)
    {
        driver->state_retry = RETRY_IDLE;
        driver->retry_count = 0;
    }
}
void retryDriver(driver_CfgType *driver)
{
    __HAL_TIM_SET_COMPARE(driver->TIM_Handle,
                          driver->TIM_Channel,
                          PWM_RETRY_DUTY);

    driver->on = true;
}

//checa se ja passou mais do que o tempo de overcurrent
void RetryCheck(driver_CfgType *driver)
{
    // Confirma overcurrent após tempo mínimo
    if(driver->state_retry == RETRY_WAIT)
    {
        if(xTaskGetTickCount() - driver->overcurrent_tick >
           pdMS_TO_TICKS(driver->max_time_overcurrent))
        {
            // Desliga driver
            __HAL_TIM_SET_COMPARE(driver->TIM_Handle,
                                  driver->TIM_Channel,
                                  0);
                                    for (uint8_t i = 0u; i < 8u; ++i)
                                        outputMode[i] = OUTPUT_MODE_GENERIC_PWM;


            driver->on = false;

            driver->retry_tick = xTaskGetTickCount();
            driver->state_retry = RETRY_TRYING;
        }
    }

    // Loop de retry
    if(driver->state_retry == RETRY_TRYING)
    {
        if(xTaskGetTickCount() - driver->retry_tick >
           pdMS_TO_TICKS(driver->timer_wait_overcurrent))
        {
            if(driver->retry_count < driver->max_retry_count)
            {
                driver->retry_count++;

                retryDriver(driver);

                // Reinicia timer de retry (fica em TRYING)
                driver->retry_tick = xTaskGetTickCount();
            }
            else
            {
                driver->state_retry = RETRY_FAIL;
            }
        }
    }
}
void OutputTask(void *argument)
{
    Actuator_SelfCheck_Service();
	Outputs_Update();




}
