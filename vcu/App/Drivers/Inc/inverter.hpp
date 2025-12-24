#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include <array>

namespace drivers::inverter {

enum class Temperatures : uint8 {
	ModuleA, ModuleB, ModuleC, GateDriverBoard, ControlBoard,
	RTD1, RTD2, RTD3, Coolant, HotSpot, Motor, TorqueShudder, Count
};

enum class MotorPosition : uint8 {
	MotorAngle, MotorSpeed, ElectricalOutputFrequency, DeltaResolverFiltered, Count
};

struct State {
	std::array<int16_t, static_cast<size_t>(Temperatures::Count)> temperatures;
	std::array<int16_t, static_cast<size_t>(MotorPosition::Count)> motorPosition;
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

	static constexpr size_t NUM_DATA = 4;
	static constexpr int DATA_1_BEGIN = 0;
	static constexpr int DATA_2_BEGIN = 4;
	static constexpr int DATA_3_BEGIN = 8;

	static constexpr uint32_t CAN_ID_TEMP_1 = 0x0A0;
	static constexpr uint32_t CAN_ID_TEMP_2 = 0x0A1;
	static constexpr uint32_t CAN_ID_TEMP_3 = 0x0A2;

	static constexpr uint32_t CAN_ID_MOTOR_POSITION = 0x0A5;

	template <size_t dataLength>
	void processStandardMessage(const CAN::Message &message, std::array<int16_t, dataLength> &data, int dataStart) {
		for (int i = 0; i < NUM_DATA; i++) {
			data[dataStart + i] = ((uint16_t)message.data[i*2+1] << 8) | message.data[i*2];
		}
	}

};

} // namespace drivers::inverter
