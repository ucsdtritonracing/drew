#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"

#ifndef DRIVERS_CONFIGURATOR_CONFIGURATOR_HPP_
#define DRIVERS_CONFIGURATOR_CONFIGURATOR_HPP_


namespace drivers::configurator {

class Configurator: public drivers::can::CANPeripheral<Configurator> {
public:
	Configurator(drivers::can::CANBus& canBus); // constructor functions

	void init();

	void processCommandPedal1(const drivers::can::Message& message); //PEDAL MAP 1
	void processCommandPedal2(const drivers::can::Message& message); //PEDAL MAP 2
	void processCommandAPPS(const drivers::can::Message& message); //APPS
	void processCommandBrakePedal(const drivers::can::Message& message); //Brake Pedal


private:
    static constexpr uint32_t CAN_ID_COMMAND_PEDAL_1= 400; // add actuall ids
    static constexpr uint32_t CAN_ID_COMMAND_PEDAL_2 = 401;
    static constexpr uint32_t CAN_ID_COMMAND_APPS = 402;
    static constexpr uint32_t CAN_ID_COMMAND_BRAKE_PEDAL = 403;



};// Class

}// namespace

#endif /* DRIVERS_CONFIGURATOR_CONFIGURATOR_HPP_ */
