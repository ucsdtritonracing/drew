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
		bindHandler<&Inverter::processTorqueInformation>(CAN_ID_TORQUE_INFORMATION);
		bindHandler<&Inverter::processTorqueCapability>(CAN_ID_TORQUE_CAPABILITY);
		bindHandler<&Inverter::processFaultCodes>(CAN_ID_FAULT_CODES);
		bindHandler<&Inverter::processInternalStates>(CAN_ID_INTERNAL_STATES);
	}

	void Inverter::commandMessage(uint16_t torqueRequest, bool inverterEnable) {
		uint16_t scaledTorque = torqueRequest * COMMON_SCALE;
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
		processStandardMessage(message, FOUR_DATA, state.temperatures, TEMP_1_START, COMMON_SCALE);
	}

	void Inverter::processTemperature2(const CAN::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.temperatures, TEMP_2_START, COMMON_SCALE);
	}

	void Inverter::processTemperature3(const CAN::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.temperatures, TEMP_3_START, COMMON_SCALE);
	}

	void Inverter::processMotorPosition(const CAN::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.motorPosition, MOTOR_POSITION_START, MOTOR_POSITION_SCALES);
	}

	void Inverter::processTorqueInformation(const CAN::Message &message) {
		processStandardMessage(message, TWO_DATA, state.torqueInformation, TORQUE_INFO_START, COMMON_SCALE);
	}

	void Inverter::processTorqueCapability(const CAN::Message &message) {
		processStandardMessage(message, ONE_DATA, state.torqueInformation, TORQUE_CAP_START, COMMON_SCALE);
	}

	void Inverter::processFaultCodes(const CAN::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.faultCodes, FAULT_START, NO_SCALE);
	}

	void Inverter::processInternalStates(const CAN::Message &message) {
		if (message.numBytes != 8) {
			return; // incorrect number of bytes received, bad message
		}
		// Hard-coded b/c it takes least runtime and storage
		state.internalStates[0] = message.data[6] & INVERTER_ENABLE_STATE_MASK;
		state.internalStates[1] = message.data[6] & INVERTER_ENABLE_LOCKOUT_MASK;
		state.internalStates[2] = message.data[7] & BMS_ACTIVE_MASK;
		state.internalStates[3] = message.data[7] & BMS_LIMITING_TORQUE_MASK;
	}

} // namespace drivers::inverter
