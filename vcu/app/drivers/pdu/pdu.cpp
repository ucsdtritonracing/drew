#include "drivers/pdu/pdu.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "drivers/can/can_utils.hpp"
#include "vehicle/types/pdu_types.hpp"
#include "vehicle/vehicle_state.hpp"
#include "generics/snapshot.hpp"
#include <cmath>
#include <algorithm>
#include <cstring>


namespace drivers::pdu {

PDU::PDU(drivers::can::CANBus &canBus) :
		drivers::can::CANPeripheral<PDU>(canBus) {}

void PDU::init() {
	bindHandler<&PDU::processMessage1>(CAN_ID_RX_1);
	bindHandler<&PDU::processMessage2>(CAN_ID_RX_2);
}

void PDU::setCurrentLimit(uint8_t channel, float amps) {
	if (amps < 0) {
		return;
	}

	if (channel != 0 && channel <= vehicle::pdu::NUM_CHANNELS) {
		const float PDU_MAX_CURRENT = CHANNEL_LIMIT_10A_MASK & (1 << (channel - 1)) ? LOW_CURRENT_LIMIT : HIGH_CURRENT_LIMIT; // set to 10A if 1, 20A if 0
		requestedCurrentLimit[channel - 1] = std::min(amps, PDU_MAX_CURRENT);
		for (size_t i = 0; i < vehicle::pdu::NUM_CHANNELS; i++) {
			txData[i] = static_cast<uint8_t>(
				std::round(requestedCurrentLimit[i] * PDU_BIT_TO_POWER_SCALE)
				);
		}
	}
}
void PDU::setPWMDutyCycle(uint8_t channel, uint8_t dutyCyclePercent) {
	if (channel != 0 && channel <= vehicle::pdu::NUM_CHANNELS) {
		requestedPWMDutyPercent[channel - 1] = std::min(dutyCyclePercent, PDU_MAX_PWM);
		for (size_t i = 0; i < vehicle::pdu::NUM_CHANNELS; i++) {
			txData[i] = static_cast<uint8_t>(
				std::round(requestedPWMDutyPercent[i] * PDU_BIT_TO_POWER_SCALE)
				);
		}
	}
}

void PDU::enableChannel(vehicle::pdu::ChannelConfig config) {
	setCurrentLimit(config.channel, config.currentLimitAmps);
}

void PDU::disableChannel(vehicle::pdu::ChannelConfig config) {
	setCurrentLimit(config.channel, 0.0f);
}

void PDU::sendCommand(CommandMode mode) {
	switch (mode) {
	case CurrentLimit:
		memcpy(txData, requestedCurrentLimit, drivers::can::MAX_CLASSICAL_CAN_DATA_LENGTH);
		canBus.transmit(CAN_ID_SET_CURRENT, txData, FDCAN_DLC_BYTES_8);
		break;
	case PWM:
		memcpy(txData, requestedPWMDutyPercent, drivers::can::MAX_CLASSICAL_CAN_DATA_LENGTH);
		canBus.transmit(CAN_ID_SET_PWM, txData, FDCAN_DLC_BYTES_8);
		break;
	}
}

void PDU::processMessage(int channelStart, const can::Message &message) {
	if (message.numBytes != 8) {
		return; // incorrect number of bytes received, bad message
	}

	vehicle::pdu::State state;

	for (size_t i = 0; i < vehicle::pdu::NUM_CHANNELS; i += 2) {
		uint8_t errorCode = ((message.data[i] & ERROR_MASK) >> 5);
		switch (errorCode) {
		case 0x00:
			state.errorStatuses[channelStart] = state.OK;
			break;
		case 0x01:
			state.errorStatuses[channelStart] = state.OPEN_CIRCUIT;
			break;
		case 0x02:
			state.errorStatuses[channelStart] = state.CURRENT_LIMIT_EXCEEDED;
			break;
		case 0x04:
			state.errorStatuses[channelStart] = state.SHORT_CIRCUIT;
			break;
		default:
			state.errorStatuses[channelStart] = state.UNKNOWN;
			break;
		}
		state.measuredCurrent[channelStart] = ((message.data[i] << 8) | message.data[i + 1]) & CURRENT_MASK;
		channelStart++;
	}

	vehicle::vehicleState.setPDUState(state);
}

void PDU::processMessage1(const can::Message &message) {
	processMessage(RX_1_CHANNEL_BEGIN, message);
}
void PDU::processMessage2(const can::Message &message) {
	processMessage(RX_2_CHANNEL_BEGIN, message);
}

void PDU::stopAllChannels() {
	memset(requestedCurrentLimit, 0, vehicle::pdu::NUM_CHANNELS);
	memset(requestedPWMDutyPercent, 0, vehicle::pdu::NUM_CHANNELS);
	memset(txData, 0, vehicle::pdu::NUM_CHANNELS);
	canBus.transmit(CAN_ID_SET_CURRENT, txData, FDCAN_DLC_BYTES_8);
	canBus.transmit(CAN_ID_SET_PWM, txData, FDCAN_DLC_BYTES_8); // current command won't shut off outputs by itself if PWM commands are present
}

} // namespace drivers::pdu
