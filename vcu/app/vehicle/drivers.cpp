#include "vehicle/drivers.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/sas/sas.hpp"
#include "drivers/pdu/pdu.hpp"
#include "drivers/pedals/pedals.hpp"
#include "drivers/broadcaster/broadcaster.hpp"
#include "drivers/sound/sound.hpp"
#include "drivers/wheels/wheels.hpp"


namespace vehicle {

drivers::can::CANBus CANBus1{};
drivers::can::CANBus CANBus2{};
drivers::pdu::PDU pduDriver(CANBus1);
drivers::sas::SAS sasDriver(CANBus1);
drivers::inverter::Inverter inverterDriver(CANBus1);
drivers::pedals::Pedals pedalsDriver;
drivers::broadcaster::Broadcaster broadcasterDriver(CANBus1);
drivers::sound::Sound soundDriver;
drivers::wheels::Wheels wheelsDriver;

} // namespace vehicle
