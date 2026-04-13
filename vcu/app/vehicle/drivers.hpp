#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/inverter/inverter.hpp"
#include "drivers/sas/sas.hpp"
#include "drivers/pdu/pdu.hpp"
#include "drivers/pedals/pedals.hpp"


namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::pdu::PDU pdu;
extern drivers::sas::SAS sas;
extern drivers::inverter::Inverter inverter;
extern drivers::pedals::Pedals pedals;

} // namespace vehicle
