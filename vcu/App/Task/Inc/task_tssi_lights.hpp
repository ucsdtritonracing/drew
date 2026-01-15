#pragma once
#include "task.hpp"
#include "drivers.hpp"

namespace tasks {

class TSSILightsTask: public Task<TSSILightsTask, osPriorityHigh, 512> {
public:
	TSSILightsTask();
	void loop();

private:
	bool faultStateBMS;
	bool faultStateIMD;
};
} // namespace tasks
