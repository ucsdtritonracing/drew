#pragma once

#include "generics/task.hpp"
#include "drivers/pdu/pdu.hpp"

namespace tasks {

class PDUTask: public Task<PDUTask, osPriorityHigh, 256> {
public:
	void loop();

private:
	const uint8_t PDU_COMMAND_INTERVAL = 100;
};

} // namespace tasks
