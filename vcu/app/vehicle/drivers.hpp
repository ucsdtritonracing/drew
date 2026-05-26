#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/inverter/inverter.hpp"
#include "drivers/sas/sas.hpp"
#include "drivers/pdu/pdu.hpp"
#include "drivers/pedals/pedals.hpp"
#include "drivers/broadcaster/broadcaster.hpp"
#include "drivers/sound/sound.hpp"
#include "drivers/configurator/configurator.hpp"
#include "drivers/wheels/wheels.hpp"



namespace vehicle {

extern drivers::can::CANBus CANBus1;
extern drivers::can::CANBus CANBus2;
extern drivers::pdu::PDU pduDriver;
extern drivers::sas::SAS sasDriver;
extern drivers::inverter::Inverter inverterDriver;
extern drivers::pedals::Pedals pedalsDriver;
extern drivers::broadcaster::Broadcaster broadcasterDriver;
extern drivers::sound::Sound soundDriver;
extern drivers::configurator::Configurator configuratorDriver;
extern drivers::wheels::Wheels wheelsDriver;

} // namespace vehicle
