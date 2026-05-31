/*
 * adc_manager.h
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */
#include "app_data.h"
#include "dma.h"
#include "adc.h"

#ifndef APP_ADC_MANAGER_H_
#define APP_ADC_MANAGER_H_
// ===== DMA RAW BUFFERS =====

extern uint16_t adc1_buf[4];
extern uint16_t adc2_buf[4];
extern uint16_t adc4_buf[1];
extern uint16_t adc5_buf[3];

// ===== RTOS SEMAPHORE (CREATED BY CUBEMX) =====

extern osSemaphoreId_t adcSemHandle;

// ===== PUBLIC FUNCTIONS =====

void ADC_Manager_Start();
void ADCTask();
void ADC_ProcessData(void);

// Utility
float ADC_RawToVoltage(uint16_t raw);


#endif /* APP_ADC_MANAGER_H_ */
