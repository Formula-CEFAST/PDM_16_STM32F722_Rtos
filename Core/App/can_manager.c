/*
 * can_manager.c
 *
 *  Created on: Jan 26, 2026
 *      Author: diogo
 */


#include "can_manager.h"
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


 void SendCanFrame(uint16_t id,
                         uint16_t v1,
                         uint16_t v2,
                         uint16_t v3,
                         uint16_t v4)
{
    txHeader.StdId = id;
    txHeader.ExtId = 0;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = 8;
    txHeader.TransmitGlobalTime = DISABLE;

    txData[0] = (v1 >> 8) & 0xFF;
    txData[1] = v1 & 0xFF;

    txData[2] = (v2 >> 8) & 0xFF;
    txData[3] = v2 & 0xFF;

    txData[4] = (v3 >> 8) & 0xFF;
    txData[5] = v3 & 0xFF;

    txData[6] = (v4 >> 8) & 0xFF;
    txData[7] = v4 & 0xFF;

    if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0)
    {
        HAL_CAN_AddTxMessage(&hcan1,
                             &txHeader,
                             txData,
                             &txMailBox);
    }
}



// ================= CAN TASK =================

void CANTaskApp(void)
{
    static uint8_t counter = 0;
    static uint8_t slowFrame = 0;

    if (txMailBox > 6)
    {
        HAL_CAN_AbortTxRequest(&hcan1,
                               CAN_TX_MAILBOX0 |
                               CAN_TX_MAILBOX1 |
                               CAN_TX_MAILBOX2);

        hcan1.ErrorCode = 0;
    }


  // osDelay(2);
   SendCanFrame(101,
                   adcData.smartswitch3_current[0],
                   adcData.smartswitch4_current[0],
				   	flagCan,
                   adcData.smartswitch4_current[0]);

    // ID 101




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
	if(rxHeader.StdId==MEGACANID+3){

		//
		flagCan = (rxData[0] << 8) | rxData[1];


	}


	rxHeader.ExtId=0;

}


