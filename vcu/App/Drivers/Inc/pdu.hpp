#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::pdu {
constexpr size_t NUM_CHANNELS = 8;
struct State {
	uint16_t measuredCurrent[NUM_CHANNELS];
	enum ErrorStatus {
		OK, OPEN_CIRCUIT, CURRENT_LIMIT_EXCEEDED, SHORT_CIRCUIT, UNKNOWN
	} errorStatuses[NUM_CHANNELS];
	uint8_t requestedCurrentLimit[drivers::can::MAX_CLASSICAL_CAN_DATA_LENGTH] = {};
	uint8_t requestedPWMDutyPercent[drivers::can::MAX_CLASSICAL_CAN_DATA_LENGTH] = {};
};

class PDU: public drivers::can::CANPeripheral<PDU, State> {
public:
	PDU(drivers::can::CANBus &canBus);

	/*
	 * @brief Set the current limit for a given channel.
	 *
	 * @param channel
	 * @param amps Current in Amperes
	 */
	void setCurrentLimit(uint8_t channel, float amps);

	/*
	 * @brief Set the PWM duty cycle for a given channel.
	 *
	 * @param channel
	 * @param dutyCyclePercent Duty cycle in percent (0-100)
	 */
	void setPWMDutyCycle(uint8_t channel, uint8_t dutyCyclePercent);

	/*
	 * @brief Process an incoming message with message ID of TxMessage1
	 *
	 * @param message
	 */
	void processMessage1(const can::Message &message);
	/*
	 * @brief Process an incoming message with message ID of TxMessage2
	 *
	 * @param message
	 */
	void processMessage2(const can::Message &message);
	/*
	 * @brief Shut off all output from PDU
	 *
	 * @param message
	 */
	void stopAllChannels();

private:
	// the helper function to make code shorter
	void processMessage(int channelStart, const can::Message &message);

	uint8_t txData[drivers::can::MAX_CLASSICAL_CAN_DATA_LENGTH] = {};
	static constexpr uint8_t CHANNEL_MASK = 0b01100110;

	static constexpr uint8_t LOW_CURRENT_LIMIT = 		10;
	static constexpr uint8_t HIGH_CURRENT_LIMIT = 		20;
	static constexpr uint32_t PDU_BIT_TO_POWER_SCALE = 	2.5; // need 2.5 bits per unit increase in duty/current
	static constexpr uint8_t PDU_MAX_PWM = 				100;
	static constexpr uint32_t CAN_ID_SET_PWM = 			0x000A0630;
	static constexpr uint32_t CAN_ID_SET_CURRENT = 		0x000A0620;
	static constexpr uint32_t CAN_ID_RX_1 = 			0x000A0610;
	static constexpr uint32_t CAN_ID_RX_2 = 			0x000A0611;
	static constexpr uint8_t ERROR_MASK = 				0b11100000;
	static constexpr uint16_t CURRENT_MASK = 			0b0000001111111111;
	static constexpr int RX_1_CHANNEL_BEGIN = 			0;
	static constexpr int RX_2_CHANNEL_BEGIN = 			4;
};

} // namespace drivers::pdu
