#pragma once

#include "can_bus.hpp"
#include "pdu.hpp"
#include "steering_angle_sensor.hpp"
#include "inverter.hpp"
#include "imu.hpp"


namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::pdu::PDU pdu;
extern drivers::sas::SteeringAngleSensor sas;
extern drivers::inverter::Inverter inverter;
extern drivers::imu::IMU imu;

} // namespace vehicle
