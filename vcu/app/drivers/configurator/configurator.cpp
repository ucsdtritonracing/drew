#include "drivers/configurator/configurator.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/vehicle_configuration.hpp"


namespace drivers::configurator {

Configurator::Configurator(drivers::can::CANBus& canBus)
    : CANPeripheral(canBus) {}

void Configurator::init() {
	bindHandler<&Configurator::processCommandSetParameter>(CAN_ID_SET_PARAMETER);
	bindHandler<&Configurator::processCommandGetParameter>(CAN_ID_GET_PARAMETER);
	parameterResponseSlotHandle = bindTxSlot(CAN_ID_PARAMETER_RESPONSE, drivers::can::STATUS);
}

Parameter Configurator::parseParameterId(const drivers::can::Message& message) {
	uint16_t rawId = static_cast<uint16_t>(message.data[0]) | (static_cast<uint16_t>(message.data[1]) << 8);

	switch (rawId) {
	case static_cast<Parameter>(Parameter::APP1_THRESHOLD):
	case static_cast<Parameter>(Parameter::APP2_THRESHOLD):
	case static_cast<Parameter>(Parameter::BSEF_THRESHOLD):
	case static_cast<Parameter>(Parameter::BSER_THRESHOLD):
	case static_cast<Parameter>(Parameter::MAXIMUM_TORQUE_REQUEST_NM):
	case static_cast<Parameter>(Parameter::PEDAL_MAP):
		return static_cast<Parameter>(rawId);
	default:
		return Parameter::INVALID;
	}
}

void Configurator::processCommandSetParameter(const drivers::can::Message& message) {
	if (message.numBytes != 8) {
		return;
	}

	const Parameter parameter = parseParameterId(message);

	const uint32_t rawData =	static_cast<uint32_t>(message.data[0]) |
								static_cast<uint32_t>(message.data[1]) << 8 |
								static_cast<uint32_t>(message.data[2]) << 16 |
								static_cast<uint32_t>(message.data[3]) << 24;

	const float data = reinterpret_cast<float>(rawData);

	const uint8_t index	= message.data[6];
	const uint8_t type	= message.data[7];
	const uint16_t bigIndex = static_cast<uint16_t>(message.data[6]) | static_cast<uint16_t>(message.data[7]) << 8;

	switch (parameter) {
	case Parameter::APP1_THRESHOLD:
		switch (index) {
		case ThresholdIndex::FAULT_LOW:
			stagedConfiguration.app1Thresholds.faultThresholds.VoltageRange(min, max)
		}

		break;
	case Parameter::APP2_THRESHOLD:
		break;
	case Parameter::BSEF_THRESHOLD:
		break;
	case Parameter::BSER_THRESHOLD:
		break;
	case Parameter::MAXIMUM_TORQUE_REQUEST_NM:
		break;
	case Parameter::PEDAL_MAP:
		break;
	default:
		return;
	}
}

void Configurator::processCommandGetParameter(const drivers::can::Message& message) {
	if (message.numBytes != 8) {
		return;
	}

	const Parameter parameter = parseParameterId(message);


}

} // namespace drivers::configurator








