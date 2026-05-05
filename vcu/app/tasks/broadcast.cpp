#include "tasks/broadcast.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/drivers.hpp"
#include "cmsis_os.h"


namespace rtos::tasks {

void BroadcastTask::loop() {
	auto pedals = vehicle::vehicleState.getPedals();

	vehicle::broadcasterDriver.broadcastBrakesMessage(pedals);
	vehicle::broadcasterDriver.broadcastAPPMessage(pedals);
	osDelay(DELAY / 2); // need to space out messages to not overflow buffer

	auto wheels = vehicle::vehicleState.getWheelSpeeds();
	bool r2dbPressed = vehicle::vehicleState.getReadyToDriveButtonPressed();
	bool sdcClosed = vehicle::vehicleState.getShutdownCircuitClosed();
	bool r2dbEnabled = vehicle::vehicleState.getMode() == vehicle::Mode::READY_TO_DRIVE;

	vehicle::broadcasterDriver.broadcastWheelsMessage(wheels);
	vehicle::broadcasterDriver.broadcastFlagsMessage(r2dbPressed, sdcClosed, r2dbEnabled);
	osDelay(DELAY / 2);
}

} // namespace rtos::tasks
