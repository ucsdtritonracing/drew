#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include <type_traits>
#include <cassert>

namespace drivers::inverter {

enum class Temperatures : uint8_t {
	ModuleA, ModuleB, ModuleC, GateDriverBoard, ControlBoard,
	RTD1, RTD2, RTD3, Coolant, HotSpot, Motor, TorqueShudder, Count
};

enum class MotorPosition : uint8_t {
	MotorAngle, MotorSpeed, ElectricalOutputFrequency, DeltaResolverFiltered, Count
};

enum class TorqueInformation : uint8_t {
	CommandedTorque, TorqueFeedback, TorqueCapability, Count
};

enum class FaultCodes : uint8_t {
	PostFaultLo, PostFaultHi, RunFaultLo, RunFaultHi, Count
};

enum class PostFaultLo : uint8_t {
	HardwareGate_Desaturation, HardwareOvercurrent, AcceleratorShorted, AcceleratorOpen,
	CurrentSensorLow, CurrentSensorHigh, ModuleTempLow, ModuleTempHigh,
	ControlPCBTempLow, ControlPCBTempHigh, GateDrivePCBTempLow, GateDrivePCBTempHigh,
	Sense5V_Low, Sense5V_High, Sense12V_Low, Sense12V_High
};

enum class PostFaultHi : uint8_t {
	Sense2p5V_Low, Sense2p5V_High, Sense1p5_Low, Sense1p5_High,
	DC_BusVoltageHigh, DC_BusVoltageLow, PrechargeTimeout, PrechargeVoltageFailure,
	EEPROMChecksumInvalid, EEPROMDataOutOfRange, EEPROMUpdateRequired, HardwareDC_BusOvervoltage,
	BrakeShorted = 14, BrakeOpen = 15
};

enum class RunFaultLo : uint8_t {
	MotorOverspeed, Overcurrent, Overvoltage, InverterOverTemp,
	AcceleratorInputShorted, AcceleratorInputOpen, DirectionCommand, InverterResponseTimeout,
	HardwareGate_Desaturation, HardwareOvercurrent, Undervoltage, CANCommandMessageLost,
	MotorOverTemp
};

enum class RunFaultHi : uint8_t {
	BrakeInputShorted, BrakeInputOpen, ModuleAOvertemp, ModuleBOvertemp,
	ModuleCOvertemp, PCBOvertemp, GateDriveBoard1Overtemp, GateDriveBoard2Overtemp,
	GateDriveBoard3Overtemp, CurrentSensor, ResolverNotConnected = 14
};

enum class InternalStates : uint8_t {
	InverterEnableState, InverterEnableLockout, BMSActive, BMSLimitingTorque, Count
};

struct State {
	float temperatures[static_cast<size_t>(Temperatures::Count)];
	float motorPosition[static_cast<size_t>(MotorPosition::Count)];
	float torqueInformation[static_cast<size_t>(TorqueInformation::Count)];
	uint16_t faultCodes[static_cast<size_t>(FaultCodes::Count)];
	bool internalStates[static_cast<size_t>(InternalStates::Count)];
};

class Inverter: public drivers::CAN::CANPeripheral<Inverter, State> {
public:

	Inverter(drivers::CAN::CANBus &canBus);

	/*
	 * @brief Send torque request to motor.
	 *
	 * @param torque request
	 * @param inverter enabled or disabled
	 */
	void commandMessage(uint16_t torqueRequest, bool inverterEnable);

	/*
	 * @brief Process Temperature 1 message
	 *
	 * @param CAN message
	 */
	void processTemperature1(const CAN::Message &message);

	/*
	 * @brief Process Temperature 2 message
	 *
	 * @param CAN message
	 */
	void processTemperature2(const CAN::Message &message);

	/*
	 * @brief Process Temperature 3 message
	 *
	 * @param CAN message
	 */
	void processTemperature3(const CAN::Message &message);

	/*
	 * @brief Process Motor Position message
	 *
	 * @param CAN message
	 */
	void processMotorPosition(const CAN::Message &message);

	/*
	 * @brief Process Torque Information message
	 *
	 * @param CAN message
	 */
	void processTorqueInformation(const CAN::Message &message);

	/*
	 * @brief Process Torque Capability message
	 *
	 * @param CAN message
	 */
	void processTorqueCapability(const CAN::Message &message);

	/*
	 * @brief Process Fault Codes message
	 *
	 * @param CAN message
	 */
	void processFaultCodes(const CAN::Message &message);

	/*
	 * @brief Returns if a specific fault has occured
	 *
	 * @param Category of fault (4 codes)
	 * @param Specific fault in category (16 faults per code)
	 */
	template<typename Enum>
	bool findFault(FaultCodes faultCode, Enum faultName) {
		static_assert(std::is_enum_v<Enum>, "Must input an enum.");
		return state.faultCodes[static_cast<size_t>(faultCode)] & static_cast<uint16_t>(1u << static_cast<uint8_t>(faultName));
	}

