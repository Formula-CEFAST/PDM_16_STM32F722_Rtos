/*
 * can_manager.h
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */

#ifndef APP_CAN_MANAGER_H_
#define APP_CAN_MANAGER_H_

#include "can.h"
#include "app_data.h"
#include "cmsis_os.h"

// ===== CAN IDs =====

#define MEGACANID                  1520

#define CAN_ID_SW_CURRENT_CH0      100
#define CAN_ID_SW_CURRENT_CH1      101
#define CAN_ID_SW_VOLTAGE          102
#define CAN_ID_SW_TEMPERATURE      103
#define CAN_ID_SW5_DIAGNOSTICS     104
#define CAN_ID_SW6_DIAGNOSTICS     105
#define CAN_ID_MISC                106

// ===== CAN message struct for queue =====

typedef struct
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
} CAN_Message_t;

// ===== Globals =====

extern osMessageQueueId_t canRxQueue;

// ===== Functions =====

void CAN_Manager_Init(void);

void CANTaskApp(void);

void CAN_Send(uint32_t id,
              uint8_t *data,
              uint8_t len);

void SendCanFrame(uint16_t id,
                  uint16_t v1,
                  uint16_t v2,
                  uint16_t v3,
                  uint16_t v4);

#endif /* APP_CAN_MANAGER_H_ */
