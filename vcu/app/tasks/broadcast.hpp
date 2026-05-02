#pragma once

#include "generics/task.hpp"


namespace tasks {

class BroadcastTask : public Task<BroadcastTask> {
public:
	void loop();

private:
	static const uint32_t DELAY = 100;
};

} // namespace tasks
