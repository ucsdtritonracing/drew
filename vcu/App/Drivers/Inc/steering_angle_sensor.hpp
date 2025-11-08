#pragma once
#include "stm32g4xx_hal.h"

namespace drivers::SAS {

class SteeringAngleSensor {
public:
    explicit SteeringAngleSensor(FDCAN_HandleTypeDef* fdcan_);

    SteeringAngleSensor(SteeringAngleSensor&&)					= delete;
    SteeringAngleSensor(const SteeringAngleSensor&) 			= delete;
    SteeringAngleSensor& operator=(const SteeringAngleSensor&) 	= delete;
    SteeringAngleSensor& operator=(SteeringAngleSensor&&)		= delete;

    void resetAngle();
    void resetCalibration();

private:
    FDCAN_HandleTypeDef* fdcan;
    FDCAN_TxHeaderTypeDef txHeader;
    uint8_t txData[2];

    static constexpr uint32_t	CAN_ID_CONFIG			= 0x7C0;	// "CAN ID 02 0x7C0 LWS_Config"
    static constexpr uint8_t	RESET_ANGLE_CCW			= 0x03;		// "3h Sets the signal LWS_Angle to 0"
    static constexpr uint8_t 	RESET_CALIBRATION_CCW	= 0x05;		// "5h Resets the calibration status of the angle"
};

} // namespace drivers::SAS
