/*
 * My_ARM_CAN_Lib_V1_0.h
 *
 *  Created on: Apr 28, 2025
 *      Author: Administrator
 *
 *   V1.0 :  2025, 04, 28  == 최초작성 == AIOT 2기
 */

#ifndef MY_LIB_MY_ARM_CAN_LIB_V1_0_H_
#define MY_LIB_MY_ARM_CAN_LIB_V1_0_H_

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = {'0', '1', '2', '3', '4', '5', '6', '7'}; // ?��?�� Data
uint8_t RxData[8];  // RX 버퍼
HAL_StatusTypeDef TxStatus;
uint8_t can1_rx_end_flag = 0;
uint8_t can2_rx_end_flag = 0;


// CAN RX Info Help
void can_info_rx(CAN_HandleTypeDef *hcan, char *mode)
{
  char *buf;
  buf = (hcan->Instance  == CAN1) ? "CAN1" : "CAN2";
  printf("%s RX Prescaler = %d, Mode = %s\r\n", buf, hcan->Init.Prescaler, mode);
  printf("Dev_MSG_ID = 0x%03lX, RTR : %lu-User Data Frame\r\n", RxHeader.StdId, RxHeader.RTR);
  printf("RX Byte(DLC) : %lu Rx Data = %s\r\n\n", (uint32_t)RxHeader.DLC, RxData);

  //사용예제 : can_info_rx(&hcan1, "LOOPBACK");
}

// CAN TX Info Help
void can_info_tx(CAN_HandleTypeDef *hcan, char *mode)
{
  char *buf;
  buf = (hcan->Instance  == CAN1) ? "CAN1" : "CAN2";
  printf("%s TX Prescaler = %d, Mode = %s\r\n", buf, hcan->Init.Prescaler, mode);
  printf("Dev_MSG_ID = 0x%03lX, RTR : %lu-User Data Frame\r\n", TxHeader.StdId, TxHeader.RTR);
  printf("TX Byte(DLC) : %lu Tx Data = %s\r\n\n", (uint32_t)TxHeader.DLC, TxData);

  //사용예제 : can_info_tx(&hcan1, "LOOPBACK");
}


// CAN 송신 함수
char can_tx(CAN_HandleTypeDef *hcan, uint16_t  dev_msg_id, uint8_t tx_byte, uint8_t *pdata)
 {
   TxHeader.DLC = tx_byte; // default = 8 byte;
   TxHeader.IDE = CAN_ID_STD; // mode
   TxHeader.StdId = dev_msg_id;
   TxHeader.ExtId = 0;
   TxHeader.RTR = CAN_RTR_DATA; // 0 = My TX(내가 전송할때), 1 = TX Call(전송요청)
   uint32_t TxMailbox;

   if(HAL_CAN_AddTxMessage(&hcan->Instance, &TxHeader, pdata, &TxMailbox) == HAL_OK)
    {
     #if debug
 	  printf("CAN TX OK!!!\r\n");
     #endif

 	return 1;
    }
   else
   {
    #if debug
 	 printf("CAN TX ERROR!!!\r\n");
    #endif

    return 0;
   }
 }


// CAN 수신 함수
char can_rx(CAN_HandleTypeDef *hcan)
 {
   // Polling
   if(HAL_CAN_GetRxFifoFillLevel(&hcan->Instance, CAN_RX_FIFO0) > 0)
   {
 	if(HAL_CAN_GetRxMessage(&hcan->Instance, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
 	{
       #if debug
 	     printf("Rx data = %s\r\n", RxData);
       #endif

 	  return 1;
 	}
    else
    {
      #if debug
 	   printf("Rx data Error\r\n");
      #endif
 	 return 0;
    }
   }
  else return 0;
 }


 // 필터 설정 (모든 메시지 수신 허용)
 // CAN 필터 설정 함수
char CAN_Filter_Config(CAN_HandleTypeDef *hcan)
 {
  CAN_FilterTypeDef sFilterConfig;

  sFilterConfig.FilterBank = (hcan->Instance  == CAN1) ? 0 : 14;  // CAN2는 14번 이상을 사용 == 요게 매우 중요함
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;//0
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) == HAL_OK)
   {
     return 1;
   }
  else return 0;
 }

void CAN_Transmit(void)
{
 TxHeader.DLC = 6;//8; //
 TxHeader.IDE = CAN_ID_STD;
 TxHeader.StdId = 0x321; //
 //TxHeader.TransmitGlobalTime = ENABLE;

 if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, (uint32_t*)CAN_TX_MAILBOX0) != HAL_OK)
 {
  // ?��?�� ?��?�� 처리
  Error_Handler();
  printf("tx error\r\n");
 }
}

void CAN_Receive(void)
{
 if(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
  {
   if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
     uint16_t timestamp = RxHeader.Timestamp;
     printf("Time = %u, Can Rx = %s\r\n", timestamp, RxData);
    }
  }
}


// CAN RX 인터럽트 콜백 함수
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
   if (hcan->Instance == CAN1) // 수신은 CAN1만 수신 CAN2로부터 인터럽트가 왔을 때
    {
        if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
          // 수신 데이터 RxData 처리
         can1_rx_end_flag = 1; // can1
        }
    }

  else if(hcan->Instance == CAN2)
   {
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
	 {
	  // 수신 데이터 RxData 처리
	  can2_rx_end_flag = 1; // can2
	 }
   }
}

#endif /* MY_LIB_MY_ARM_CAN_LIB_V1_0_H_ */
