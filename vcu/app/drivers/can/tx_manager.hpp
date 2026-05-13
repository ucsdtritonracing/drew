#pragma once

#include "drivers/can/tx_slot.hpp"


namespace drivers::can {

class TxManager {
public:
	TxSlotHandle addSlot(uint32_t id, TxPriority priority);
	Message getNextMessage();
	void publish(TxSlotHandle handle, const uint8_t data[], uint32_t numBytes);
private:
	static constexpr uint8_t MAX_SLOTS = 128;

	uint8_t numSlots = 0;
	TxSlot slots[MAX_SLOTS];
};

} // namespace drivers::can
