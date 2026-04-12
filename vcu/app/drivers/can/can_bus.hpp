#pragma once

#include "drivers/can/can_utils.hpp"
#include "stm32g4xx_hal.h"


namespace drivers::can {

class CANBus {
using CANHandler = void (*)(void*, const Message&);

struct HandlerEntry {
	void* instance;
	uint32_t id;
	CANHandler callback;
};
public:
	CANBus();

	/*
	 * @brief Initialize the CAN Bus.
	 *
	 * @param fdcan FDCAN peripheral to attach the CAN Bus to.
	 */
    void init(FDCAN_HandleTypeDef& fdcan);

    /*
     * @brief Add a CAN message handler.
     *
     * @param instance Instance providing the callback
     * @param id CAN ID to trigger callback when received
     * @param callback
     */
    void addMessageHandler(void *instance, uint32_t id, CANHandler callback);

    /*
     * @brief Process an incoming CAN Message.
     *
     * @param message
     */
    void processMessage(Message *message) const;

    /*
     * @brief Transmit a CAN frame on the bus.
     *
     * @param id CAN ID of the CAN frame
     * @param data Payload of the CAN frame
     * @param dlc Data Length Code of the CAN frame
     */
    void transmit(uint32_t id, const uint8_t *data, uint32_t dlc) const;

private:
	static constexpr size_t MAX_HANDLERS = 32;
    HandlerEntry handlers[MAX_HANDLERS];
    size_t numHandlers = 0;

    FDCAN_HandleTypeDef* fdcan;
};

} // namespace drivers::can
