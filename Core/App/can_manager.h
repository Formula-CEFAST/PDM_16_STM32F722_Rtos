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

// ===== CAN message struct for queue =====
#define MEGACANID 1520
typedef struct {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
} CAN_Message_t;


extern osMessageQueueId_t canRxQueue;


// ===== Functions =====

void CAN_Manager_Init(void);
void CANTaskApp(uint16_t valor);
void CAN_Send(uint32_t id, uint8_t *data, uint8_t len);

#endif /* APP_CAN_MANAGER_H_ */
