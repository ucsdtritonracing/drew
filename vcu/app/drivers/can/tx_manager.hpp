#pragma once

#include "drivers/can/tx_slot.hpp"


namespace drivers::can {

struct PendingTx {
	TxSlotHandle slot;
	Message message;
	bool valid;
};

class TxManager {
public:
	TxSlotHandle addSlot(uint32_t id, TxPriority priority);
	PendingTx getNextTx();
	void publish(TxSlotHandle handle, const uint8_t data[], uint32_t numBytes);
	void markQueued(TxSlotHandle handle);
private:
	static constexpr uint8_t MAX_SLOTS = 128;

	uint8_t numSlots = 0;
	TxSlot slots[MAX_SLOTS];
};

} // namespace drivers::can
