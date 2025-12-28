#pragma once
#include "can_bus.hpp"
#include "can_peripheral.hpp"
#include "stm32g4xx_hal.h"

namespace drivers::sas {

struct State {
	uint16_t steeringAngle;
	uint8_t speed;
	enum Mode {
		CALIBRATED_VALID,
		UNCALIBRATED_VALID,
		FAILURE,
		INVALID
	} mode;
};

class SteeringAngleSensor : public drivers::can::CANPeripheral<SteeringAngleSensor, State> {
public:
    SteeringAngleSensor() = default;

    /*
     * @brief Initialize the SAS driver.
     *
     * @param canBus CAN Bus that the driver will be attached to
     */
    void init(drivers::can::CANBus& canBus);
    void resetAngle();
    void resetCalibration();
    void processCANMessage(const can::Message& message);

private:
    constexpr State::Mode parseMode(const uint8_t& modeByte);

    static constexpr uint32_t	CAN_ID_STATUS			= 0x2B0;	// "CAN ID 01 0x2B0 LWS_Standard"
    static constexpr size_t 	ANGLE_LOW_BYTE			= 0;
    static constexpr size_t 	ANGLE_HIGH_BYTE			= 1;
    static constexpr size_t 	SPEED_BYTE				= 2;
    static constexpr size_t 	MODE_BYTE				= 3;

    static constexpr uint8_t	MODE_CALIBRATED_VALID	= 7;
    static constexpr uint8_t	MODE_UNCALIBRATED_VALID	= 6;
    static constexpr uint8_t	MODE_FAILURE			= 4;

    static constexpr uint32_t	CAN_ID_CONFIG			= 0x7C0;	// "CAN ID 02 0x7C0 LWS_Config"
    static constexpr uint8_t	RESET_ANGLE_CCW			= 0x03;		// "3h Sets the signal LWS_Angle to 0"
    static constexpr uint8_t 	RESET_CALIBRATION_CCW	= 0x05;		// "5h Resets the calibration status of the angle"

    uint8_t txData[2];
};

} // namespace drivers::sas
