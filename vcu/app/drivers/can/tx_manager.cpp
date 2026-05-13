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


Message TxManager::getNextMessage() {
	uint8_t highestPrioritySlotId = 0;
	for (uint8_t id = 0; id < numSlots; id++) {
		if (slots[id].priority < slots[highestPrioritySlotId].priority) {
			highestPrioritySlotId = id;
		}
	}

	Message nextMessage;
	TxSlot &slot = slots[highestPrioritySlotId];
	nextMessage.id = slot.id;
	nextMessage.numBytes = slot.numBytes;
	memcpy(nextMessage.data, slot.data, slot.numBytes);

	return nextMessage;
}

} // namespace drivers::can
