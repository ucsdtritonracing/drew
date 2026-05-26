#include "tasks/control_loop.hpp"
#include "vehicle/drivers.hpp"
#include "vehicle/torque.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/types/pedals_types.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "vehicle/types/steering_types.hpp"
#include "vehicle/types/mode_types.hpp"
#include "vehicle/types/pdu_types.hpp"
#include "stm32g4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include <cmath>
#include <algorithm>


namespace rtos::tasks {

void ControlLoopTask::setup() {
	appsBrakePedalPlausibilityFaulted = false;
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_12V_MAIN_CHANNEL);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_12V_LEFT_CHANNEL);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_12V_RIGHT_CHANNEL);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_TSB_FANS_CHANNEL);
}

const vehicle::Mode ControlLoopTask::getNextMode(vehicle::Mode currentMode, TransitionInputs inputs) const {
	vehicle::Mode nextMode = currentMode;

	switch (currentMode) {
	case vehicle::Mode::CONFIGURATION:
		if (!inputs.configurationModeRequested) {
			nextMode = vehicle::Mode::IDLE;
		}
		break;
	case vehicle::Mode::IDLE:
		if (inputs.configurationModeRequested) {
			nextMode = vehicle::Mode::CONFIGURATION;
		} else if (inputs.brakePressed && inputs.readyToDriveButtonPressed && inputs.shutdownCircuitClosed) {
			nextMode = vehicle::Mode::READY_TO_DRIVE;
		}
		break;
	case vehicle::Mode::READY_TO_DRIVE:
		if (!inputs.shutdownCircuitClosed) {
			nextMode = vehicle::Mode::IDLE;
		}
		break;
	default:
		nextMode = vehicle::Mode::IDLE;
	}

	return nextMode;
}

void ControlLoopTask::onEnter(vehicle::Mode mode) {
	switch (mode) {
	case vehicle::Mode::IDLE:
		vehicle::pduDriver.disableChannel(vehicle::VehicleConfiguration::PDU_PUMPS_CHANNEL);
		vehicle::pduDriver.disableChannel(vehicle::VehicleConfiguration::PDU_RADIATOR_FANS_CHANNEL);
		break;
	case vehicle::Mode::READY_TO_DRIVE:
		app1Fault.reset();
		app2Fault.reset();
		bsefFault.reset();
		bserFault.reset();
		appsPlausibilityFault.reset();
		appsBrakePedalPlausibilityFaulted = false;

		vehicle::soundDriver.play();
		vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_PUMPS_CHANNEL);
		vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_RADIATOR_FANS_CHANNEL);
		break;
	case vehicle::Mode::CONFIGURATION:
		break;
	}
}

void ControlLoopTask::loop() {
	/*		INPUTS		*/
	const uint32_t currentTick = HAL_GetTick();
	// state
	const vehicle::Mode mode = vehicle::vehicleState.getMode();
	const vehicle::pedals::State pedals = vehicle::vehicleState.getPedals();
	const float torqueCapability = vehicle::vehicleState.getInverterTorqueCapability();
	const bool readyToDriveButtonPressed = vehicle::vehicleState.getReadyToDriveButtonPressed();
	const bool shutdownCircuitClosed = vehicle::vehicleState.getShutdownCircuitClosed();
	const bool configurationModeRequested = vehicle::configuratorDriver.requestingConfigurationMode();
	// computed state
	const float appCommand = pedals.app1;	// value being used for calculations
	const bool brakePressed = (pedals.bsef > vehicle::vehicleConfiguration.bsefBrakeEngagedThreshold && pedals.bsefValid) ||
							  (pedals.bser > vehicle::vehicleConfiguration.bserBrakeEngagedThreshold && pedals.bserValid);
	const bool appsPlausible = torque::isAPPSPlausible(pedals.app1, pedals.app2);
	const bool appsBrakePedalPlausible = torque::isAPPSBrakePedalPlausible(appsBrakePedalPlausibilityFaulted, appCommand, pedals.bsef, pedals.bser);


	/*		TRANSITION		*/
	const vehicle::Mode nextMode = getNextMode(
		mode,
		TransitionInputs{brakePressed, readyToDriveButtonPressed, shutdownCircuitClosed, configurationModeRequested}
	);
	if (nextMode != mode) {
		onEnter(nextMode);
	}
	vehicle::vehicleState.setMode(nextMode);


	/*		UPDATES		*/
	app1Fault.update(!pedals.app1Valid, currentTick);
	app2Fault.update(!pedals.app2Valid, currentTick);
	bsefFault.update(!pedals.bsefValid, currentTick);
	bserFault.update(!pedals.bserValid, currentTick);
	appsPlausibilityFault.update(!appsPlausible, currentTick);
	appsBrakePedalPlausibilityFaulted = !appsBrakePedalPlausible;


	/*		OUTPUTS		*/
	// brake light
	if (brakePressed) {
		vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_BRAKE_LIGHT_CHANNEL);
	} else {
		vehicle::pduDriver.disableChannel(vehicle::VehicleConfiguration::PDU_BRAKE_LIGHT_CHANNEL);
	}

	// torque
	switch (nextMode) {
	case vehicle::Mode::IDLE:
		vehicle::inverterDriver.sendCommandMessage(0, false);
		break;
	case vehicle::Mode::CONFIGURATION:
		vehicle::inverterDriver.sendCommandMessage(0, false);
		break;
	case vehicle::Mode::READY_TO_DRIVE:
		vehicle::vehicleState.setAPPFault(appsPlausibilityFault.torqueInhibited(currentTick));
		vehicle::vehicleState.setABPPCFault(appsBrakePedalPlausibilityFaulted);

		const bool torqueInhibited = app1Fault.torqueInhibited(currentTick) ||
									 app2Fault.torqueInhibited(currentTick) ||
									 bsefFault.torqueInhibited(currentTick) ||
									 bserFault.torqueInhibited(currentTick) ||
									 appsPlausibilityFault.torqueInhibited(currentTick) ||
									 appsBrakePedalPlausibilityFaulted;

		float torqueScalar = 0.0f;

		if (!torqueInhibited) {
			torqueScalar = torque::computeDriverTorqueRequest(appCommand);
		}

		if (!std::isfinite(torqueScalar)) {
			torqueScalar = 0.0f;
		}

		torqueScalar = std::clamp(torqueScalar, 0.0f, 1.0f);
		const float driverTorqueRequestNm = torqueScalar * std::min(torque::MAX_TORQUE_LIMIT_NM, vehicle::vehicleConfiguration.maxTorqueNm);
		const float torqueRequest = std::min(driverTorqueRequestNm, torqueCapability);

		vehicle::inverterDriver.sendCommandMessage(torqueRequest, true);
		break;
	}

	loopTimer.wait();
}

} // namespace rtos::tasks
