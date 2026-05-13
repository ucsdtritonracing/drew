#pragma once

#include "generics/snapshot.hpp"
#include "vehicle/types/pedals_types.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "vehicle/types/steering_types.hpp"
#include "vehicle/types/mode_types.hpp"
#include "vehicle/types/pdu_types.hpp"
#include "vehicle/types/inverter_types.hpp"
#include <stdint.h>
#include <cmath>
#include <atomic>


namespace vehicle {

class VehicleState {
public:
	VehicleState();

	// abstract state
	Mode getMode() const;
	void setReadyToDriveButtonPressed(bool status);
	void setShutdownCircuitClosed(bool status);
	void setMode(Mode newMode);

	// polled
	bool getReadyToDriveButtonPressed() const;
	bool getShutdownCircuitClosed() const;

	// pedals
	pedals::State getPedals() const;
	void setPedals(pedals::State state);

	// pdu
	pdu::State getPDUState() const;
	void setPDUState(pdu::State state);

	// wheel speed sensors
	wheels::State getWheelSpeeds() const;
	void setWheelSpeeds(wheels::State state);

	// steering
	steering::State getSteering() const;
	void setSteering(steering::State state);

	bool getConfigurationLocked();
	void setConfigurationLocked(bool locked);

	// inverter
	float getInverterTorqueCapability() const;
	void setInverterTemperature1(inverter::Temperature1& state);
	void setInverterTemperature2(inverter::Temperature2& state);
	void setInverterTemperature3(inverter::Temperature3& state);
	void setInverterMotorPosition(inverter::MotorPosition& state);
	void setInverterTorqueInformation(inverter::TorqueInformation& state);
	void setInverterTorqueCapability(inverter::TorqueCapability& state);
	void setInverterInternalStates(inverter::InternalStates& state);
	void setInverterFaultFlags(inverter::FaultFlags& state);


private:
	// High sample rate sensors
	Snapshot<pedals::State> pedals;
	Snapshot<wheels::State> wheelSpeeds;

	// CAN: SAS
	Snapshot<steering::State> steering;

	// CAN: PDU
	Snapshot<pdu::State> pdu;

	// CAN: Inverter
	Snapshot<inverter::Temperature1>		inverterTemperature1;
	Snapshot<inverter::Temperature2>		inverterTemperature2;
	Snapshot<inverter::Temperature3>		inverterTemperature3;
	Snapshot<inverter::MotorPosition>		inverterMotorPosition;
	Snapshot<inverter::TorqueInformation>	inverterTorqueInformation;
	Snapshot<inverter::TorqueCapability>	inverterTorqueCapability;
	Snapshot<inverter::InternalStates>		inverterInternalStates;
	Snapshot<inverter::FaultFlags>			inverterFaultFlags;


    // Polled signals
	std::atomic<bool> readyToDriveButtonPressed;
	std::atomic<bool> shutdownCircuitClosed;
	std::atomic<bool> configurationLocked;


	// Abstract State
    std::atomic<Mode> mode;

};

extern VehicleState vehicleState;

} // namespace vehicle
