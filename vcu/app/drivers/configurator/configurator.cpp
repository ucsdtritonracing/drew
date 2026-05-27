#include "drivers/configurator/configurator.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/types/configuration_types.hpp"
#include "vehicle/torque.hpp"
#include <string.h>


namespace drivers::configurator {

Configurator::Configurator(drivers::can::CANBus& canBus)
    : CANPeripheral(canBus) {}


void Configurator::init() {
	bindHandler<&Configurator::processCommandSetParameter>(CAN_ID_SET_PARAMETER);
	bindHandler<&Configurator::processCommandWriteConfiguration>(CAN_ID_WRITE_CONFIGURATION);
	bindHandler<&Configurator::processCommandConfigurationMode>(CAN_ID_CONFIGURATION_MODE);
	bindHandler<&Configurator::processCommandSetThrottleLow>(CAN_ID_CONFIGURE_THROTTLE_LOW);
	bindHandler<&Configurator::processCommandSetThrottleMax>(CAN_ID_CONFIGURE_THROTTLE_HIGH);
	bindHandler<&Configurator::processCommandSetSASZero>(CAN_ID_CONFIGURE_SAS_ZERO);
	bindHandler<&Configurator::processCommandSetBPSEngaged>(CAN_ID_CONFIGURE_BRAKE_ENGAGE);

	parameterResponseSlotHandle = bindTxSlot(CAN_ID_PARAMETER_RESPONSE, drivers::can::STATUS);
}


void Configurator::setMaxTorque(float value) {
	if (value < 0 || value > torque::MAX_TORQUE_LIMIT_NM) {
		return;
	}
	stagedConfiguration.maxTorqueNm = value;
}


bool Configurator::requestingConfigurationMode() const {
	return configurationModeRequested;
}


void Configurator::processCommandWriteConfiguration(const drivers::can::Message& message) {
	if (message.numBytes != WRITE_CONFIGURATION_MESSAGE_NUM_BYTES) {
		return;
	}

	if (stagedConfiguration.valid() && vehicle::vehicleState.getMode() == vehicle::Mode::CONFIGURATION) {
		vehicle::vehicleConfiguration = stagedConfiguration;
	}
}


void Configurator::processCommandConfigurationMode(const drivers::can::Message& message) {
	if (message.numBytes != CONFIGURATION_MODE_MESSAGE_NUM_BYTES) {
		return;
	}
	configurationModeRequested = static_cast<bool>(message.data[0]);
}

void Configurator::processCommandSetThrottleLow(const drivers::can::Message& message){
	if (message.numBytes != AUTO_CONFIGURATION_MESSAGE_NUM_BYTES) {
			return;
	}
	vehicle::pedals::State pedalState = vehicle::vehicleState.getPedals();
	if(!pedalState.app1Valid || !pedalState.app2Valid){
		return;
	} else {
		setThreshold(Parameter::APP2_THRESHOLD, ThresholdType::SIGNAL_LOW, pedalState.app2);
		setThreshold(Parameter::APP1_THRESHOLD, ThresholdType::SIGNAL_LOW, pedalState.app1);
	}
}

void Configurator::processCommandSetThrottleMax(const drivers::can::Message& message){
	if (message.numBytes != AUTO_CONFIGURATION_MESSAGE_NUM_BYTES) {
			return;
	}
	vehicle::pedals::State pedalState = vehicle::vehicleState.getPedals();
	if(!pedalState.bsefValid || !pedalState.bserValid){
		return;
	} else {
		setThreshold(Parameter::APP2_THRESHOLD, ThresholdType::SIGNAL_HIGH, pedalState.app2);
		setThreshold(Parameter::APP1_THRESHOLD, ThresholdType::SIGNAL_HIGH, pedalState.app1);
	}
}

void Configurator::processCommandSetBPSEngaged(const drivers::can::Message& message){
	if (message.numBytes != AUTO_CONFIGURATION_MESSAGE_NUM_BYTES) {
			return;
	}
	vehicle::pedals::State pedalState = vehicle::vehicleState.getPedals();
	if(!pedalState.app1Valid || !pedalState.app2Valid){
		return;
	} else {
		setThreshold(Parameter::BSER_THRESHOLD, ThresholdType::ENGAGE, pedalState.bser);
		setThreshold(Parameter::BSEF_THRESHOLD, ThresholdType::ENGAGE, pedalState.bsef);
	}
}

void Configurator::processCommandSetParameter(const drivers::can::Message& message) {
	if (vehicle::vehicleState.getMode() != vehicle::Mode::CONFIGURATION) {
		return;
	}

	if (message.numBytes != PARAMETER_MESSAGE_NUM_BYTES) {
		return;
	}

	const std::optional<Parameter> parameter = parseParameterId(message);
	if (!parameter) {
		return;
	}

	const uint32_t rawData =	static_cast<uint32_t>(message.data[PARAMETER_MESSAGE_DATA_START])			|
								static_cast<uint32_t>(message.data[PARAMETER_MESSAGE_DATA_START + 1]) << 8	|
								static_cast<uint32_t>(message.data[PARAMETER_MESSAGE_DATA_START + 2]) << 16 |
								static_cast<uint32_t>(message.data[PARAMETER_MESSAGE_DATA_START + 3]) << 24;
	float value = unscale(rawData);

	const uint16_t info =	static_cast<uint16_t>(message.data[PARAMETER_MESSAGE_INFO_START]) |
							static_cast<uint16_t>(message.data[PARAMETER_MESSAGE_INFO_START + 1]) << 8;

	switch (*parameter) {
	case Parameter::MAX_TORQUE_REQUEST: {
		setMaxTorque(value);
		break;
	}
	case Parameter::PEDAL_MAP: {
		stagedConfiguration.pedalMap.setPoint(info, value);
		break;
	}
	case Parameter::APP1_THRESHOLD:
	case Parameter::APP2_THRESHOLD:
	case Parameter::BSEF_THRESHOLD:
	case Parameter::BSER_THRESHOLD: {
		std::optional<ThresholdType> type = parseThresholdType(info);
		if (type.has_value()) {
			setThreshold(*parameter, *type, value);
		}
		break;
	}
	default:
		break;
	}
}


void Configurator::setThreshold(Parameter parameter, ThresholdType type, float value) {
	vehicle::AnalogCalibration* calibration = nullptr;

	switch (parameter) {
	case Parameter::APP1_THRESHOLD:
		calibration = &stagedConfiguration.app1Thresholds;
		break;
	case Parameter::APP2_THRESHOLD:
		calibration = &stagedConfiguration.app2Thresholds;
		break;
	case Parameter::BSEF_THRESHOLD:
		if (type == ThresholdType::ENGAGE) {
			stagedConfiguration.bsefBrakeEngagedThreshold = value;
			return;
		}
		calibration = &stagedConfiguration.bsefThresholds;
		break;
	case Parameter::BSER_THRESHOLD:
		if (type == ThresholdType::ENGAGE) {
			stagedConfiguration.bserBrakeEngagedThreshold = value;
			return;
		}
		calibration = &stagedConfiguration.bserThresholds;
		break;
	default:
		return;
	}

	vehicle::Range* range = nullptr;
	if (type == ThresholdType::FAULT_LOW || type == ThresholdType::FAULT_HIGH) {
		range = &calibration->faultThresholds;
	} else {
		range = &calibration->signalThresholds;
	}

	if (type == ThresholdType::FAULT_HIGH || type == ThresholdType::SIGNAL_HIGH) {
		*range = {range->getMin(), value};
	} else {
		*range = {value, range->getMax()};
	}
}


void Configurator::sendThresholdRanges(Parameter parameter) {
	vehicle::AnalogCalibration* calibration = nullptr;
	switch (parameter) {
	case Parameter::APP1_THRESHOLD:
		calibration = &stagedConfiguration.app1Thresholds;
		break;
	case Parameter::APP2_THRESHOLD:
		calibration = &stagedConfiguration.app2Thresholds;
		break;
	case Parameter::BSEF_THRESHOLD:
		calibration = &stagedConfiguration.bsefThresholds;
		break;
	case Parameter::BSER_THRESHOLD:
		calibration = &stagedConfiguration.bserThresholds;
		break;
	default:
		return;
	}
	sendParameterValue(parameter, static_cast<uint16_t>(ThresholdType::FAULT_LOW), calibration->faultThresholds.getMin());
	sendParameterValue(parameter, static_cast<uint16_t>(ThresholdType::FAULT_HIGH), calibration->faultThresholds.getMax());
	sendParameterValue(parameter, static_cast<uint16_t>(ThresholdType::SIGNAL_LOW), calibration->signalThresholds.getMin());
	sendParameterValue(parameter, static_cast<uint16_t>(ThresholdType::SIGNAL_HIGH), calibration->signalThresholds.getMax());
}


void Configurator::sendParameterValue(Parameter parameter, uint16_t info, float data) {
	uint16_t rawParameter = static_cast<uint16_t>(parameter);
	txData[0] = rawParameter;
	txData[1] = rawParameter >> 8;

	txData[2] = info;
	txData[3] = info >> 8;

	uint32_t scaledData = scale(data);
	txData[4] = scaledData;
	txData[5] = scaledData >> 8;
	txData[6] = scaledData >> 16;
	txData[7] = scaledData >> 24;

	canBus.publishTxSlot(parameterResponseSlotHandle, txData, PARAMETER_MESSAGE_NUM_BYTES);
}


void Configurator::broadcastNextConfigurationParameter() {
	if (vehicle::vehicleState.getMode() != vehicle::Mode::CONFIGURATION) {
		return;
	}

	switch (broadcastParameterIndex) {
	// APP1
	case 0:
		sendParameterValue(Parameter::APP1_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_LOW), stagedConfiguration.app1Thresholds.faultThresholds.getMin());
		break;
	case 1:
		sendParameterValue(Parameter::APP1_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_HIGH), stagedConfiguration.app1Thresholds.faultThresholds.getMax());
		break;
	case 2:
		sendParameterValue(Parameter::APP1_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_LOW), stagedConfiguration.app1Thresholds.signalThresholds.getMin());
		break;
	case 3:
		sendParameterValue(Parameter::APP1_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_HIGH), stagedConfiguration.app1Thresholds.signalThresholds.getMax());
		break;

	// APP2
	case 4:
		sendParameterValue(Parameter::APP2_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_LOW), stagedConfiguration.app2Thresholds.faultThresholds.getMin());
		break;
	case 5:
		sendParameterValue(Parameter::APP2_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_HIGH), stagedConfiguration.app2Thresholds.faultThresholds.getMax());
		break;
	case 6:
		sendParameterValue(Parameter::APP2_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_LOW), stagedConfiguration.app2Thresholds.signalThresholds.getMin());
		break;
	case 7:
		sendParameterValue(Parameter::APP2_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_HIGH), stagedConfiguration.app2Thresholds.signalThresholds.getMax());
		break;

	// BSEF
	case 8:
		sendParameterValue(Parameter::BSEF_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_LOW), stagedConfiguration.bsefThresholds.faultThresholds.getMin());
		break;
	case 9:
		sendParameterValue(Parameter::BSEF_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_HIGH), stagedConfiguration.bsefThresholds.faultThresholds.getMax());
		break;
	case 10:
		sendParameterValue(Parameter::BSEF_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_LOW), stagedConfiguration.bsefThresholds.signalThresholds.getMin());
		break;
	case 11:
		sendParameterValue(Parameter::BSEF_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_HIGH), stagedConfiguration.bsefThresholds.signalThresholds.getMax());
		break;
	case 12:
		sendParameterValue(Parameter::BSEF_THRESHOLD, static_cast<uint16_t>(ThresholdType::ENGAGE), stagedConfiguration.bsefBrakeEngagedThreshold);
		break;

	// BSER
	case 13:
		sendParameterValue(Parameter::BSER_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_LOW), stagedConfiguration.bserThresholds.faultThresholds.getMin());
		break;
	case 14:
		sendParameterValue(Parameter::BSER_THRESHOLD, static_cast<uint16_t>(ThresholdType::FAULT_HIGH), stagedConfiguration.bserThresholds.faultThresholds.getMax());
		break;
	case 15:
		sendParameterValue(Parameter::BSER_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_LOW), stagedConfiguration.bserThresholds.signalThresholds.getMin());
		break;
	case 16:
		sendParameterValue(Parameter::BSER_THRESHOLD, static_cast<uint16_t>(ThresholdType::SIGNAL_HIGH), stagedConfiguration.bserThresholds.signalThresholds.getMax());
		break;
	case 17:
		sendParameterValue(Parameter::BSER_THRESHOLD, static_cast<uint16_t>(ThresholdType::ENGAGE), stagedConfiguration.bserBrakeEngagedThreshold);
		break;

	// max torque request
	case 18:
		sendParameterValue(Parameter::MAX_TORQUE_REQUEST, 0, stagedConfiguration.maxTorqueNm);
		break;

	// pedal map
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
		sendParameterValue(Parameter::PEDAL_MAP, broadcastParameterIndex - 18, stagedConfiguration.pedalMap.getPoint(broadcastParameterIndex - 18));
		break;

	default:
		break;
	}

	broadcastParameterIndex = (broadcastParameterIndex + 1) % NUM_BROADCAST_PARAMETER_VALUES;
}


