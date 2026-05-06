#pragma once

#include "generics/task.hpp"
#include "drivers/can/can_bus.hpp"


namespace tasks {

class CANRecoveryTask: public Task<CANRecoveryTask, osPriorityLow, 128> {
public:
	void init(FDCAN_HandleTypeDef &hfdcan1, FDCAN_HandleTypeDef &hfdcan2);
	void loop();

	static constexpr uint32_t RETRY_DELAY_MS = 100;

	static constexpr uint32_t CANBUS1_BUS_OFF_FLAG = 1u << 0;
	static constexpr uint32_t CANBUS2_BUS_OFF_FLAG = 1u << 1;
	static constexpr uint32_t ANY_CANBUS_BUS_OFF_FLAG = CANBUS1_BUS_OFF_FLAG | CANBUS2_BUS_OFF_FLAG;

private:
	FDCAN_HandleTypeDef *hfdcan1;
	FDCAN_HandleTypeDef *hfdcan2;
};

} // namespace tasks
