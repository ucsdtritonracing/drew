#include "vehicle/types/configuration_types.hpp"
#include "vehicle/torque.hpp"


namespace vehicle {

bool PedalsConfig::valid() const {
	if (!app1Thresholds.faultThresholds.validRange()  ||
		!app1Thresholds.signalThresholds.validRange() ||
		!app2Thresholds.faultThresholds.validRange()  ||
		!app2Thresholds.signalThresholds.validRange()) {
		return false;
	}
	if (!bsefThresholds.faultThresholds.validRange()  ||
		!bsefThresholds.signalThresholds.validRange() ||
		!bserThresholds.faultThresholds.validRange()  ||
		!bserThresholds.signalThresholds.validRange()) {
		return false;
	}
	if (bsefBrakeEngagedThreshold < 0 || bsefBrakeEngagedThreshold > 1 ||
		bserBrakeEngagedThreshold < 0 || bserBrakeEngagedThreshold > 1) {
		return false;
	}
	return true;
}

bool WheelsConfig::valid() const {
	if (frontWheelMetersPerRevolution <= 0 || rearWheelMetersPerRevolution <= 0) {
		return false;
	}
	if (wheelSpeedSensorAlpha <= 0 || wheelSpeedSensorAlpha > 1) {
		return false;
	}
	return true;
}

bool TorqueConfig::valid() const {
	if (maxTorqueNm < 0 || maxTorqueNm > torque::MAX_TORQUE_LIMIT_NM) {
		return false;
	}
	return true;
}

bool PDUConfig::valid() const {
	if (PDU_12V_RIGHT_CHANNEL.channel == 0 || PDU_12V_RIGHT_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}
	if (PDU_12V_MAIN_CHANNEL.channel == 0 || PDU_12V_MAIN_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}
	if (PDU_TSB_FANS_CHANNEL.channel == 0 || PDU_TSB_FANS_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}
	if (PDU_RADIATOR_FANS_CHANNEL.channel == 0 || PDU_RADIATOR_FANS_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}
	if (PDU_PUMPS_CHANNEL.channel == 0 || PDU_PUMPS_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}
	if (PDU_BRAKE_LIGHT_CHANNEL.channel == 0 || PDU_BRAKE_LIGHT_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}
	if (PDU_UNUSED_CHANNEL.channel == 0 || PDU_UNUSED_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}
	if (PDU_12V_LEFT_CHANNEL.channel == 0 || PDU_12V_LEFT_CHANNEL.channel > vehicle::pdu::NUM_CHANNELS) {
	    return false;
	}

	return true;
}

} // namespace vehicle
