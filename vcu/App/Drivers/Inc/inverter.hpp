#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include <type_traits>

namespace drivers::inverter {

enum class TemperatureSensors : uint8_t {
	ModuleA, ModuleB, ModuleC, GateDriverBoard, ControlBoard,
	RTD1, RTD2, RTD3, Coolant, HotSpot, Motor, TorqueShudder, Count
};

enum class MotorPosition : uint8_t {
	MotorAngle, MotorSpeed, ElectricalOutputFrequency, DeltaResolverFiltered, Count
};

enum class TorqueInformation : uint8_t {
	CommandedTorque, TorqueFeedback, TorqueCapability, Count
};

enum class FaultFlags : uint8_t {
	PostFault, RunFault, Count
};

enum class PostFault : uint8_t {
	HARDWARE_GATE_DESATURATION, HARDWARE_OVERCURRENT, ACCELERATOR_SHORTED, ACCELERATOR_OPEN,
	CURRENT_SENSOR_LOW, CURRENT_SENSOR_HIGH, MODULE_TEMPERATURE_LOW, MODULE_TEMPERATURE_HIGH,
	CONTROL_PCB_TEMPERATURE_LOW, CONTROL_PCB_TEMPERATURE_HIGH, GATE_DRIVE_PCB_TEMPERATURE_LOW, GATE_DRIVE_PCB_TEMPERATURE_HIGH,
	SENSE_5V_LOW, SENSE_5V_HIGH, SENSE_12V_LOW, SENSE_12V_HIGH,
	SENSE_2p5V_LOW, SENSE_2p5V_HIGH, SENSE_1p5V_LOW, SENSE_1p5V_HIGH,
	DC_BUS_VOLTAGE_HIGH, DC_BUS_VOLTAGE_LOW, PRECHARGE_TIMEOUT, PRECHARGE_VOLTAGE_FAILURE,
	EEPROM_CHECKSUM_INVALID, EEPROM_DATA_OUT_OF_RANGE, EEPROM_UPDATE_REQUIRED, HARDWARE_DC_BUS_OVERVOLTAGE,
	BRAKE_SHORTED = 30, BRAKE_OPEN = 31
};

enum class RunFault : uint8_t {
	MOTOR_OVERSPEED, OVERCURRENT, OVERVOLTAGE, INVERTER_OVERTEMPERATURE,
	ACCELERATOR_INPUT_SHORTED, ACCELERATOR_INPUT_OPEN, DIRECTION_COMMAND, INVERTER_RESPONSE_TIMEOUT,
	HARDWARE_GATE_DESATURATION, HARDWARE_OVERCURRENT, UNDERVOLTAGE, CAN_COMMAND_MESSAGE_LOST,
	MOTOR_OVERTEMPERATURE, BRAKE_INPUT_SHORTED = 16, BRAKE_INPUT_OPEN, MODULE_A_OVERTEMPERATURE,
	MODULE_B_OVERTEMPERATURE, MODULE_C_OVERTEMPERATURE, PCB_OVERTEMPERATURE, GATE_DRIVE_BOARD_1_OVERTEMPERATURE,
	GATE_DRIVE_BOARD_2_OVERTEMPERATURE, GATE_DRIVE_BOARD_3_OVERTEMPERATURE, CURRENT_SENSOR, RESOLVER_NOT_CONNECTED = 30
};

enum class InternalStates : uint8_t {
	InverterEnableState, InverterEnableLockout, BMSActive, BMSLimitingTorque, Count
};

struct State {
	float temperatures[static_cast<size_t>(TemperatureSensors::Count)];
	float motorPosition[static_cast<size_t>(MotorPosition::Count)];
	float torqueInformation[static_cast<size_t>(TorqueInformation::Count)];
	uint32_t faultFlags[static_cast<size_t>(FaultFlags::Count)];
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
	void sendCommandMessage(uint16_t torqueRequestNm, bool inverterEnable);

	/*
	 * @brief Process Temperature 1 message
	 *
	 * @param CAN message
	 */
	void processTemperature1Message(const CAN::Message &message);

	/*
	 * @brief Process Temperature 2 message
	 *
	 * @param CAN message
	 */
	void processTemperature2Message(const CAN::Message &message);

	/*
	 * @brief Process Temperature 3 message
	 *
	 * @param CAN message
	 */
	void processTemperature3Message(const CAN::Message &message);

	/*
	 * @brief Process Motor Position message
	 *
	 * @param CAN message
	 */
	void processMotorPositionMessage(const CAN::Message &message);

	/*
	 * @brief Process Torque Information message
	 *
	 * @param CAN message
	 */
	void processTorqueInformationMessage(const CAN::Message &message);

