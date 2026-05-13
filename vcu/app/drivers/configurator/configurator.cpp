#include "drivers/configurator/configurator.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/vehicle_configuration.hpp"

namespace drivers::configurator {

Configurator::Configurator(drivers::can::CANBus& canBus)
    : canBus(canBus)
{
}

void Configurator::init() {
	bindHandler<&Configurator::processCommandPedal1>(CAN_ID_COMMAND_PEDAL_1);
	bindHandler<&Configurator::processCommandPedal2>(CAN_ID_COMMAND_PEDAL_2);
	bindHandler<&Configurator::processCommandAPPS>(CAN_ID_COMMAND_APPS);
	bindHandler<&Configurator::processCommandBrakePedal>(CAN_ID_COMMAND_BRAKE_PEDAL);

}

void Configurator::processCommandPedal1(const drivers::can::Message& message) {
	if (message.numBytes != 8) {
		return; // incorrect number of bytes received, bad message
	}

}

void Configurator::processCommandPedal2(const drivers::can::Message& message) {
	if (message.numBytes != 8) {
		return; // incorrect number of bytes received, bad message
	}
}

void Configurator::processCommandAPPS(const drivers::can::Message& message) {
	if (message.numBytes != 8) {
		return; // incorrect number of bytes received, bad message
	}
}

void Configurator::processCommandBrakePedal(const drivers::can::Message& message) {
	if (message.numBytes != 8) {
		return; // incorrect number of bytes received, bad message
	}
}


} // namespace drivers::configurator








