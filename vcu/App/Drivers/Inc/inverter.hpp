#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::inverter {

struct State {
	struct Temperatures {
		int16_t moduleA;
		int16_t moduleB;
		int16_t moduleC;
		int16_t gateDriverBoard;
		int16_t controlBoard;
		int16_t rtd1;
		int16_t rtd2;
		int16_t rtd3;
		int16_t coolant;
		int16_t hotSpot;
		int16_t motor;
	};
	Temperatures temperatures;
	int16_t torqueShudder;
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
	 * @brief Process Temperature 1 messages
	 *
	 * @param CAN message
	 */
	void processTemperature1(const CAN::Message &message);

	/*
	 * @brief Process Temperature 2 messages
	 *
	 * @param CAN message
	 */
	void processTemperature2(const CAN::Message &message);

	/*
	 * @brief Process Temperature 3 messages
	 *
	 * @param CAN message
	 */
	void processTemperature3(const CAN::Message &message);

private:

	uint8_t txData[drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH] = {};

	static constexpr uint32_t COMMAND_MESSAGE_ID = 0x0C0;
	static constexpr uint8_t TORQUE_SCALE = 10;
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

};

} // namespace drivers::inverter
