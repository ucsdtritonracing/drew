#pragma once

#include "generics/task.hpp"


namespace tasks {

class WheelsTask : public Task<WheelsTask, osPriorityNormal, 128> {
public:
	void loop();

	static constexpr uint32_t INTERVAL_MS = 50;
};

} // namespace tasks
