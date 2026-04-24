#include "tasks/control_loop.hpp"
#include "generics/task.hpp"
#include "vehicle/drivers.hpp"
#include "vehicle/torque.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/types/pedals_types.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "vehicle/types/steering_types.hpp"
#include "vehicle/types/mode_types.hpp"
#include "vehicle/types/pdu_types.hpp"
#include "cmsis_os.h"
#include <cmath>
#include <algorithm>


namespace tasks {

void ControlLoopTask::setup() {
	appsBrakePedalPlausibilityFaulted = false;
}

const vehicle::Mode ControlLoopTask::getNextMode(vehicle::Mode currentMode, TransitionInputs inputs) const {
	vehicle::Mode nextMode = currentMode;

	switch (currentMode) {
	case vehicle::Mode::IDLE:
		if (inputs.brakePressed && inputs.readyToDriveButtonPressed && inputs.shutdownCircuitClosed) {
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
		break;
	case vehicle::Mode::READY_TO_DRIVE:
		app1Fault.reset();
		app2Fault.reset();
		bsefFault.reset();
		bserFault.reset();
		appsPlausibilityFault.reset();
		appsBrakePedalPlausibilityFaulted = false;
		break;
	}
}

void ControlLoopTask::loop() {
	/*		INPUTS		*/
	// state
	const vehicle::Mode mode = vehicle::vehicleState.getMode();
	const vehicle::pedals::State pedals = vehicle::vehicleState.getPedals();
	const float torqueCapability = vehicle::vehicleState.getInverterTorqueCapability();
	const bool readyToDriveButtonPressed = vehicle::vehicleState.getReadyToDriveButtonPressed();
	const bool shutdownCircuitClosed = vehicle::vehicleState.getShutdownCircuitClosed();
	// computed state
	const bool brakePressed = (pedals.bsef > vehicle::vehicleConfiguration.bsefBrakeEngagedThreshold && pedals.bsefValid) ||
							  (pedals.bser > vehicle::vehicleConfiguration.bserBrakeEngagedThreshold && pedals.bserValid);


	/*		TRANSITION		*/
	const vehicle::Mode nextMode = getNextMode(
		mode,
		TransitionInputs{
			brakePressed,
			readyToDriveButtonPressed,
			shutdownCircuitClosed
		}
	);
	if (nextMode != mode) {
		onEnter(nextMode);
	}
	vehicle::vehicleState.setMode(nextMode);


	/*		OUTPUTS		*/
	switch (nextMode) {
	case vehicle::Mode::IDLE:
		vehicle::inverterDriver.sendCommandMessage(0, false);
		break;
	case vehicle::Mode::READY_TO_DRIVE:
		const float appCommand = pedals.app1;	// value being used for calculations
		const bool appsPlausible = torque::isAPPSPlausible(pedals.app1, pedals.app2);
		const bool appsBrakePedalPlausible = torque::isAPPSBrakePedalPlausible(appsBrakePedalPlausibilityFaulted, appCommand, pedals.bsef, pedals.bser);
		const uint32_t currentTick = HAL_GetTick();

		app1Fault.update(!pedals.app1Valid, currentTick);
		app2Fault.update(!pedals.app2Valid, currentTick);
		bsefFault.update(!pedals.bsefValid, currentTick);
		bserFault.update(!pedals.bserValid, currentTick);
		appsPlausibilityFault.update(!appsPlausible, currentTick);
		appsBrakePedalPlausibilityFaulted = !appsBrakePedalPlausible;

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
		const float driverTorqueRequestNm = torqueScalar * torque::MAX_TORQUE_LIMIT_NM;
		const uint16_t torqueRequest = static_cast<uint16_t>(std::lroundf(std::min(driverTorqueRequestNm, torqueCapability)));

		vehicle::inverterDriver.sendCommandMessage(torqueRequest, true);
		break;
	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
