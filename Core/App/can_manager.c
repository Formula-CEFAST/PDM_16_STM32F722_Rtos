/*
 * can_manager.c
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */


#include "can_manager.h"
#include "adc_manager.h"
// ===== External FDCAN handle from CubeMX =====

extern CAN_HandleTypeDef hcan1;
CAN_RxHeaderTypeDef rxHeader;
CAN_TxHeaderTypeDef txHeader;
uint8_t rxData[8];
  uint8_t txData[8];
osMessageQueueId_t canRxQueue;
uint32_t txMailBox;

// ================= INIT =================

void CAN_Manager_Init(void)
{
	  CAN_FilterTypeDef  sFilterConfig;

	      	sFilterConfig.FilterBank = 10;
	        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	        sFilterConfig.FilterIdHigh = 0x0000;
	        sFilterConfig.FilterIdLow = 0x0000;
	        sFilterConfig.FilterMaskIdHigh = 0x0000;
	        sFilterConfig.FilterMaskIdLow = 0x0000;
	        sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	        sFilterConfig.FilterActivation = ENABLE;
	        sFilterConfig.SlaveStartFilterBank = 0;

	        if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	        {
	          /* Filter configuration Error */
	          Error_Handler();
	        }
	  HAL_CAN_Start(&hcan1);
	 HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

}






// ================= CAN TASK =================

void CANTaskApp(uint16_t valor)
{
	if (hcan1.ErrorCode>0)
	{
	    HAL_CAN_AbortTxRequest(&hcan1, CAN_TX_MAILBOX0 |
	                                  CAN_TX_MAILBOX1 |
	                                  CAN_TX_MAILBOX2);
	    hcan1.ErrorCode=0;
	}
	uint16_t corrente_bombaCombustivel =valor;
		 uint16_t  corrente_bombaAgua = valor;

		 txHeader.StdId = 100;                  // ID CAN padrão (11 bits)
		 txHeader.ExtId = 0;                    // Não usado em Standard ID
		 txHeader.IDE = CAN_ID_STD;             // Standard ID
		 txHeader.RTR = CAN_RTR_DATA;           // Data frame
		 txHeader.DLC = 8;              // 8 bytes
		 txHeader.TransmitGlobalTime = DISABLE; // Sem timestamp

		 txData[0] = corrente_bombaCombustivel >> 8;  // MSB
		 txData[1] = corrente_bombaCombustivel & 0xFF; // LSB

		 txData[2] = corrente_bombaAgua/256;
		 txData[3] = corrente_bombaAgua%256;

		 txData[4]=65000/256;
		 txData[5]=65000%256;

		 txData[6]=0;
		 txData[7]=0;
		 if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0)
		 {
		     HAL_CAN_AddTxMessage(&hcan1,
		                          &txHeader,
		                          txData,
		                          &txMailBox);
		 }
		 else{
			 HAL_CAN_AbortTxRequest(&hcan1, CAN_TX_MAILBOX0 |
				                                  CAN_TX_MAILBOX1 |
				                                  CAN_TX_MAILBOX2);
			 HAL_CAN_AddTxMessage(&hcan1,
			 		                          &txHeader,
			 		                          txData,
			 		                          &txMailBox);

		 }
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);

	if (rxHeader.StdId == 1572 &&rxHeader.ExtId==0) {
		for(int i=0;i<8;i++){
			canOutStatus[i]=(rxData[1]>>i) &0x01;
		}

	}
	if(rxHeader.StdId==1523 && rxHeader.ExtId==0){

		tensaoBatMega = ( (uint16_t)rxData[3]);
	}
	if(rxHeader.StdId==1520+33 && rxHeader.ExtId==0){

		for(int i=0;i<7;i++){
					if(genericPWMDuty[0]!=rxData[0]){
						newAngle=1;

					}
					genericPWMDuty[i]=rxData[i];
				}

		}

	if(rxHeader.StdId==MEGACANID+54 && rxHeader.ExtId==0){


		if(rxData[5]>0){
			genericPWMDuty[7]=1;
		}

		else{
			genericPWMDuty[7]=0;
		}

	}

	rxHeader.ExtId=0;

}


