#include "task_pdu.hpp"
#include "task.hpp"
#include "main.h"
#include <cstdio>

namespace tasks {

PDUTask::PDUTask() {}

void PDUTask::loop() {
	vehicle::pdu.sendCommand();
	osDelay(PDU_REFRESH_RATE);
}

} // namespace tasks
