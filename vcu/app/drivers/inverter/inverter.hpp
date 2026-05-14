#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "drivers/can/tx_slot.hpp"
#include "drivers/inverter/inverter_can_types.hpp"
#include "vehicle/types/inverter_types.hpp"
#include <type_traits>


namespace drivers::inverter {

class Inverter: public drivers::can::CANPeripheral<Inverter> {
public:

	Inverter(drivers::can::CANBus &canBus);

	/*
	 * @brief Initialize the inverter driver.t
	 */
	void init();

	/*
	 * @brief Send torque request to motor.
	 *
	 * @param torque request
	 * @param inverter enabled or disabled
	 */
	void sendCommandMessage(float torqueRequestNm, bool inverterEnable);

	/*
	 * @brief Process Temperature 1 message
	 *
	 * @param CAN message
	 */
	void processTemperature1Message(const can::Message &message);

	/*
	 * @brief Process Temperature 2 message
	 *
	 * @param CAN message
	 */
	void processTemperature2Message(const can::Message &message);

	/*
	 * @brief Process Temperature 3 message
	 *
	 * @param CAN message
	 */
	void processTemperature3Message(const can::Message &message);

	/*
	 * @brief Process Motor Position message
	 *
	 * @param CAN message
	 */
	void processMotorPositionMessage(const can::Message &message);

	/*
	 * @brief Process Torque Information message
	 *
	 * @param CAN message
	 */
	void processTorqueInformationMessage(const can::Message &message);

	/*
	 * @brief Process Torque Capability message
	 *
	 * @param CAN message
	 */
	void processTorqueCapabilityMessage(const can::Message &message);

	/*
	 * @brief Process Fault Codes message
	 *
	 * @param CAN message
	 */
	void processFaultFlagsMessage(const can::Message &message);

	/*
	 * @brief Process Internal States message
	 *
	 * @param CAN message
	 */
	void processInternalStatesMessage(const can::Message &message);


private:
	drivers::can::TxSlotHandle torqueCommandSlotHandle;
	uint8_t txData[drivers::can::MAX_CLASSICAL_CAN_DATA_LENGTH] = 	{};

	static constexpr float COMMON_SCALE = 							10.0f; // temperature, torque, angle, frequency
	static constexpr float NO_SCALE = 								1.0f; // angular velocity, internal

	static constexpr uint32_t COMMAND_MESSAGE_ID = 					0x0C0;
	static constexpr uint8_t DIRECTION_FORWARD = 					1; // Forward
	static constexpr uint8_t INVERTER_DISCHARGE = 					0; // Discharge disabled
	static constexpr uint8_t SPEED_MODE_ENABLE = 					0; // Do not override Torque Mode
	// Set motor to max speed of 0 (using torque instead)
	static constexpr uint8_t SPEED_COMMAND_BYTE_2 = 				0;
	static constexpr uint8_t SPEED_COMMAND_BYTE_3 = 				0;
	// Set motor torque limit to 210 N.m --> 210 * 10 = 2100 (scaled)
	static constexpr uint8_t COMMANDED_TORQUE_LIMIT_BYTE_6 = 		0b00110100;
	static constexpr uint8_t COMMANDED_TORQUE_LIMIT_BYTE_7 = 		0b00001000;
	static constexpr uint8_t MAX_TORQUE_ALLOWED = 					210;

	static constexpr uint32_t CAN_ID_TEMP_1 = 						0x0A0;
	static constexpr uint32_t CAN_ID_TEMP_2 = 						0x0A1;
	static constexpr uint32_t CAN_ID_TEMP_3 = 						0x0A2;

	static constexpr uint32_t CAN_ID_MOTOR_POSITION = 				0x0A5;

	static constexpr uint32_t CAN_ID_TORQUE_INFORMATION = 			0x0AC;
	static constexpr uint32_t CAN_ID_TORQUE_CAPABILITY = 			0x0B1;

	static constexpr uint32_t CAN_ID_FAULT_CODES = 					0x0AB;

	static constexpr uint32_t CAN_ID_INTERNAL_STATES = 				0x0AA;
	static constexpr uint8_t INVERTER_ENABLE_STATE_MASK = 			0x01;
	static constexpr uint8_t INVERTER_ENABLE_LOCKOUT_MASK = 		0x80;
	static constexpr uint8_t BMS_ACTIVE_MASK = 						0x02;
	static constexpr uint8_t BMS_LIMITING_TORQUE_MASK = 			0x04;

	template <size_t dataLength>
	void processStandardMessage(const can::Message &message, size_t numData, float (&data)[dataLength], size_t dataStart, uint8_t scale);

	template <size_t dataLength, size_t scalerLength>
	void processStandardMessage(const can::Message &message, size_t numData, float (&data)[dataLength], size_t dataStart, const uint8_t (&scale)[scalerLength]);

};

} // namespace drivers::inverter
