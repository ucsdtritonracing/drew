#include "pdu.hpp"
#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include <cmath>
#include <algorithm>
#include <cstring>

namespace drivers::pdu {

PDU::PDU(drivers::can::CANBus &canBus) :
		drivers::can::CANPeripheral<PDU, State>(canBus) {}

void PDU::init() {
	bindHandler<&PDU::processMessage1>(CAN_ID_RX_1);
	bindHandler<&PDU::processMessage2>(CAN_ID_RX_2);
}

void PDU::setCurrentLimit(uint8_t channel, float amps) {
	float PDU_MAX_CURRENT;
	if (amps < 0) {
		return; // how did we get here?
	}
	if (channel != 0 && channel <= NUM_CHANNELS) { // check channel in range
		PDU_MAX_CURRENT = CHANNEL_MASK & (1 << (channel - 1)) ? LOW_CURRENT_LIMIT : HIGH_CURRENT_LIMIT; // set to 10A if 1, 20 if 0
		state.requestedCurrentLimit[channel - 1] = std::min(amps, PDU_MAX_CURRENT);
		for (int i = 0; i < NUM_CHANNELS; i++) {
			txData[i] = static_cast<uint8_t>(
				std::round(state.requestedCurrentLimit[i] * PDU_BIT_TO_POWER_SCALE)
				);
		}
	}
}
void PDU::setPWMDutyCycle(uint8_t channel, uint8_t dutyCyclePercent) {
	if (channel != 0 && channel <= NUM_CHANNELS) {
		state.requestedPWMDutyPercent[channel - 1] = std::min(dutyCyclePercent, PDU_MAX_PWM);
		for (int i = 0; i < NUM_CHANNELS; i++) {
			txData[i] = static_cast<uint8_t>(
				std::round(state.requestedPWMDutyPercent[i] * PDU_BIT_TO_POWER_SCALE)
				);
		}
	}
}

void PDU::sendCommand(CommandMode mode) {
	switch (mode) {
	case CurrentLimit:
		memcpy(txData, state.requestedCurrentLimit, drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH);
		canBus.transmit(CAN_ID_SET_CURRENT, txData, FDCAN_DLC_BYTES_8);
		break;
	case PWM:
		memcpy(txData, state.requestedCurrentLimit, drivers::CAN::MAX_CLASSICAL_CAN_DATA_LENGTH);
		canBus.transmit(CAN_ID_SET_PWM, txData, FDCAN_DLC_BYTES_8);
		break;
	default:
		return; // bad mode, reject
	}
}

void PDU::processMessage(int channelStart, const can::Message &message) {
	if (message.numBytes != 8) {
		return; // incorrect number of bytes received, bad message
	}
	for (int i = 0; i < NUM_CHANNELS; i += 2) {
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
}

void PDU::processMessage1(const can::Message &message) {
	processMessage(RX_1_CHANNEL_BEGIN, message);
}
void PDU::processMessage2(const can::Message &message) {
	processMessage(RX_2_CHANNEL_BEGIN, message);
}

void PDU::stopAllChannels() {
	memset(state.requestedCurrentLimit, 0, NUM_CHANNELS); // set all bytes to 0
	memset(state.requestedPWMDutyPercent, 0, NUM_CHANNELS);
	memset(txData, 0, NUM_CHANNELS); // set all bytes to 0
	canBus.transmit(CAN_ID_SET_CURRENT, txData, FDCAN_DLC_BYTES_8);
	canBus.transmit(CAN_ID_SET_PWM, txData, FDCAN_DLC_BYTES_8); // current command won't shut off outputs by itself if PWM commands are present
}

} // namespace drivers::pdu
