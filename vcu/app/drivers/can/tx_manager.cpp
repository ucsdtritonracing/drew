#include "drivers/can/tx_manager.hpp"
#include "drivers/can/tx_slot.hpp"
#include <cstring>


namespace drivers::can {

TxSlotHandle TxManager::addSlot(uint32_t id, TxPriority priority) {
	if (numSlots >= MAX_SLOTS) {
		return TxSlotHandle{
			.id = 0,
			.valid = false
		};
	}

	TxSlotHandle handle = TxSlotHandle{
		.id = numSlots,
		.valid = true
	};


	TxSlot &slot = slots[numSlots];
	slot.id = id;
	slot.priority = priority;
	slot.newMessage = false;
	slot.lastTx = 0;

	numSlots++;

	return handle;
}


void TxManager::publish(TxSlotHandle handle, const uint8_t data[], uint32_t numBytes) {
	if (!handle.valid) {
		return;
	}

	TxSlot &slot = slots[handle.id];

	std::memcpy(slot.data, data, numBytes);
	slot.numBytes = numBytes;
	slot.newMessage = true;
}


void TxManager::markQueued(TxSlotHandle handle) {
	if (!handle.valid) {
		return;
	}

	TxSlot &slot = slots[handle.id];
	slot.newMessage = false;
	slot.lastTx = xTaskGetTickCount();
}


PendingTx TxManager::getNextTx() {
	uint8_t highestPrioritySlotId = 0;

	bool newMessages = false;
	for (uint8_t id = 0; id < numSlots; id++) {
		if (!slots[id].newMessage) {
			continue;
		}

		if (!newMessages || slots[id].priority < slots[highestPrioritySlotId].priority) {
			highestPrioritySlotId = id;
			newMessages = true;
		}
	}

	PendingTx nextTx;
	if (newMessages) {
		TxSlot &slot = slots[highestPrioritySlotId];
		nextTx.slot = TxSlotHandle{
			.id = highestPrioritySlotId,
			.valid = true
		};
		nextTx.valid = true;
		nextTx.message.id = slot.id;
		nextTx.message.numBytes = slot.numBytes;
		memcpy(nextTx.message.data, slot.data, slot.numBytes);
	} else {
		nextTx.valid = false;
	}

	return nextTx;
}

} // namespace drivers::can
