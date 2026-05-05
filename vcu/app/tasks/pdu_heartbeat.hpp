#pragma once

#include "generics/task.hpp"
#include "drivers/pdu/pdu.hpp"
#include "generics/timers.hpp"

namespace tasks {

class PDUHeartbeatTask: public Task<PDUHeartbeatTask, osPriorityLow, 128> {
public:
	void loop();

private:
	const uint8_t PDU_COMMAND_INTERVAL = 100;
	timers::PeriodicTimer loopTimer{PDU_COMMAND_INTERVAL};
};

} // namespace tasks
