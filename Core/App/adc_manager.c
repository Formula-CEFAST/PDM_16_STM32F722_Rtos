/*
 * adc_manager.c
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */
#include "app_data.h"
#include "adc_manager.h"

extern ADC_HandleTypeDef hadc1;
#define K0 0.0224719
#define K1 0.0224719
#define K2 0.0224719
#define K3 0.0224719
#define K4 0.0224719

#define ADC_CHANNEL_COUNT    13

// ===== DMA BUFFERS =====


uint16_t adcBuffer[ADC_CHANNEL_COUNT];
uint8_t muxState=0;

// ================= START ALL ADCs =================

void ADC_Manager_Start()
{
    /* Calibracao ADC */
   // HAL_ADCEx_Calibration_Start(&hadc1);

    /* Start DMA */
    HAL_ADC_Start_DMA(&hadc1,
                      (uint32_t*)adcBuffer,
                      ADC_CHANNEL_COUNT);

    /* Start Timer Trigger */
    HAL_TIM_Base_Start(&htim8);


}

// ================= DMA CALLBACK =================

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // Signal ADC task when DMA completes a sequence
   // osSemaphoreRelease(adcSemHandle);
	ADC_ProcessData(muxState);
	SetNextMuxState();

}


// ================= RAW TO VOLTAGE =================

float ADC_RawToVoltage(uint16_t raw)
{
    return (3.3f * raw) / 4095.0f;   // 12-bit ADC
}


// ================= ADC PROCESSING TASK =================

void SetNextMuxState(void)
{
    muxState = (muxState + 1) % 4;

    switch(muxState)
    {
        case 0:
            HAL_GPIO_WritePin(SEL0_GPIO_Port, SEL0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEL1_GPIO_Port, SEL1_Pin, GPIO_PIN_RESET);
            break;

        case 1:
            HAL_GPIO_WritePin(SEL0_GPIO_Port, SEL0_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(SEL1_GPIO_Port, SEL1_Pin, GPIO_PIN_RESET);
            break;

        case 2:
            HAL_GPIO_WritePin(SEL0_GPIO_Port, SEL0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(SEL1_GPIO_Port, SEL1_Pin, GPIO_PIN_SET);
            break;

    	case 3:

    		HAL_GPIO_WritePin(SEL0_GPIO_Port, SEL0_Pin, GPIO_PIN_SET);
    		HAL_GPIO_WritePin(SEL1_GPIO_Port, SEL1_Pin, GPIO_PIN_SET);
    	break;

    }
}




//map the signals to proper variables
void ADC_ProcessData(uint8_t muxState)
{
    adcData.analogSpare3 = ADC_RawToVoltage(adcBuffer[0]);
    adcData.analogSpare2 = ADC_RawToVoltage(adcBuffer[3]);
    adcData.analogSpare1 = ADC_RawToVoltage(adcBuffer[2]);
    adcData.analogSpare4 = ADC_RawToVoltage(adcBuffer[1]);

    switch(muxState)
    {
        case 0:
            adcData.smartswitch1_current[0] = adcBuffer[4];
            adcData.smartswitch2_current[0] = adcBuffer[5];
            adcData.smartswitch4_current[0] = adcBuffer[6];
            adcData.smartswitch5[0]         = adcBuffer[7];
            adcData.smartswitch6[0]         = adcBuffer[8];
            adcData.sensorCurrent           = adcBuffer[9];
            adcData.smartswitch3_current[0] = adcBuffer[10];
            break;

        case 1:
            adcData.smartswitch1_current[1] = adcBuffer[4];
            adcData.smartswitch2_current[1] = adcBuffer[5];
            adcData.smartswitch4_current[1] = adcBuffer[6];
            adcData.smartswitch5[1]         = adcBuffer[7];
            adcData.smartswitch6[1]         = adcBuffer[8];
            adcData.smartswitch3_current[1] = adcBuffer[10];
            break;

        case 2:
            adcData.smartswitch1_voltage = adcBuffer[4];
            adcData.smartswitch2_voltage = adcBuffer[5];
            adcData.smartswitch4_voltage = adcBuffer[6];
            adcData.smartswitch5[2]      = adcBuffer[7];
            adcData.smartswitch6[2]      = adcBuffer[8];
            adcData.smartswitch3_voltage = adcBuffer[10];
            break;

        case 3:
            adcData.smartswitch1_temp = adcBuffer[4];
            adcData.smartswitch2_temp = adcBuffer[5];
            adcData.smartswitch4_temp = adcBuffer[6];
            adcData.smartswitch5[3]   = adcBuffer[7];
            adcData.smartswitch6[3]   = adcBuffer[8];
            adcData.smartswitch3_temp = adcBuffer[10];
            break;
    }
}



