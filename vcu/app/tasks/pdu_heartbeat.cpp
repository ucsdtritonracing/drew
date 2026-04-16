#include "tasks/pdu_heartbeat.hpp"
#include "generics/task.hpp"
#include "vehicle/drivers.hpp"
#include "main.h"
#include <cstdio>


namespace tasks {

void PDUHeartbeatTask::loop() {
	vehicle::pduDriver.sendCommand(drivers::pdu::CommandMode::CurrentLimit);
	osDelay(PDU_COMMAND_INTERVAL);
}

} // namespace tasks
