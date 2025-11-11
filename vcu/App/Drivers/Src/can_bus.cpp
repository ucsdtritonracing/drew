#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include "main.h"

namespace drivers::CAN {

CANBus::CANBus(FDCAN_HandleTypeDef *fdcan, osMessageQueueId_t rxQueue)
		: fdcan(fdcan), rxQueue(rxQueue) {}

void CANBus::transmit(uint32_t id, const uint8_t *data, uint32_t dlc) const {
	FDCAN_TxHeaderTypeDef txHeader;
	txHeader.Identifier 			= id;
	txHeader.IdType 				= (id <= MAX_CAN_STD_ID) ? FDCAN_STANDARD_ID : FDCAN_EXTENDED_ID;
	txHeader.TxFrameType 			= FDCAN_DATA_FRAME;
	txHeader.DataLength 			= dlc;
	txHeader.ErrorStateIndicator 	= FDCAN_ESI_ACTIVE;
	txHeader.BitRateSwitch			= FDCAN_BRS_OFF;
	txHeader.FDFormat				= FDCAN_CLASSIC_CAN;
	txHeader.TxEventFifoControl		= FDCAN_NO_TX_EVENTS;
	txHeader.MessageMarker			= 0;
	if (HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &txHeader, data) != HAL_OK) {
		Error_Handler();
	}
}

void CANBus::addMessageHandler(void *instance, uint32_t id, CANHandler callback) {
	if (numHandlers >= MAX_HANDLERS) {
		return;
	}

	for (size_t i = 0; i < numHandlers; i++) {
		if (handlers[i].id == id &&
			handlers[i].instance == instance &&
			handlers[i].callback == callback) {
			return;
		}
	}
	handlers[numHandlers++] = {instance, id, callback};
}

} // namespace drivers::CAN
