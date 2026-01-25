#pragma once
#include <task.hpp>

namespace tasks {

class APPS1Task : public Task<APPS1Task, osPriorityNormal, 256> {
public:
	void loop();
};

} // namespace tasks
