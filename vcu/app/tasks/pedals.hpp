#pragma once

#include "generics/task.hpp"


namespace tasks {

class PedalsTask : public Task<PedalsTask, osPriorityNormal, 256> {
public:
	void loop();

	static constexpr uint32_t PEDAL_BUFFER_HALF_COMPLETE_FLAG = 1u << 0;
	static constexpr uint32_t PEDAL_BUFFER_FULL_COMPLETE_FLAG = 1u << 1;
};

} // namespace tasks
