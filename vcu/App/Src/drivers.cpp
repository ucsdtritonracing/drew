#include "drivers.hpp"
#include "can_bus.hpp"
#include "pdu.hpp"
#include "steering_angle_sensor.hpp"

drivers::can::CANBus CANBus1{};
drivers::can::CANBus CANBus2{};
drivers::pdu::PDU pdu{};
drivers::sas::SteeringAngleSensor sas{};
