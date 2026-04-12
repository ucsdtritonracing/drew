#include "tasks/pdu.hpp"
#include "generics/task.hpp"
#include "vehicle/drivers.hpp"
#include "main.h"
#include <cstdio>


namespace tasks {

void PDUTask::loop() {
	vehicle::pdu.sendCommand(drivers::pdu::CommandMode::CurrentLimit);
	osDelay(PDU_COMMAND_INTERVAL);
}

} // namespace tasks
