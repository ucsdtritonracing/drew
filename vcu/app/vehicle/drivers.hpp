#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/inverter/inverter.hpp"
#include "drivers/sas/sas.hpp"
#include "drivers/pdu/pdu.hpp"
#include "drivers/pedals/pedals.hpp"
#include "drivers/broadcaster/broadcaster.hpp"


namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::pdu::PDU pduDriver;
extern drivers::sas::SAS sasDriver;
extern drivers::inverter::Inverter inverterDriver;
extern drivers::pedals::Pedals pedalsDriver;
extern drivers::broadcaster::Broadcaster broadcasterDriver;

} // namespace vehicle
