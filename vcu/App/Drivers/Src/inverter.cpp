#include "inverter.hpp"
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::inverter {
	Inverter::Inverter(drivers::CAN::CANBus &canBus) :
			drivers::CAN::CANPeripheral<Inverter, State>(canBus) {
		bindHandler<&Inverter::processTemperature1>(CAN_ID_TEMP_1);
		bindHandler<&Inverter::processTemperature2>(CAN_ID_TEMP_2);
		bindHandler<&Inverter::processTemperature3>(CAN_ID_TEMP_3);
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
		state.temperatures.moduleA = ((uint16_t)message.data[1] << 8) | message.data[0];
		state.temperatures.moduleB = ((uint16_t)message.data[3] << 8) | message.data[2];
		state.temperatures.moduleC = ((uint16_t)message.data[5] << 8) | message.data[4];
		state.temperatures.gateDriverBoard = ((uint16_t)message.data[7] << 8) | message.data[6];
	}

	void Inverter::processTemperature2(const CAN::Message &message) {
		state.temperatures.controlBoard = ((uint16_t)message.data[1] << 8) | message.data[0];
		state.temperatures.rtd1 = ((uint16_t)message.data[3] << 8) | message.data[2];
		state.temperatures.rtd2 = ((uint16_t)message.data[5] << 8) | message.data[4];
		state.temperatures.rtd3 = ((uint16_t)message.data[7] << 8) | message.data[6];
	}

	void Inverter::processTemperature3(const CAN::Message &message) {
		state.temperatures.coolant = ((uint16_t)message.data[1] << 8) | message.data[0];
		state.temperatures.hotSpot = ((uint16_t)message.data[3] << 8) | message.data[2];
		state.temperatures.motor = ((uint16_t)message.data[5] << 8) | message.data[4];
		state.torqueShudder = ((uint16_t)message.data[7] << 8) | message.data[6];
	}

} // namespace drivers::inverter
/*
 * static constexpr size_t NUM_TEMPS = 4;
 * std::array<int16_t, NUM_TEMPS> Inverter::processTemperature(const CAN::Message &message) {
		std::array<int16_t, NUM_TEMPS> temperatures;
		for (int i = 0; i < NUM_TEMPS; i++) {
			temperatures[i] = (int16_t)(((uint16_t)message.data[i*2+1] << 8) | message.data[i*2]);
		}
		return temperatures;
	}
 */
