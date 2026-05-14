#pragma once

#include "drivers/can/can_utils.hpp"
#include "drivers/can/tx_manager.hpp"
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
     * @brief Add a CAN message Tx slot.
     *
     * @param id CAN ID of the message
     * @param priority 0 is the highest priority possible
     * @return TxSlotHandle of the slot created
     */
    TxSlotHandle addTxSlot(uint32_t id, TxPriority priority);

    /*
     * @brief Process an incoming CAN Message.
     *
     * @param message
     */
    void processMessage(Message *message) const;

    /*
     * @brief Publish a new message for a TxSlot on the bus.
     *
     * @param handle Handle of the TxSlot
     * @param data Payload of the CAN frame
     * @param numBytes Data length of the CAN frame
     */
    void publishTxSlot(TxSlotHandle handle, const uint8_t data[], uint32_t numBytes);

    /*
     * @brief Try to transmit the next messages on the bus.
     */
    void flushTx();

private:
	static constexpr size_t MAX_HANDLERS = 64;
    HandlerEntry handlers[MAX_HANDLERS];
    size_t numHandlers = 0;

    TxManager txManager;

    FDCAN_HandleTypeDef* fdcan;
};

} // namespace drivers::can
