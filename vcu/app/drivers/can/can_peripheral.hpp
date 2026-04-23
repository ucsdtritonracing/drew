#pragma once

#include "drivers/can/can_bus.hpp"
#include <utility>


namespace drivers::can {

/*
 * CAN peripheral template class.
 *
 * @param Derived Derived class type.
 * @param canBus CAN bus this peripheral is on.
 *
 * Use bindHandler to register handlers for received messages on the CAN bus, e.g:
 *
 * Peripheral(CANBus& canBus) {
 * 		bindHandler<&Peripheral::processStatusMessage>(0x100);
 * 		bindHandler<&Peripheral::processCalibrationMessage>(0x101);
 * }
 */
template<typename Derived>
class CANPeripheral {
public:
	CANPeripheral(CANBus& canBus) : canBus(canBus) {}

	CANPeripheral(CANPeripheral&&)					= delete;
	CANPeripheral(const CANPeripheral&) 			= delete;
	CANPeripheral& operator=(const CANPeripheral&) 	= delete;
	CANPeripheral& operator=(CANPeripheral&&)		= delete;

protected:
	CANBus& canBus;

	template<void (Derived::*Method)(const Message&)>
	void bindHandler(uint32_t id) {
		if (id <= 0 || id >= MAX_CAN_EXTD_ID) return;

	    canBus.addMessageHandler(this, id,
	        [](void *instance, const Message& message) {
	    		Derived* d = static_cast<Derived*>(instance);
	            (d->*Method)(message);
	        }
	    );
	}
};

} // namespace drivers::can
