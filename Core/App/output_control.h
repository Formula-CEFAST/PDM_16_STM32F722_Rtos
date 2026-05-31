/*
 * output_control.h
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */

#ifndef APP_OUTPUT_CONTROL_H_
#define APP_OUTPUT_CONTROL_H_

#include "app_data.h"
#include "driver_cfg.h"
// ===== FUNCTIONS =====

void OutputManager_Init(void);
void OutputTask(void *argument);

// Optional helpers
void Outputs_Update(void);
uint16_t GetOutputPWM(uint8_t ch);

#endif /* APP_OUTPUT_CONTROL_H_ */
