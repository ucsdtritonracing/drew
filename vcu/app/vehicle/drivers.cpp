#include "vehicle/drivers.hpp"


namespace vehicle {

drivers::can::CANBus CANBus1{};
drivers::can::CANBus CANBus2{};
drivers::pdu::PDU pduDriver(CANBus1);
drivers::sas::SAS sasDriver(CANBus1);
drivers::inverter::Inverter inverterDriver(CANBus1);
drivers::pedals::Pedals pedalsDriver;
drivers::broadcaster::Broadcaster broadcasterDriver(CANBus1);
drivers::gpio::Outputs outputsDriver;
drivers::configurator::Configurator configuratorDriver(CANBus1);
drivers::wheels::Wheels wheelsDriver;

} // namespace vehicle
