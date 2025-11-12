#pragma once
#include "cmsis_os.h"
#include "stm32g4xx_hal.h"

namespace drivers::CAN {

constexpr size_t MAX_CLASSICAL_CAN_DATA_LENGTH = 8;
constexpr uint32_t MAX_CAN_STD_ID = 0x7FF;
constexpr uint32_t MAX_CAN_EXTD_ID = 0x1FFFFFFF;

struct Message {
	FDCAN_RxHeaderTypeDef *rxHeader;
	uint8_t numBytes;
	uint8_t data[MAX_CLASSICAL_CAN_DATA_LENGTH];
};

using CANHandler = void (*)(void*, const Message&);

struct HandlerEntry {
	uint32_t id;
	void* instance;
	CANHandler callback;
};

class CANBus {
public:
    CANBus(FDCAN_HandleTypeDef* fdcan, osMessageQueueId_t rxQueue);

    void transmit(uint32_t id, const uint8_t *data, uint32_t dlc) const;
    void addMessageHandler(uint32_t id, void *peripheral, CANHandler callback);
    void processMessages();

private:
	static constexpr size_t MAX_HANDLERS = 32;
    HandlerEntry handlers[MAX_HANDLERS];
    size_t numHandlers = 0;

    FDCAN_HandleTypeDef* fdcan;
    osMessageQueueId_t rxQueue;
};

} // namespace drivers::CAN
