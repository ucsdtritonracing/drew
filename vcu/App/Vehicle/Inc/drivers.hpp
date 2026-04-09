#pragma once

#include "can_bus.hpp"
#include "pdu.hpp"
#include "steering_angle_sensor.hpp"


namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::pdu::PDU pdu;
extern drivers::sas::SteeringAngleSensor sas;

} // namespace vehicle
