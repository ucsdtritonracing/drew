#pragma once
#include "can_utils.hpp"
#pragma once
#include "can_bus.hpp"
#include <utility>

namespace drivers::CAN {

/*
 * CAN peripheral template class.
 *
 * @param Derived Derived class type.
 * @param StateType Derived class state encapsulation struct.
 * @param canBus CAN bus this peripheral is on.
 *
 * Use bindHandler to register handlers for received messages on the CAN bus, e.g:
 *
 * Peripheral(CANBus& canBus) {
 * 		bindHandler<&Peripheral::processStatusMessage>(0x100);
 * 		bindHandler<&Peripheral::processCalibrationMessage>(0x101);
 * }
 *
 * State is double-buffered, where 'stableState' is read and 'state' is written.
 * This is to deal with non-atomic reads/writes and race conditions.
 * After an object is edited, it needs to be swapped into 'stableState'.
 * 		This is done automatically with registered handlers.
 * 		However, edits outside of a handler will need to manually call commitChanges().
 */
template<typename Derived, typename StateType>
class CANPeripheral {
public:
	CANPeripheral(CANBus& canBus) : canBus(canBus) {}

	CANPeripheral(CANPeripheral&&)					= delete;
	CANPeripheral(const CANPeripheral&) 			= delete;
	CANPeripheral& operator=(const CANPeripheral&) 	= delete;
	CANPeripheral& operator=(CANPeripheral&&)		= delete;

	const StateType getState() const {
		return stableState;
	}

private:
	StateType stableState{};

protected:
	StateType state{};
	CANBus& canBus;

	template<void (Derived::*Method)(const Message&)>
	void bindHandler(uint32_t id) {
		if (id <= 0 || id >= MAX_CAN_STD_ID) return;

	    canBus.addMessageHandler(id, this,
	        [](void *instance, const Message& message) {
	    		Derived* d = static_cast<Derived*>(instance);
	            (d->*Method)(message);
	            d->commitChanges();
	        }
	    );
	}

	void commitChanges() {
		std::swap(state, stableState);
	}
};

} // namespace drivers::CAN
