#include <vehicle_state.hpp>


namespace vehicle {

VehicleState::VehicleState() {
	wheelSpeeds 				= {0, 0, 0, 0};
	acceleratorPedalPositions 	= {0, 0};
	brakePressures 				= {0, 0};

	steeringAngleDegrees 		= 0;

	bmsFault 					= false;
	imdFault 					= false;
	readyToDriveButtonPressed 	= false;
	shutdownCircuitClosed 		= false;

	readyToDrive 				= false;
}

VehicleState vehicleState{};

} // namespace vehicle
