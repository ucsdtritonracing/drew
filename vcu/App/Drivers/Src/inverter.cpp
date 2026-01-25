#include "inverter.hpp"
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::inverter {
	Inverter::Inverter(drivers::can::CANBus &canBus) :
			drivers::can::CANPeripheral<Inverter, State>(canBus) {}

	void Inverter::init() {
		bindHandler<&Inverter::processTemperature1Message>(CAN_ID_TEMP_1);
		bindHandler<&Inverter::processTemperature2Message>(CAN_ID_TEMP_2);
		bindHandler<&Inverter::processTemperature3Message>(CAN_ID_TEMP_3);
		bindHandler<&Inverter::processMotorPositionMessage>(CAN_ID_MOTOR_POSITION);
		bindHandler<&Inverter::processTorqueInformationMessage>(CAN_ID_TORQUE_INFORMATION);
		bindHandler<&Inverter::processTorqueCapabilityMessage>(CAN_ID_TORQUE_CAPABILITY);
		bindHandler<&Inverter::processFaultFlagsMessage>(CAN_ID_FAULT_CODES);
		bindHandler<&Inverter::processInternalStatesMessage>(CAN_ID_INTERNAL_STATES);
	}

	void Inverter::sendCommandMessage(float torqueRequestNm, bool inverterEnable) {
		uint16_t scaledTorque = torqueRequestNm * COMMON_SCALE;
		if (scaledTorque > (MAX_TORQUE_ALLOWED * COMMON_SCALE)) scaledTorque = MAX_TORQUE_ALLOWED * COMMON_SCALE;
		txData[0] = scaledTorque & 0xFF;
		txData[1] = (scaledTorque >> 8) & 0xFF;
		txData[2] = SPEED_COMMAND_BYTE_2;
		txData[3] = SPEED_COMMAND_BYTE_3;
		txData[4] = DIRECTION_FORWARD;
		txData[5] = inverterEnable | (INVERTER_DISCHARGE << 1) | (SPEED_MODE_ENABLE << 2);
		txData[6] = COMMANDED_TORQUE_LIMIT_BYTE_6;
		txData[7] = COMMANDED_TORQUE_LIMIT_BYTE_7;
		canBus.transmit(COMMAND_MESSAGE_ID, txData, FDCAN_DLC_BYTES_8);
	}

	template <size_t dataLength>
	void Inverter::processStandardMessage(const can::Message &message, size_t numData, float (&data)[dataLength], size_t dataStart, uint8_t scale) {
		if (message.numBytes != FDCAN_DLC_BYTES_8) {
			return; // incorrect number of bytes received, bad message
		}
		for (size_t i = 0; i < numData; i++) {
			int16_t raw = (static_cast<uint16_t>(message.data[i*2+1]) << 8) | message.data[i*2];
			data[dataStart + i] = static_cast<float>(raw) / scale;
		}
	}

	template <size_t dataLength, size_t scalerLength>
	void Inverter::processStandardMessage(const can::Message &message, size_t numData, float (&data)[dataLength], size_t dataStart, const uint8_t (&scale)[scalerLength]) {
		if (message.numBytes != FDCAN_DLC_BYTES_8) {
			return; // incorrect number of bytes received, bad message
		}
		for (size_t i = 0; i < numData; i++) {
			int16_t raw = (static_cast<uint16_t>(message.data[i*2+1]) << 8) | message.data[i*2];
			data[dataStart + i] = static_cast<float>(raw) / scale[i];
		}
	}

	void Inverter::processTemperature1Message(const can::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.temperatures, TEMP_1_START, COMMON_SCALE);
	}

	void Inverter::processTemperature2Message(const can::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.temperatures, TEMP_2_START, COMMON_SCALE);
	}

	void Inverter::processTemperature3Message(const can::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.temperatures, TEMP_3_START, COMMON_SCALE);
	}

	void Inverter::processMotorPositionMessage(const can::Message &message) {
		processStandardMessage(message, FOUR_DATA, state.motorPosition, MOTOR_POSITION_START, MOTOR_POSITION_SCALES);
	}

	void Inverter::processTorqueInformationMessage(const can::Message &message) {
		processStandardMessage(message, TWO_DATA, state.torqueInformation, TORQUE_INFO_START, COMMON_SCALE);
	}

	void Inverter::processTorqueCapabilityMessage(const can::Message &message) {
		processStandardMessage(message, ONE_DATA, state.torqueInformation, TORQUE_CAP_START, COMMON_SCALE);
	}

	void Inverter::processFaultFlagsMessage(const can::Message &message) {
		if (message.numBytes != FDCAN_DLC_BYTES_8) {
			return; // incorrect number of bytes received, bad message
		}
		state.faultFlags[static_cast<size_t>(FaultFlags::PostFault)] = 	(static_cast<uint32_t>(message.data[3] << 24)) |
																		(static_cast<uint32_t>(message.data[2] << 16)) |
																		(static_cast<uint32_t>(message.data[1] << 8))  |
																		(static_cast<uint32_t>(message.data[0]));

		state.faultFlags[static_cast<size_t>(FaultFlags::RunFault)] = 	(static_cast<uint32_t>(message.data[7] << 24)) |
																		(static_cast<uint32_t>(message.data[6] << 16)) |
																		(static_cast<uint32_t>(message.data[5] << 8))  |
																		(static_cast<uint32_t>(message.data[4]));
	}

	bool Inverter::checkFault(PostFault fault) {
		return state.faultFlags[static_cast<size_t>(FaultFlags::PostFault)] & static_cast<uint32_t>(1u << static_cast<uint8_t>(fault));
	}

	bool Inverter::checkFault(RunFault fault) {
		return state.faultFlags[static_cast<size_t>(FaultFlags::RunFault)] & static_cast<uint32_t>(1u << static_cast<uint8_t>(fault));
	}

	void Inverter::processInternalStatesMessage(const can::Message &message) {
		if (message.numBytes != FDCAN_DLC_BYTES_8) {
			return; // incorrect number of bytes received, bad message
		}
		// Hard-coded b/c it takes least runtime and storage
		state.internalStates[0] = message.data[6] & INVERTER_ENABLE_STATE_MASK;
		state.internalStates[1] = message.data[6] & INVERTER_ENABLE_LOCKOUT_MASK;
		state.internalStates[2] = message.data[7] & BMS_ACTIVE_MASK;
		state.internalStates[3] = message.data[7] & BMS_LIMITING_TORQUE_MASK;
	}

	float Inverter::getTemperature(TemperatureSensors sensor) {
		return state.temperatures[static_cast<size_t>(sensor)];
	}

	float Inverter::getMotorRPM() {
		return state.motorPosition[static_cast<size_t>(MotorPosition::MotorSpeed)];
	}

	float Inverter::getCommandedTorque() {
		return state.torqueInformation[static_cast<size_t>(TorqueInformation::CommandedTorque)];
	}

	float Inverter::getTorqueFeedback() {
		return state.torqueInformation[static_cast<size_t>(TorqueInformation::TorqueFeedback)];
	}

	float Inverter::getTorqueCapability() {
		return state.torqueInformation[static_cast<size_t>(TorqueInformation::TorqueCapability)];
	}

	bool Inverter::getInternalState(InternalStates internalState) {
		return state.internalStates[static_cast<size_t>(internalState)];
	}

} // namespace drivers::inverter
