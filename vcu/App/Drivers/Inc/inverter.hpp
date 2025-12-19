#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::inverter {

struct State {

};

class Inverter: public drivers::CAN::CANPeripheral<Inverter, State> {
public:
	Inverter(drivers::CAN::CANBus &canBus);

	/*
	 * TODO: list all methods required
	 */

	void processMessageFaultCodes(const CAN::Message &message);

	bool checkError(const uint8_t bitMask);

private:

	/*
	 * TODO: Fill in constants and private variables
	 */
};

} // namespace drivers::inverter
