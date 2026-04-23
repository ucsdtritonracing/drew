#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "vehicle/types/steering_types.hpp"
#include "generics/snapshot.hpp"
#include "stm32g4xx_hal.h"


namespace drivers::sas {

class SAS : public drivers::can::CANPeripheral<SAS> {
public:
    SAS(drivers::can::CANBus& canBus);

    /*
     * @brief Initialize the SAS driver.
     */
    void init();

    /*
     * @brief Reset the SAS angle reading to 0.
     */
    void resetAngle();

    /*
     * @brief Reset the SAS calibration status.
     */
    void resetCalibration();

    /*
     * @brief Process an incoming CAN Message with ID of CAN_ID_STATUS
     *
     * @param message
     */
    void processCANMessage(const can::Message& message);

private:
    constexpr vehicle::steering::Mode parseMode(const uint8_t& modeByte);

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