	/*
	 * @brief Process Internal States message
	 *
	 * @param CAN message
	 */
	void processInternalStates(const CAN::Message &message);

private:

	uint8_t txData[drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH] = {};
	static constexpr uint8_t COMMON_SCALE = 10; //temperature, torque, angle, frequency
	static constexpr uint8_t NO_SCALE = 1; // angular velocity, internal

	static constexpr uint32_t COMMAND_MESSAGE_ID = 0x0C0;
	static constexpr uint8_t DIRECTION_COMMAND = 1; // Forward
	static constexpr uint8_t INVERTER_DISCHARGE = 0; // Discharge disabled
	static constexpr uint8_t SPEED_MODE_ENABLE = 0; // Do not override Torque Mode
	// Set motor to max speed of 4100 RPM
	static constexpr uint8_t SPEED_COMMAND_BYTE_2 = 0b00010100;
	static constexpr uint8_t SPEED_COMMAND_BYTE_3 = 0b00010000;
	// Set motor torque limit to 210 N.m --> 210 * 10 = 2100 (scaled)
	static constexpr uint8_t COMMANDED_TORQUE_LIMIT_BYTE_6 = 0b00110100;
	static constexpr uint8_t COMMANDED_TORQUE_LIMIT_BYTE_7 = 0b00001000;

	static constexpr uint32_t CAN_ID_TEMP_1 = 0x0A0;
	static constexpr uint32_t CAN_ID_TEMP_2 = 0x0A1;
	static constexpr uint32_t CAN_ID_TEMP_3 = 0x0A2;
	static constexpr size_t TEMP_1_START = static_cast<size_t>(Temperatures::ModuleA);
	static constexpr size_t TEMP_2_START = static_cast<size_t>(Temperatures::ControlBoard);
	static constexpr size_t TEMP_3_START = static_cast<size_t>(Temperatures::Coolant);

	static constexpr uint32_t CAN_ID_MOTOR_POSITION = 0x0A5;
	static constexpr size_t MOTOR_POSITION_START = static_cast<size_t>(MotorPosition::MotorAngle);

	static constexpr uint32_t CAN_ID_TORQUE_INFORMATION = 0x0AC;
	static constexpr uint32_t CAN_ID_TORQUE_CAPABILITY = 0x0B1;
	static constexpr size_t TORQUE_INFO_START = static_cast<size_t>(TorqueInformation::CommandedTorque);
	static constexpr size_t TORQUE_CAP_START = static_cast<size_t>(TorqueInformation::TorqueCapability);

	static constexpr uint32_t CAN_ID_FAULT_CODES = 0x0AB;
	static constexpr size_t FAULT_START = static_cast<size_t>(FaultCodes::PostFaultLo);

	static constexpr uint32_t CAN_ID_INTERNAL_STATES = 0x0AA;
	static constexpr uint8_t INVERTER_ENABLE_STATE_MASK = 0x01;
	static constexpr uint8_t INVERTER_ENABLE_LOCKOUT_MASK = 0x80;
	static constexpr uint8_t BMS_ACTIVE_MASK = 0x02;
	static constexpr uint8_t BMS_LIMITING_TORQUE_MASK = 0x04;

	static constexpr size_t ONE_DATA = 1;
	static constexpr size_t TWO_DATA = 2;
	static constexpr size_t FOUR_DATA = 4;
	static constexpr uint8_t MOTOR_POSITION_SCALES[FOUR_DATA] = {COMMON_SCALE, NO_SCALE, COMMON_SCALE, COMMON_SCALE};

	template <size_t dataLength, typename Data>
	void processStandardMessage(const CAN::Message &message, size_t numData, Data (&data)[dataLength], size_t dataStart, uint8_t scale) {
		if (message.numBytes != 8) {
			return; // incorrect number of bytes received, bad message
		}
		assert(dataStart + numData <= dataLength);
		for (size_t i = 0; i < numData; i++) {
			data[dataStart + i] = static_cast<Data>(static_cast<int16_t>((static_cast<uint16_t>(message.data[i*2+1]) << 8) | message.data[i*2])) / scale;
		}
	}

	template <size_t dataLength, size_t scalerLength, typename Data>
	void processStandardMessage(const CAN::Message &message, size_t numData, Data (&data)[dataLength], size_t dataStart, const uint8_t (&scale)[scalerLength]) {
		if (message.numBytes != 8) {
			return; // incorrect number of bytes received, bad message
		}
		assert(dataStart + numData <= dataLength);
		for (size_t i = 0; i < numData; i++) {
			data[dataStart + i] = static_cast<Data>(static_cast<int16_t>((static_cast<uint16_t>(message.data[i*2+1]) << 8) | message.data[i*2])) / scale[i];
		}
	}

};

} // namespace drivers::inverter