float Configurator::unscale(uint32_t data) {
	return static_cast<float>(static_cast<int32_t>(data)) / PARAMETER_MESSAGE_SCALE;
}


uint32_t Configurator::scale(float value) {
	int32_t scaled = static_cast<int32_t>(value * PARAMETER_MESSAGE_SCALE);
	return static_cast<uint32_t>(scaled);
}


std::optional<Parameter> Configurator::parseParameterId(const drivers::can::Message& message) {
	uint16_t rawId = static_cast<uint16_t>(message.data[0]) | (static_cast<uint16_t>(message.data[1]) << 8);

	switch (rawId) {
	case static_cast<uint16_t>(Parameter::APP1_THRESHOLD):
	case static_cast<uint16_t>(Parameter::APP2_THRESHOLD):
	case static_cast<uint16_t>(Parameter::BSEF_THRESHOLD):
	case static_cast<uint16_t>(Parameter::BSER_THRESHOLD):
	case static_cast<uint16_t>(Parameter::MAX_TORQUE_REQUEST):
	case static_cast<uint16_t>(Parameter::PEDAL_MAP):
		return static_cast<Parameter>(rawId);
	default:
		return std::nullopt;
	}
}


std::optional<ThresholdType> Configurator::parseThresholdType(uint16_t info) {
	switch (info) {
	case static_cast<uint16_t>(ThresholdType::FAULT_LOW):
	case static_cast<uint16_t>(ThresholdType::FAULT_HIGH):
	case static_cast<uint16_t>(ThresholdType::SIGNAL_LOW):
	case static_cast<uint16_t>(ThresholdType::SIGNAL_HIGH):
	case static_cast<uint16_t>(ThresholdType::ENGAGE):
		return static_cast<ThresholdType>(info);
	default:
		return std::nullopt;
	}
}

} // namespace drivers::configurator








