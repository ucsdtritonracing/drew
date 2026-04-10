#pragma once

#include "can_bus.hpp"
#include "inverter.hpp"
#include "pdu.hpp"
#include "steering_angle_sensor.hpp"
#include "pedals.hpp"


namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::pdu::PDU pdu;
extern drivers::sas::SteeringAngleSensor sas;
extern drivers::inverter::Inverter inverter;
extern drivers::pedals::Pedals pedals;

} // namespace vehicle
