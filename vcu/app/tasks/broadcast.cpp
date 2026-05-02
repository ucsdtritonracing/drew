#include "tasks/broadcast.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/drivers.hpp"
#include "cmsis_os.h"


namespace rtos::tasks {

void BroadcastTask::loop() {
	auto wheels = vehicle::vehicleState.getWheelSpeeds();
	auto pedals = vehicle::vehicleState.getPedals();

	bool r2dbPressed = vehicle::vehicleState.getReadyToDriveButtonPressed();
	bool sdcClosed = vehicle::vehicleState.getShutdownCircuitClosed();
	bool r2dbEnabled = vehicle::vehicleState.getMode() == vehicle::Mode::READY_TO_DRIVE;

	vehicle::broadcasterDriver.broadcastWheelsMessage(wheels);
	vehicle::broadcasterDriver.broadcastBrakesMessage(pedals);
	vehicle::broadcasterDriver.broadcastAPPMessage(pedals);
	vehicle::broadcasterDriver.broadcastFlagsMessage(r2dbPressed, sdcClosed, r2dbEnabled);

	osDelay(DELAY);
}

} // namespace rtos::tasks
