#pragma once

#include "drivers/configurator/parameter.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "drivers/can/tx_slot.hpp"
#include "vehicle/vehicle_configuration.hpp"


namespace drivers::configurator {

class Configurator: public drivers::can::CANPeripheral<Configurator> {
public:
	Configurator(drivers::can::CANBus& canBus);

	void init();

	void processCommandSetParameter(const drivers::can::Message& message);
	void processCommandGetParameter(const drivers::can::Message& message);
	void processCommandPushConfiguration(const drivers::can::Message& message);

private:
	Parameter parseParameterId(const drivers::can::Message& message);
	void sendParameterValue(Parameter parameter, uint32_t data, uint16_t info);

	drivers::can::TxSlotHandle parameterResponseSlotHandle;

    static constexpr uint32_t CAN_ID_SET_PARAMETER		= 0x190;
    static constexpr uint32_t CAN_ID_GET_PARAMETER		= 0x191;
    static constexpr uint32_t CAN_ID_PARAMETER_RESPONSE	= 0xCCC;

    vehicle::VehicleConfiguration stagedConfiguration;
};

} // namespace drivers::configurator
