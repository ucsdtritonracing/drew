#include "steering_angle_sensor.hpp"
#include "can_utils.hpp"

namespace drivers::SAS {

SteeringAngleSensor::SteeringAngleSensor(FDCAN_HandleTypeDef* fdcan_)
    : fdcan(fdcan_)
{
    // Common TxHeader setup
    txHeader.Identifier = CAN_ID_CONFIG;
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.DataLength = FDCAN_DLC_BYTES_2;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;
    txData[1] = 0x00;		// unchanged byte
}

void SteeringAngleSensor::resetAngle() {
    txData[0] = RESET_ANGLE_CCW;
    drivers::CAN::transmit(fdcan, &txHeader, txData);
}


void SteeringAngleSensor::resetCalibration() {
    txData[0] = RESET_CALIBRATION_CCW;
    drivers::CAN::transmit(fdcan, &txHeader, txData);
}

} // namespace drivers::SAS
