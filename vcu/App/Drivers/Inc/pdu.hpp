#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"

namespace drivers::PDU {

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
    void processMessage1(CAN::Message& message);

	/*
	 * @brief Process an incoming message with message ID of TxMessage2
	 *
	 * @param message
	 */
    void processMessage2(CAN::Message& message);

    static constexpr size_t NUM_CHANNELS = 8;

private:
    uint8_t txData[drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH];
};

struct State {
	uint16_t measuredCurrent[PDU::NUM_CHANNELS];
	enum ErrorStatus {
		OK,
		OPEN_CIRCUIT,
		CURRENT_LIMIT_EXCEEDED,
		SHORT_CIRCUIT
	} errorStatuses[PDU::NUM_CHANNELS];
};

} // namespace drivers::PDU
