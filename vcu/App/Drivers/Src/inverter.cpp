#include "inverter.hpp"
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::inverter {
	Inverter::Inverter(drivers::CAN::CANBus &canBus) :
			drivers::CAN::CANPeripheral<Inverter, State>(canBus) {
		bindHandler<&Inverter::processTemperature1>(CAN_ID_TEMP_1);
		bindHandler<&Inverter::processTemperature2>(CAN_ID_TEMP_2);
		bindHandler<&Inverter::processTemperature3>(CAN_ID_TEMP_3);
		bindHandler<&Inverter::processMotorPosition>(CAN_ID_MOTOR_POSITION);
	}

	void Inverter::commandMessage(uint16_t torqueRequest, bool inverterEnable) {
		uint16_t scaledTorque = torqueRequest * TORQUE_SCALE;
		if (scaledTorque > 2100) scaledTorque = 2100; // If too redundant, delete
		txData[0] = scaledTorque & 0xFF;
		txData[1] = (scaledTorque >> 8) & 0xFF;
		txData[2] = SPEED_COMMAND_BYTE_2;
		txData[3] = SPEED_COMMAND_BYTE_3;
		txData[4] = DIRECTION_COMMAND;
		txData[5] = inverterEnable | (INVERTER_DISCHARGE << 1) | (SPEED_MODE_ENABLE << 2);
		txData[6] = COMMANDED_TORQUE_LIMIT_BYTE_6;
		txData[7] = COMMANDED_TORQUE_LIMIT_BYTE_7;
		canBus.transmit(COMMAND_MESSAGE_ID, txData, FDCAN_DLC_BYTES_8);
	}

	void Inverter::processTemperature1(const CAN::Message &message) {
		processStandardMessage(message, state.temperatures, DATA_1_BEGIN);
	}

	void Inverter::processTemperature2(const CAN::Message &message) {
		processStandardMessage(message, state.temperatures, DATA_2_BEGIN);
	}

	void Inverter::processTemperature3(const CAN::Message &message) {
		processStandardMessage(message, state.temperatures, DATA_3_BEGIN);
	}

	void Inverter::processMotorPosition(const CAN::Message &message) {
		processStandardMessage(message, state.motorPosition, DATA_1_BEGIN);
	}

} // namespace drivers::inverter
