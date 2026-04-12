#include "vehicle/drivers.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/pdu/pdu.hpp"
#include "drivers/pedals/pedals.hpp"
#include "drivers/sas/steering_angle_sensor.hpp"


namespace vehicle {

drivers::can::CANBus CANBus1{};
drivers::can::CANBus CANBus2{};
drivers::pdu::PDU pdu(CANBus1);
drivers::sas::SteeringAngleSensor sas(CANBus1);
drivers::inverter::Inverter inverter(CANBus1);
drivers::pedals::Pedals pedals;

} // namespace vehicle
