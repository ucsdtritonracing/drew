#include "tasks/startup.hpp"
#include "vehicle/drivers.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "cmsis_os.h"


namespace rtos::tasks {

void StartupTask::setup() {
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_12V_MAIN_CHANNEL);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_12V_RIGHT_CHANNEL);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_TSB_FANS_CHANNEL);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_PUMPS_CHANNEL);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_RADIATOR_FANS_CHANNEL);
	osDelay(STARTUP_DELAY);
	vehicle::pduDriver.enableChannel(vehicle::VehicleConfiguration::PDU_12V_LEFT_CHANNEL);
}

void StartupTask::loop() {
	osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
}

} // namespace rtos::tasks
