#include "drivers/can/can_utils.hpp"
#include "rtos/objects.hpp"
#include "cmsis_os.h"
#include "stm32g4xx_hal.h"


/*
 *	@brief FDCAN Rx FIFO0 Callback
 *	@param hfdcan Pointer to an FDCAN_HandleTypeDef structure that contains the configuration information for the specified FDCAN.
 *	@param RxFifo0ITs Indicates which Rx FIFO 0 interrupts are signaled. This parameter can be any combination of FDCAN_Rx_Fifo0_Interrupts.
 *	@retval None
 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		// Get new message
		static drivers::can::Message message;
		FDCAN_RxHeaderTypeDef rxHeader;
	    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, message.data) != HAL_OK) {
	    	//Error_Handler();
	    }
	    message.id = rxHeader.Identifier;
	    message.numBytes = rxHeader.DataLength;

	    // Send CAN message to correct CANBus
		if (hfdcan->Instance == FDCAN1) {
			osMessageQueuePut(rtos::CANBus1RxQueue.getHandle(), &message, 0, 0);
		} else if (hfdcan->Instance == FDCAN2) {
			osMessageQueuePut(rtos::CANBus2RxQueue.getHandle(), &message, 0, 0);
		}

		// Enable callback for new messages
		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			//Error_Handler();
		}
	}
}

/*
  * @brief  Error status callback.
  * @param  hfdcan pointer to an FDCAN_HandleTypeDef structure that contains the configuration information for the specified FDCAN.
  * @param  ErrorStatusITs indicates which Error Status interrupts are signaled. This parameter can be any combination of @arg FDCAN_Error_Status_Interrupts.
  * @retval None
  */
void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
    FDCAN_ProtocolStatusTypeDef protocol_status;
    HAL_FDCAN_GetProtocolStatus(hfdcan, &protocol_status);

    if (protocol_status.BusOff != 0)
    {
        if (hfdcan->Instance == FDCAN1) {
        	osThreadFlagsSet(rtos::CANRecoveryTask.getHandle(), rtos::tasks::CANRecoveryTask::CANBUS1_BUS_OFF_FLAG);
        } else if (hfdcan->Instance == FDCAN2) {
        	osThreadFlagsSet(rtos::CANRecoveryTask.getHandle(), rtos::tasks::CANRecoveryTask::CANBUS2_BUS_OFF_FLAG);
        }
    }
}
