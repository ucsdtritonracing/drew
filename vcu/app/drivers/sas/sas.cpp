#include "drivers/sas/sas.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "vehicle/types/steering_types.hpp"
#include "vehicle/vehicle_state.hpp"


namespace drivers::sas {

SAS::SAS(drivers::can::CANBus& canBus)
		: drivers::can::CANPeripheral<SAS>(canBus) {}

void SAS::init() {
	bindHandler<&SAS::processCANMessage>(CAN_ID_STATUS);
	txData[1] = 0x00;		// unchanged byte

	configCommandSlotHandle = bindTxStream(CAN_ID_CONFIG, drivers::can::TxPriority::STATUS);
	resetAngleCommandSlotHandle = bindTxStream(RESET_CALIBRATION_CCW, drivers::can::TxPriority::STATUS);
	resetCalibrationCommandSlotHandle = bindTxStream(RESET_ANGLE_CCW, drivers::can::TxPriority::STATUS);

}

void SAS::resetAngle() {
    txData[0] = RESET_ANGLE_CCW;
    canBus.publishTxStream(resetAngleCommandSlotHandle, txData, 2);
}


void SAS::resetCalibration() {
    txData[0] = RESET_CALIBRATION_CCW;
    canBus.publishTxStream(configCommandSlotHandle, txData, 2);
}

constexpr vehicle::steering::Mode SAS::parseMode(const uint8_t& modeByte) {
	switch (modeByte) {
	case MODE_CALIBRATED_VALID:
		return vehicle::steering::Mode::CALIBRATED_VALID;
	case MODE_UNCALIBRATED_VALID:
		return vehicle::steering::Mode::UNCALIBRATED_VALID;
	case MODE_FAILURE:
		return vehicle::steering::Mode::FAILURE;
	default:
		return vehicle::steering::Mode::INVALID;
	}
}

void SAS::processCANMessage(const drivers::can::Message& message) {
	vehicle::steering::State state;

	if (message.numBytes <= ANGLE_HIGH_BYTE ) return;
	state.angleDegrees = (static_cast<uint16_t>(message.data[ANGLE_HIGH_BYTE]) << 8)
							| static_cast<uint16_t>(message.data[ANGLE_LOW_BYTE]);

	if (message.numBytes <= SPEED_BYTE) return;
	state.speedDegreesPerSecond = message.data[SPEED_BYTE];

	if (message.numBytes <= MODE_BYTE) return;
	state.mode = parseMode(message.data[MODE_BYTE]);

	vehicle::vehicleState.setSteering(state);
}

} // namespace drivers::sas
