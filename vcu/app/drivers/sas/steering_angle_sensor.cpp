#include "../../driver/can/can_bus.hpp"
#include "../../drivers/sas/steering_angle_sensor.hpp"

#include "../../drivers/can/can_bus.hpp"
#include "../../drivers/can/can_bus.hpp"
#include "../../drivers/can/can_peripheral.hpp"

namespace drivers::sas {

SteeringAngleSensor::SteeringAngleSensor(drivers::can::CANBus& canBus)
		: drivers::can::CANPeripheral<SteeringAngleSensor, State>(canBus) {}

void SteeringAngleSensor::init() {
	bindHandler<&SteeringAngleSensor::processCANMessage>(CAN_ID_STATUS);
	txData[1] = 0x00;		// unchanged byte
}

void SteeringAngleSensor::resetAngle() {
    txData[0] = RESET_ANGLE_CCW;
    canBus.transmit(CAN_ID_CONFIG, txData, FDCAN_DLC_BYTES_2);
}


void SteeringAngleSensor::resetCalibration() {
    txData[0] = RESET_CALIBRATION_CCW;
    canBus.transmit(CAN_ID_CONFIG, txData, FDCAN_DLC_BYTES_2);
}

constexpr State::Mode SteeringAngleSensor::parseMode(const uint8_t& modeByte) {
	switch (modeByte) {
	case MODE_CALIBRATED_VALID:
		return State::CALIBRATED_VALID;
	case MODE_UNCALIBRATED_VALID:
		return State::UNCALIBRATED_VALID;
	case MODE_FAILURE:
		return State::FAILURE;
	default:
		return State::INVALID;
	}
}

void SteeringAngleSensor::processCANMessage(const drivers::can::Message& message) {
	if (message.numBytes <= ANGLE_HIGH_BYTE ) return;
	state.steeringAngle = (static_cast<uint16_t>(message.data[ANGLE_HIGH_BYTE]) << 8)
							| static_cast<uint16_t>(message.data[ANGLE_LOW_BYTE]);

	if (message.numBytes <= SPEED_BYTE) return;
	state.speed = message.data[SPEED_BYTE];

	if (message.numBytes <= MODE_BYTE) return;
	state.mode = parseMode(message.data[MODE_BYTE]);
}

} // namespace drivers::sas
