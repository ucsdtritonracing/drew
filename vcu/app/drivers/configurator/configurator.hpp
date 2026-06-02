#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "drivers/can/tx_slot.hpp"
#include "vehicle/pedal_map.hpp"
#include "vehicle/vehicle_configuration.hpp"


namespace drivers::configurator {

class Configurator: public drivers::can::CANPeripheral<Configurator> {
public:
	Configurator(drivers::can::CANBus& canBus);

	void init();
	bool requestingConfigurationMode() const;

	void processCommandConfigurationMode(const drivers::can::Message& message);
	void processCommandWriteConfiguration(const drivers::can::Message& message);
	void processCommandFlashConfiguration(const drivers::can::Message& message);
	void processCommandThreshold(const drivers::can::Message& message);
	void processCommandBSEEngage(const drivers::can::Message& message);
	void processCommandPedalMap(const drivers::can::Message& message);
	void processCommandTorque(const drivers::can::Message& message);

	void broadcastParameters();

private:
	bool configurationModeRequested = false;
    vehicle::VehicleConfiguration stagedConfiguration;

	drivers::can::TxSlotHandle app1SlotHandle;
	drivers::can::TxSlotHandle app2SlotHandle;
	drivers::can::TxSlotHandle bsefSlotHandle;
	drivers::can::TxSlotHandle bserSlotHandle;
	drivers::can::TxSlotHandle bseEngageSlotHandle;

	drivers::can::TxSlotHandle torqueSlotHandle;

	drivers::can::TxSlotHandle pedalMapPointSlotHandles[vehicle::PedalMap::NUM_EDITABLE_POINTS];

	uint8_t txData[can::MAX_CLASSICAL_CAN_DATA_LENGTH];

	void broadcastAPP1Parameter(vehicle::VehicleConfiguration &config);
	void broadcastAPP2Parameter(vehicle::VehicleConfiguration &config);
	void broadcastBSEFParameter(vehicle::VehicleConfiguration &config);
	void broadcastBSERParameter(vehicle::VehicleConfiguration &config);
	void broadcastBSEEngageParameter(vehicle::VehicleConfiguration &config);
	void broadcastTorqueParameter(vehicle::VehicleConfiguration &config);
	void broadcastPedalMapParameters(vehicle::VehicleConfiguration &config);

	void packThreshold(vehicle::AnalogCalibration calibration, uint8_t *data);
};

} // namespace drivers::configurator
