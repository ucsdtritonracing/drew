#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/inverter/inverter.hpp"
#include "drivers/pdu/pdu.hpp"
#include "drivers/pedals/pedals.hpp"
#include "drivers/sas/steering_angle_sensor.hpp"


namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::pdu::PDU pdu;
extern drivers::sas::SteeringAngleSensor sas;
extern drivers::inverter::Inverter inverter;
extern drivers::pedals::Pedals pedals;

} // namespace vehicle
