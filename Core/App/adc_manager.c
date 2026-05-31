/*
 * adc_manager.c
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */
#include "app_data.h"
#include "adc_manager.h"

extern ADC_HandleTypeDef hadc1;
#define K 0.0224719

#define ADC_CHANNEL_COUNT    13

// ===== DMA BUFFERS =====

uint16_t adc12_buf[8];
uint16_t adc34_buf[6];
uint16_t adc4_buf[1];
uint16_t adc5_buf[3];uint16_t adc3_buf[1];

uint16_t adcBuffer[ADC_CHANNEL_COUNT];

__IO uint32_t   aADCDualConvertedValues[4];         /* ADC dual mode interleaved conversion results (ADC master and ADC slave results concatenated on data register 32 bits of ADC master). */
__IO uint32_t   aADCDualConvertedValues2[2];         /* ADC dual mode interleaved conversion results (ADC master and ADC slave results concatenated on data register 32 bits of ADC master). */

__IO uint16_t   aADCxConvertedValues[4];    /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
__IO uint16_t   aADCyConvertedValues[4];    /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */

__IO uint16_t   aADCx2ConvertedValues[2];    /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
__IO uint16_t   aADCy2ConvertedValues[2];    /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */

__IO uint16_t aADCSingleEndedAdc5[1];
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
}

// ================= DMA CALLBACK =================

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // Signal ADC task when DMA completes a sequence
   // osSemaphoreRelease(adcSemHandle);
}


// ================= RAW TO VOLTAGE =================

float ADC_RawToVoltage(uint16_t raw)
{
    return (3.3f * raw) / 4095.0f;   // 12-bit ADC
}


// ================= ADC PROCESSING TASK =================

static uint8_t getMuxState(void)
{
    // Example — adapt to your GPIO names
	HAL_GPIO_WritePin(SEL0_GPIO_Port, SEL0_Pin, 1);
	HAL_GPIO_WritePin(SEL1_GPIO_Port, SEL1_Pin, 0);

    uint8_t s0 = HAL_GPIO_ReadPin(SEL0_GPIO_Port, SEL0_Pin);
    uint8_t s1 = HAL_GPIO_ReadPin(SEL1_GPIO_Port, SEL1_Pin);

    return (s1 << 1) | s0;
}


void ADCTask()
{
	for (int i = 0; i < 4; ++i) {
	    aADCxConvertedValues[i] = aADCDualConvertedValues[i] & 0x0000FFFF;

	    if (i < 3)
	        aADCyConvertedValues[i] = aADCDualConvertedValues[i] >> 16;
	}
	for (int i = 0; i < 2; ++i) {

		aADCx2ConvertedValues[i]=aADCDualConvertedValues2[i]&0x0000FFFF;
		aADCy2ConvertedValues[i]=aADCDualConvertedValues2[i]>>16;
	}
	uint16_t teste=(aADCyConvertedValues[0]*100*K);
	 adcData.smartswitch1_current[0]==teste;
	 adcData.smartswitch1_current[1]==(uint16_t)(K*aADCyConvertedValues[3] * 100);

	 if(flagCan<300){}//flagCan++;

}



//map the signals to proper variables
	  void ADC_ProcessData()
	  {
		  uint8_t mux = getMuxState();
	      float v;

	      /* ================= ADC1 ================= */

	      // smartswitch5 (4 channels via mux)
	      v = (aADCxConvertedValues[0]);
	      adcData.smartswitch5[mux] = v;

	      adcData.temperature = (aADCxConvertedValues[1]);
	      adcData.vbat        = (aADCxConvertedValues[2]);
	      adcData.vref        = (aADCxConvertedValues[3]);


	      /* ================= ADC2 ================= */

	      // smartswitch1 (mux based)
	      v = (aADCyConvertedValues[0]);
	      switch(mux)
	      {
	          case 0: adcData.smartswitch1_current[0] = v; break;
	          case 1: adcData.smartswitch1_current[1] = v; break;
	          case 2: adcData.smartswitch1_voltage    = v; break;
	          case 3: adcData.smartswitch1_temp       = v; break;
	      }

	      adcData.analogSpare1  = (aADCyConvertedValues[1]);
	      adcData.sensorCurrent= (aADCyConvertedValues[2]);

	      // smartswitch2 (mux based)
	      v = (aADCyConvertedValues[3]);
	      switch(mux)
	      {
	          case 0: adcData.smartswitch2_current[0] = v; break;
	          case 1: adcData.smartswitch2_current[1] = v; break;
	          case 2: adcData.smartswitch2_voltage    = v; break;
	          case 3: adcData.smartswitch2_temp       = v; break;
	      }


	      /* ================= ADC3 ================= */

	      v = (aADCx2ConvertedValues[0]);
	      switch(mux)
	      {
	          case 0: adcData.smartswitch3_current[0] = v; break;
	          case 1: adcData.smartswitch3_current[1] = v; break;
	          case 2: adcData.smartswitch3_voltage    = v; break;
	          case 3: adcData.smartswitch3_temp       = v; break;
	      }


	      /* ================= ADC4 ================= */

	      // smartswitch4
	      v = (aADCy2ConvertedValues[0]);
	      switch(mux)
	      {
	          case 0: adcData.smartswitch4_current[0] = v; break;
	          case 1: adcData.smartswitch4_current[1] = v; break;
	          case 2: adcData.smartswitch4_voltage    = v; break;
	          case 3: adcData.smartswitch4_temp       = v; break;
	      }

	      // smartswitch6 (4 channels via mux)
	      v = (aADCy2ConvertedValues[1]);
	      adcData.smartswitch6[mux] = v;


	      /* ================= ADC5 ================= */

	      adcData.analogSpare2 = (aADCSingleEndedAdc5[1]);
	  }



