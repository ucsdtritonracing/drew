#include "pdu.hpp"
#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include <cmath>
#include <algorithm>
#include <cstring>

namespace drivers::PDU {
PDU::PDU(drivers::CAN::CANBus& canBus)
		: drivers::CAN::CANPeripheral<PDU, State>(canBus)
{
	bindHandler<&PDU::processMessage1>(CAN_ID_RX_1);
	bindHandler<&PDU::processMessage2>(CAN_ID_RX_2);
}

void PDU::setCurrentLimit(uint8_t channel, float amps)
{
	if(amps<0) {
		return; // how did we get here?
	}
	if(channel != 0 && channel < NUM_CHANNELS) { // check channel in range
	txData[channel-1] = static_cast<uint8_t>(std::clamp(
			std::round(amps*PDU_BIT_TO_POWER_SCALE),0.0f,255.0f)
	);
	canBus.transmit(CAN_ID_SET_CURRENT, txData, FDCAN_DLC_BYTES_8);
	}
	return;
}

void PDU::setPWMDutyCycle(uint8_t channel, uint8_t dutyCyclePercent)
{
	if(channel != 0 && channel < NUM_CHANNELS) {
	txPWMData[channel-1] = static_cast<uint8_t>(std::clamp(
			std::round(dutyCyclePercent*PDU_BIT_TO_POWER_SCALE),0.0,255.0)
	);
	canBus.transmit(CAN_ID_SET_PWM, txPWMData, FDCAN_DLC_BYTES_8);
	}
	return;
}

void PDU::processmessage(int channelStart, const CAN::Message& message){
	if(message.numBytes != 8) {
		return; // incorrect number of bytes received, bad message
	}
	for(int i = 0;i<NUM_CHANNELS;i+=2) {
		uint8_t errorCode = ((message.data[i] & errorMask) >> 5);
		switch(errorCode) {
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
		state.measuredCurrent[channelStart] = ((message.data[i]<<8) | message.data[i+1]) & currentMask;
		channelStart++;
	}
}

void PDU::processMessage1(const CAN::Message& message)
{
	processmessage(RX_1_CHANNEL_BEGIN,message);
}
void PDU::processMessage2(const CAN::Message& message)
{
	processmessage(RX_2_CHANNEL_BEGIN,message);
}

void PDU::stopAllChannels()
{
	memset(txData, 0, sizeof(txData)); // set all bytes to 0
	memset(txPWMData, 0, sizeof(txPWMData));
	canBus.transmit(CAN_ID_SET_CURRENT, txData, FDCAN_DLC_BYTES_8);
	canBus.transmit(CAN_ID_SET_PWM, txPWMData, FDCAN_DLC_BYTES_8); // current command won't shut off outputs by itself if PWM commands are present
}

}