	/*
	 * @brief Process Torque Capability message
	 *
	 * @param CAN message
	 */
	void processTorqueCapabilityMessage(const CAN::Message &message);

	/*
	 * @brief Process Fault Codes message
	 *
	 * @param CAN message
	 */
	void processFaultFlagsMessage(const CAN::Message &message);

	/*
	 * @brief Returns if a specific POST (at power up) fault has occured
	 *
	 * @param Specific POST fault
	 */
	bool checkFault(PostFault fault);

	/*
	 * @brief Returns if a specific RUN fault has occured
	 *
	 * @param Specific RUN fault
	 */
	bool checkFault(RunFault fault);

	/*
	 * @brief Process Internal States message
	 *
	 * @param CAN message
	 */
	void processInternalStatesMessage(const CAN::Message &message);

private:

	uint8_t txData[drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH] = 	{};

	static constexpr uint8_t COMMON_SCALE = 						10; // temperature, torque, angle, frequency
	static constexpr uint8_t NO_SCALE = 							1; // angular velocity, internal

	static constexpr uint32_t COMMAND_MESSAGE_ID = 					0x0C0;
	static constexpr uint8_t DIRECTION_COMMAND = 					1; // Forward
	static constexpr uint8_t INVERTER_DISCHARGE = 					0; // Discharge disabled
	static constexpr uint8_t SPEED_MODE_ENABLE = 					0; // Do not override Torque Mode
	// Set motor to max speed of 4100 RPM
	static constexpr uint8_t SPEED_COMMAND_BYTE_2 = 				0b00010100;
	static constexpr uint8_t SPEED_COMMAND_BYTE_3 = 				0b00010000;
	// Set motor torque limit to 210 N.m --> 210 * 10 = 2100 (scaled)
	static constexpr uint8_t COMMANDED_TORQUE_LIMIT_BYTE_6 = 		0b00110100;
	static constexpr uint8_t COMMANDED_TORQUE_LIMIT_BYTE_7 = 		0b00001000;
	static constexpr uint8_t MAX_TORQUE_ALLOWED = 					210;

	static constexpr uint32_t CAN_ID_TEMP_1 = 						0x0A0;
	static constexpr uint32_t CAN_ID_TEMP_2 = 						0x0A1;
	static constexpr uint32_t CAN_ID_TEMP_3 = 						0x0A2;
	static constexpr size_t TEMP_1_START = 							static_cast<size_t>(TemperatureSensors::ModuleA);
	static constexpr size_t TEMP_2_START = 							static_cast<size_t>(TemperatureSensors::ControlBoard);
	static constexpr size_t TEMP_3_START = 							static_cast<size_t>(TemperatureSensors::Coolant);

	static constexpr uint32_t CAN_ID_MOTOR_POSITION = 				0x0A5;
	static constexpr size_t MOTOR_POSITION_START = 					static_cast<size_t>(MotorPosition::MotorAngle);

	static constexpr uint32_t CAN_ID_TORQUE_INFORMATION = 			0x0AC;
	static constexpr uint32_t CAN_ID_TORQUE_CAPABILITY = 			0x0B1;
	static constexpr size_t TORQUE_INFO_START = 					static_cast<size_t>(TorqueInformation::CommandedTorque);
	static constexpr size_t TORQUE_CAP_START = 						static_cast<size_t>(TorqueInformation::TorqueCapability);

	static constexpr uint32_t CAN_ID_FAULT_CODES = 					0x0AB;

	static constexpr uint32_t CAN_ID_INTERNAL_STATES = 				0x0AA;
	static constexpr uint8_t INVERTER_ENABLE_STATE_MASK = 			0x01;
	static constexpr uint8_t INVERTER_ENABLE_LOCKOUT_MASK = 		0x80;
	static constexpr uint8_t BMS_ACTIVE_MASK = 						0x02;
	static constexpr uint8_t BMS_LIMITING_TORQUE_MASK = 			0x04;

	static constexpr size_t ONE_DATA = 								1;
	static constexpr size_t TWO_DATA = 								2;
	static constexpr size_t FOUR_DATA = 							4;
	static constexpr uint8_t MOTOR_POSITION_SCALES[FOUR_DATA] = 	{COMMON_SCALE, NO_SCALE, COMMON_SCALE, COMMON_SCALE};

	template <size_t dataLength>
	void processStandardMessage(const CAN::Message &message, size_t numData, float (&data)[dataLength], size_t dataStart, uint8_t scale);

	template <size_t dataLength, size_t scalerLength>
	void processStandardMessage(const CAN::Message &message, size_t numData, float (&data)[dataLength], size_t dataStart, const uint8_t (&scale)[scalerLength]);

};

} // namespace drivers::inverter
