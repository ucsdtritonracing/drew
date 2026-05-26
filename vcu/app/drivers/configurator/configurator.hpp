#pragma once

#include "drivers/configurator/configurator_types.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "drivers/can/tx_slot.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include <optional>


namespace drivers::configurator {

class Configurator: public drivers::can::CANPeripheral<Configurator> {
public:
	Configurator(drivers::can::CANBus& canBus);

	void init();

	void processCommandSetParameter(const drivers::can::Message& message);
	void processCommandWriteConfiguration(const drivers::can::Message& message);
	void processCommandConfigurationMode(const drivers::can::Message& message);

	bool requestingConfigurationMode() const;
	void broadcastNextConfigurationParameter();

private:
    vehicle::VehicleConfiguration stagedConfiguration;
	bool configurationModeRequested = false;
	int broadcastParameterIndex = 0;
	static constexpr int NUM_BROADCAST_PARAMETER_VALUES = 34;

	drivers::can::TxSlotHandle parameterResponseSlotHandle;
	uint8_t txData[can::MAX_CLASSICAL_CAN_DATA_LENGTH];

    static constexpr uint32_t CAN_ID_SET_PARAMETER					= 0x190;
    static constexpr uint32_t CAN_ID_PARAMETER_RESPONSE				= 0x192;
    static constexpr uint32_t CAN_ID_CONFIGURATION_MODE				= 0x193;
    static constexpr uint32_t CAN_ID_WRITE_CONFIGURATION			= 0x194;

    static constexpr size_t PARAMETER_MESSAGE_DATA_START			= 4;
    static constexpr size_t PARAMETER_MESSAGE_INFO_START			= 2;
    static constexpr size_t PARAMETER_MESSAGE_NUM_BYTES				= 8;
    static constexpr float PARAMETER_MESSAGE_SCALE					= 1000000;

    static constexpr size_t CONFIGURATION_MODE_MESSAGE_NUM_BYTES	= 1;

    static constexpr size_t WRITE_CONFIGURATION_MESSAGE_NUM_BYTES	= 0;


	std::optional<Parameter> parseParameterId(const drivers::can::Message& message);
	std::optional<ThresholdType> parseThresholdType(uint16_t info);

	uint32_t scale(float value);
	float unscale(uint32_t data);

	void setThreshold(Parameter parameter, ThresholdType type, float value);
	void setMaxTorque(float value);

	void sendParameterValue(Parameter parameter, uint16_t info, float data);
	void sendThresholdRanges(Parameter parameter);

};

} // namespace drivers::configurator
