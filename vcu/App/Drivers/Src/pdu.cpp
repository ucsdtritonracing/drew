#include "pdu.hpp"
#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include <cmath>

namespace drivers::PDU{
PDU::PDU(drivers::CAN::CANBus& canBus)
		: drivers::CAN::CANPeripheral<PDU, State>(canBus)
{
	bindHandler<&PDU::processMessage1>(CAN_ID_RX_1);
	bindHandler<&PDU::processMessage2>(CAN_ID_RX_2);
	for(int i=0;i<8;i++){
		txData[i] =0x00; // init everything to zero
	}

}

void PDU::setCurrentLimit(uint8_t channel, float amps)
{
	if(amps<0){
		amps*=-1; // check for negative and reverse it just in case
	}
	uint8_t temp = static_cast<uint8_t>(std::round(amps*2.5));// bit/current scale for current
	if (temp < 0){
		 temp = 0;
	}
	if (temp > 255){
		 temp = 255;
	}
	txData[channel-1] = static_cast<uint8_t>(temp); // we only have one byte, and its an int
	canBus.transmit(CAN_ID_SET_CURRENT, txData, FDCAN_DLC_BYTES_8);
}

void PDU::setPWMDutyCycle(uint8_t channel, uint8_t dutyCyclePercent)
{
	/*
	 *  if we for some reason switch from PWM control to CAN control
	 *  let's not mix up the transmitted data
	 */
	uint8_t temp = static_cast<uint8_t>(std::round(dutyCyclePercent*2.5));// approx	bit/duty scale for PWM
		if (temp < 0){
			 temp = 0;
		}
		if (temp > 255){
			 temp = 255;
		}
	txPWMData[channel-1] = temp;
	canBus.transmit(CAN_ID_SET_PWM, txPWMData, FDCAN_DLC_BYTES_8);
}

void PDU::processmessage(int channelStart, const CAN::Message& message){
		for(int i = 0;i<8;i+=2){
			uint8_t errorCode = ((message.data[i] & errorMask) >> 5);
			switch(errorCode){
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
			}
			state.measuredCurrent[channelStart] = ((message.data[i]<<8) | message.data[i+1]) & currentMask;
			channelStart++;
		}
}

void PDU::processMessage1(const CAN::Message& message)
{
	processmessage(0,message);
}
void PDU::processMessage2(const CAN::Message& message)
{
	processmessage(4,message);

}




}
