#include "tasks/pdu_heartbeat.hpp"
#include "rtos/task.hpp"
#include "vehicle/drivers.hpp"
#include "main.h"
#include <cstdio>


namespace rtos::tasks {

void PDUHeartbeatTask::loop() {
	vehicle::pduDriver.sendCommand(drivers::pdu::CommandMode::CurrentLimit);
	loopTimer.wait();
}

} // namespace rtos::tasks
