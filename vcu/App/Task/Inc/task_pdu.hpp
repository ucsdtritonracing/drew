#pragma once
#include "task.hpp"
#include "drivers.hpp"
#include "pdu.hpp"

namespace tasks {

class PDUTask: public Task<PDUTask, osPriorityHigh, 512> {
public:
	PDUTask();
	void loop();
private:
	const uint8_t PDU_COMMAND_DELAY = 250;
};

} // namespace tasks
