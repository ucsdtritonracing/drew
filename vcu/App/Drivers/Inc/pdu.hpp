#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::PDU {
	struct State {
		uint16_t measuredCurrent[8];
		enum ErrorStatus {
			OK,
			OPEN_CIRCUIT,
			CURRENT_LIMIT_EXCEEDED,
			SHORT_CIRCUIT
		} errorStatuses[8];
	};


class PDU : public drivers::CAN::CANPeripheral<PDU, State> {
public:
	PDU(drivers::CAN::CANBus& canBus);

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
    void processMessage1(const CAN::Message& message);
	/*
	 * @brief Process an incoming message with message ID of TxMessage2
	 *
	 * @param message
	 */
    void processMessage2(const CAN::Message& message);


private:
    // the helper function to make code shorter
    void processmessage(int channelStart, const CAN::Message& message);
    uint8_t txData[drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH];
    uint8_t txPWMData[drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH];
    static constexpr uint32_t CAN_ID_SET_PWM = 0x000A0630;
    static constexpr uint32_t CAN_ID_SET_CURRENT = 0x000A0620;
    static constexpr uint32_t CAN_ID_RX_1 = 0x000A0610;
    static constexpr uint32_t CAN_ID_RX_2 = 0x000A0611;
    static constexpr uint8_t errorMask = 0b11100000;
    static constexpr uint16_t currentMask = 0b0000001111111111;
};


} // namespace drivers::PDU
