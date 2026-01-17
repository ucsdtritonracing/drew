#include "drivers.hpp"
#include "can_bus.hpp"
#include "pdu.hpp"
#include "steering_angle_sensor.hpp"
#include "inverter.hpp"
#include "imu.hpp"


namespace vehicle {

drivers::can::CANBus CANBus1{};
drivers::can::CANBus CANBus2{};
drivers::pdu::PDU pdu(CANBus1);
drivers::sas::SteeringAngleSensor sas(CANBus1);
drivers::inverter::Inverter inverter(CANBus1);
drivers::imu::IMU imu;

} // namespace vehicle
