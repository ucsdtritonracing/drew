#include "drivers/inverter/inverter.hpp"
#include "drivers/inverter/inverter_can_types.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "vehicle/vehicle_state.hpp"
#include <cstring>


static_assert(sizeof(vehicle::inverter::PostFaults) == sizeof(uint32_t), "PostFaults must be exactly 32 bits, check CAN message layout");
static_assert(sizeof(vehicle::inverter::RunFaults) == sizeof(uint32_t), "RunFaults must be exactly 32 bits, check CAN message layout");


namespace drivers::inverter {
	Inverter::Inverter(drivers::can::CANBus &canBus) :
			drivers::can::CANPeripheral<Inverter>(canBus) {}

	void Inverter::init() {
		bindHandler<&Inverter::processTemperature1Message>(CAN_ID_TEMP_1);
		bindHandler<&Inverter::processTemperature2Message>(CAN_ID_TEMP_2);
		bindHandler<&Inverter::processTemperature3Message>(CAN_ID_TEMP_3);
		bindHandler<&Inverter::processMotorPositionMessage>(CAN_ID_MOTOR_POSITION);
		bindHandler<&Inverter::processTorqueInformationMessage>(CAN_ID_TORQUE_INFORMATION);
		bindHandler<&Inverter::processTorqueCapabilityMessage>(CAN_ID_TORQUE_CAPABILITY);
		bindHandler<&Inverter::processFaultFlagsMessage>(CAN_ID_FAULT_CODES);
		bindHandler<&Inverter::processInternalStatesMessage>(CAN_ID_INTERNAL_STATES);

		torqueCommandSlotHandle = bindTxSlot(COMMAND_MESSAGE_ID, drivers::can::TxPriority::CONTROL);
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

		canBus.publishTxSlot(torqueCommandSlotHandle, txData, 8);
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

	static inline float parseScaled(const can::Message& message, size_t index, float scale) {
	    const int16_t raw = (static_cast<uint16_t>(message.data[index * 2 + 1]) << 8)
	                 | message.data[index * 2];
	    return static_cast<float>(raw) / scale;
	}

	void Inverter::processTemperature1Message(const can::Message &message) {
		vehicle::inverter::Temperature1 temperatures1;
		temperatures1.moduleA			= parseScaled(message, static_cast<size_t>(Temperatures1::ModuleA), COMMON_SCALE);
		temperatures1.moduleB			= parseScaled(message, static_cast<size_t>(Temperatures1::ModuleB), COMMON_SCALE);
		temperatures1.moduleC			= parseScaled(message, static_cast<size_t>(Temperatures1::ModuleC), COMMON_SCALE);
		temperatures1.gateDriverBoard	= parseScaled(message, static_cast<size_t>(Temperatures1::GateDriverBoard), COMMON_SCALE);
		vehicle::vehicleState.setInverterTemperature1(temperatures1);
	}

	void Inverter::processTemperature2Message(const can::Message &message) {
		vehicle::inverter::Temperature2 temperatures2;
		temperatures2.controlBoard	= parseScaled(message, static_cast<size_t>(Temperatures2::ControlBoard), COMMON_SCALE);
		temperatures2.rtd1			= parseScaled(message, static_cast<size_t>(Temperatures2::RTD1), COMMON_SCALE);
		temperatures2.rtd2			= parseScaled(message, static_cast<size_t>(Temperatures2::RTD2), COMMON_SCALE);
		temperatures2.rtd3			= parseScaled(message, static_cast<size_t>(Temperatures2::RTD3), COMMON_SCALE);
		vehicle::vehicleState.setInverterTemperature2(temperatures2);
	}

	void Inverter::processTemperature3Message(const can::Message &message) {
		vehicle::inverter::Temperature3 temperatures3;
		temperatures3.coolant		= parseScaled(message, static_cast<size_t>(Temperatures3::Coolant), COMMON_SCALE);
		temperatures3.hotSpot		= parseScaled(message, static_cast<size_t>(Temperatures3::HotSpot), COMMON_SCALE);
		temperatures3.motor			= parseScaled(message, static_cast<size_t>(Temperatures3::Motor), COMMON_SCALE);
		temperatures3.torqueShudder	= parseScaled(message, static_cast<size_t>(Temperatures3::TorqueShudder), COMMON_SCALE);
		vehicle::vehicleState.setInverterTemperature3(temperatures3);
	}

	void Inverter::processMotorPositionMessage(const can::Message &message) {
//		processStandardMessage(message, FOUR_DATA, state.motorPosition, MOTOR_POSITION_START, MOTOR_POSITION_SCALES);
		vehicle::inverter::MotorPosition motorPosition;
		motorPosition.motorAngle				= parseScaled(message, static_cast<size_t>(MotorPosition::MotorAngle), COMMON_SCALE);
		motorPosition.motorSpeed				= parseScaled(message, static_cast<size_t>(MotorPosition::MotorSpeed), NO_SCALE);
		motorPosition.electricalOutputFrequency	= parseScaled(message, static_cast<size_t>(MotorPosition::ElectricalOutputFrequency), COMMON_SCALE);
		motorPosition.deltaResolverFiltered		= parseScaled(message, static_cast<size_t>(MotorPosition::DeltaResolverFiltered), COMMON_SCALE);
		vehicle::vehicleState.setInverterMotorPosition(motorPosition);
	}

	void Inverter::processTorqueInformationMessage(const can::Message &message) {
//		processStandardMessage(message, TWO_DATA, state.torqueInformation, TORQUE_INFO_START, COMMON_SCALE);
		vehicle::inverter::TorqueInformation torqueInformation;
		torqueInformation.commandedTorque	= parseScaled(message, static_cast<size_t>(TorqueInformation::CommandedTorque), COMMON_SCALE);
		torqueInformation.torqueFeedback	= parseScaled(message, static_cast<size_t>(TorqueInformation::TorqueFeedback), COMMON_SCALE);
		vehicle::vehicleState.setInverterTorqueInformation(torqueInformation);
	}

	void Inverter::processTorqueCapabilityMessage(const can::Message &message) {
//		processStandardMessage(message, ONE_DATA, state.torqueInformation, TORQUE_CAP_START, COMMON_SCALE);
		vehicle::inverter::TorqueCapability torqueCapability;
		torqueCapability.torqueCapability	= parseScaled(message, static_cast<size_t>(TorqueCapability::TorqueCapability), COMMON_SCALE);
		vehicle::vehicleState.setInverterTorqueCapability(torqueCapability);
	}

	void Inverter::processFaultFlagsMessage(const can::Message &message) {
		if (message.numBytes != FDCAN_DLC_BYTES_8) {
			return; // incorrect number of bytes received, bad message
		}

		vehicle::inverter::PostFaults postFaults;
		uint32_t postFaultsBitField =	(static_cast<uint32_t>(message.data[3]) << 24) |
										(static_cast<uint32_t>(message.data[2]) << 16) |
										(static_cast<uint32_t>(message.data[1]) << 8)  |
										(static_cast<uint32_t>(message.data[0]));
		std::memcpy(&postFaults, &postFaultsBitField, sizeof(postFaults));

		vehicle::inverter::RunFaults runFaults;
		uint32_t runFaultsBitField = 	(static_cast<uint32_t>(message.data[7]) << 24) |
										(static_cast<uint32_t>(message.data[6]) << 16) |
										(static_cast<uint32_t>(message.data[5]) << 8)  |
										(static_cast<uint32_t>(message.data[4]));
		std::memcpy(&runFaults, &runFaultsBitField, sizeof(runFaults));

		vehicle::inverter::FaultFlags faultFlags = {postFaults, runFaults};
		vehicle::vehicleState.setInverterFaultFlags(faultFlags);
	}

	void Inverter::processInternalStatesMessage(const can::Message &message) {
		if (message.numBytes != FDCAN_DLC_BYTES_8) {
			return; // incorrect number of bytes received, bad message
		}

		// Hard-coded b/c it takes least runtime and storage
		vehicle::inverter::InternalStates internalStates;
		internalStates.inverterEnableState		= message.data[6] & INVERTER_ENABLE_STATE_MASK;
		internalStates.inverterEnableLockout	= message.data[6] & INVERTER_ENABLE_LOCKOUT_MASK;
		internalStates.bmsActive				= message.data[7] & BMS_ACTIVE_MASK;
		internalStates.bmsLimitingTorque		= message.data[7] & BMS_LIMITING_TORQUE_MASK;
		vehicle::vehicleState.setInverterInternalStates(internalStates);
	}

} // namespace drivers::inverter
