#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "main.h"


namespace drivers::can {

CANBus::CANBus() {
	fdcan = nullptr;
}

void CANBus::init(FDCAN_HandleTypeDef& fdcan) {
	this->fdcan = &fdcan;
}

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
	HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &txHeader, data);
}

void CANBus::publishTxSlot(TxSlotHandle handle, const uint8_t data[], uint32_t numBytes) {
	txManager.publish(handle, data, numBytes);
	flushTx();
}

void CANBus::flushTx() {
	while (HAL_FDCAN_GetTxFifoFreeLevel(fdcan) > 0) {
		PendingTx tx = txManager.getNextTx();

		if (!tx.valid) {
			return;
		}

		FDCAN_TxHeaderTypeDef txHeader;
		txHeader.Identifier 			= tx.message.id;
		txHeader.IdType 				= (tx.message.id <= MAX_CAN_STD_ID) ? FDCAN_STANDARD_ID : FDCAN_EXTENDED_ID;
		txHeader.TxFrameType 			= FDCAN_DATA_FRAME;
		txHeader.DataLength 			= dlcFromBytes(tx.message.numBytes);
		txHeader.ErrorStateIndicator 	= FDCAN_ESI_ACTIVE;
		txHeader.BitRateSwitch			= FDCAN_BRS_OFF;
		txHeader.FDFormat				= FDCAN_CLASSIC_CAN;
		txHeader.TxEventFifoControl		= FDCAN_NO_TX_EVENTS;
		txHeader.MessageMarker			= 0;
		HAL_FDCAN_AddMessageToTxFifoQ(fdcan, &txHeader, tx.message.data);
		txManager.markQueued(tx.slot);
	}
}

void CANBus::addMessageHandler(void *instance, uint32_t id, CANHandler callback) {
	if (numHandlers >= MAX_HANDLERS) {
		Error_Handler();
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


TxSlotHandle CANBus::addTxSlot(uint32_t id, TxPriority priority) {
	return txManager.addSlot(id, priority);
}

void CANBus::processMessage(Message *message) const {
	for (size_t i = 0; i < numHandlers; i++) {
		HandlerEntry handler = handlers[i];
		if (handler.id == message->id) {
			handler.callback(handler.instance, *message);
		}
	}
}

} // namespace drivers::can
