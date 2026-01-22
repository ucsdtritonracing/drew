#include "task_pdu.hpp"
#include "task.hpp"
#include "main.h"
#include <cstdio>

namespace tasks {

PDUTask::PDUTask() {}

void PDUTask::loop() {
	vehicle::pdu.sendCommand(drivers::pdu::CommandMode::CurrentLimit);
	osDelay(PDU_COMMAND_INTERVAL);
}

} // namespace tasks
