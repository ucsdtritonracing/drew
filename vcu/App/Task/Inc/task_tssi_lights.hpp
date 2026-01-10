#pragma once
#include "task.hpp"
#include "drivers.hpp"

namespace tasks {

class TSSILightsTask: public Task<TSSILightsTask, osPriorityHigh, 512> {
public:
	TSSILightsTask(const drivers::can::CANBus& CANBus);
	void loop();

private:
	const drivers::can::CANBus& CANBus;
	bool faultStateBMS;
	bool faultStateIMD;
	bool flash = false;
};
} // namespace tasks
