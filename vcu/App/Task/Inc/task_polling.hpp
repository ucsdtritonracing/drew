#pragma once
#include <task.hpp>

namespace tasks {

class PollingTask : public Task<PollingTask, osPriorityNormal, 256> {
public:
	void setup();
	void loop();

private:
	static const uint32_t DELAY = 100;
};

} // namespace tasks
