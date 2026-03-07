#pragma once

#include "can_bus.hpp"
#include "inverter.hpp"
#include "steering_angle_sensor.hpp"
#include "pedals.hpp"
#include "pdu.hpp"
#include "imu.hpp"


namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::inverter::Inverter inverter;
extern drivers::sas::SteeringAngleSensor sas;
extern drivers::pedals::Pedals pedals;
extern drivers::pdu::PDU pdu;
extern drivers::imu::IMU imu;

} // namespace vehicle
