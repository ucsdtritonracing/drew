#include "inverter.hpp"
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::inverter {
	Inverter::Inverter(drivers::CAN::CANBus &canBus) :
			drivers::CAN::CANPeripheral<Inverter, State>(canBus) {
		//fill later
	}

	void Inverter::commandMessage(uint16_t torqueRequest) {
		uint16_t scaledTorque = torqueRequest * TORQUE_SCALE;
		if (scaledTorque > 2100) scaledTorque = 2100; // If too redundant, delete
		txData[0] = scaledTorque & 0xFF;
		txData[1] = (scaledTorque >> 8) & 0xFF;
		txData[2] = SPEED_COMMAND_BYTE_2;
		txData[3] = SPEED_COMMAND_BYTE_3;
		txData[4] = DIRECTION_COMMAND;
		txData[5] = INVERTER_ON | (INVERTER_DISCHARGE << 1) | (SPEED_MODE_ENABLE << 2);
		txData[6] = COMMANDED_TORQUE_LIMIT_BYTE_6;
		txData[7] = COMMANDED_TORQUE_LIMIT_BYTE_7;
		canBus.transmit(COMMAND_MESSAGE_ID, txData, FDCAN_DLC_BYTES_8);
	}

} // namespace drivers::inverter
