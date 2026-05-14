#include "drivers/broadcaster/broadcaster.hpp"
#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "vehicle/types/pedals_types.hpp"


namespace drivers::broadcaster {

Broadcaster::Broadcaster(drivers::can::CANBus &canBus) :
		drivers::can::CANPeripheral<Broadcaster>(canBus) {}


void Broadcaster::init() {
	appMessageSlotHandle = bindTxSlot(CAN_ID_APP_MESSAGE, drivers::can::TxPriority::TELEMETRY);
	brakesMessageSlotHandle = bindTxSlot(CAN_ID_BRAKES_MESSAGE, drivers::can::TxPriority::TELEMETRY);
	wheelsMessageSlotHandle = bindTxSlot(CAN_ID_WHEELS_MESSAGE, drivers::can::TxPriority::TELEMETRY);
	flagsMessageSlotHandle = bindTxSlot(CAN_ID_FLAGS_MESSAGE, drivers::can::TxPriority::TELEMETRY);
	imuMessageSlotHandle = bindTxSlot(CAN_ID_IMU_MESSAGE, drivers::can::TxPriority::TELEMETRY);
}


void Broadcaster::broadcastWheelsMessage(const vehicle::wheels::State& wheels) {
	int16_t frontRight	= static_cast<int16_t>(wheels.frontRight * WHEELS_SCALE);
	int16_t frontLeft	= static_cast<int16_t>(wheels.frontLeft * WHEELS_SCALE);
	int16_t rearRight	= static_cast<int16_t>(wheels.rearRight * WHEELS_SCALE);
	int16_t rearLeft	= static_cast<int16_t>(wheels.rearLeft * WHEELS_SCALE);

	uint16_t rawFR = static_cast<uint16_t>(frontRight);
	uint16_t rawFL = static_cast<uint16_t>(frontLeft);
	uint16_t rawRR = static_cast<uint16_t>(rearRight);
	uint16_t rawRL = static_cast<uint16_t>(rearLeft);

	txData[WHEELS_FR_START]		= static_cast<uint8_t>(rawFR & BYTE_MASK);
	txData[WHEELS_FR_START+1]	= static_cast<uint8_t>((rawFR >> 8) & BYTE_MASK);
	txData[WHEELS_FL_START]		= static_cast<uint8_t>(rawFL & BYTE_MASK);
	txData[WHEELS_FL_START+1]	= static_cast<uint8_t>((rawFL >> 8) & BYTE_MASK);
	txData[WHEELS_RL_START]		= static_cast<uint8_t>(rawRR & BYTE_MASK);
	txData[WHEELS_RL_START+1]	= static_cast<uint8_t>((rawRR >> 8) & BYTE_MASK);
	txData[WHEELS_RR_START]		= static_cast<uint8_t>(rawRL & BYTE_MASK);
	txData[WHEELS_RR_START+1]	= static_cast<uint8_t>((rawRL >> 8) & BYTE_MASK);

	canBus.publishTxSlot(wheelsMessageSlotHandle, txData, WHEELS_MESSAGE_NUM_BYTES);
}


void Broadcaster::broadcastBrakesMessage(const vehicle::pedals::State& pedals) {
	uint16_t bsef	= static_cast<uint16_t>(pedals.bsef * PEDALS_SCALE);
	uint16_t bser	= static_cast<uint16_t>(pedals.bser * PEDALS_SCALE);

	txData[BRAKES_BSEF_START]	= static_cast<uint8_t>(bsef & BYTE_MASK);
	txData[BRAKES_BSEF_START+1]	= static_cast<uint8_t>((bsef >> 8) & BYTE_MASK);
	txData[BRAKES_BSER_START]	= static_cast<uint8_t>(bser & BYTE_MASK);
	txData[BRAKES_BSER_START+1]	= static_cast<uint8_t>((bser >> 8) & BYTE_MASK);
	txData[BRAKES_FLAGS_START]	= static_cast<uint8_t>(
			((pedals.bsefValid & 1u) << BRAKES_BSEF_VALID_BIT) |
			((pedals.bserValid & 1u) << BRAKES_BSER_VALID_BIT)
	);
	canBus.publishTxSlot(brakesMessageSlotHandle, txData, BRAKES_MESSAGE_NUM_BYTES);
}


void Broadcaster::broadcastAPPMessage(const vehicle::pedals::State& pedals) {
	uint16_t app1	= static_cast<uint16_t>(pedals.app1 * PEDALS_SCALE);
	uint16_t app2	= static_cast<uint16_t>(pedals.app2 * PEDALS_SCALE);

	txData[APP_APP1_START]	= static_cast<uint8_t>(app1 & BYTE_MASK);
	txData[APP_APP1_START+1]	= static_cast<uint8_t>((app1 >> 8) & BYTE_MASK);
	txData[APP_APP2_START]	= static_cast<uint8_t>(app2 & BYTE_MASK);
	txData[APP_APP2_START+1]	= static_cast<uint8_t>((app2 >> 8) & BYTE_MASK);
	txData[APP_FLAGS_START]	= static_cast<uint8_t>(
			((pedals.app1Valid & 1u) << APP_APP1_VALID_BIT) |
			((pedals.app2Valid & 1u) << APP_APP2_VALID_BIT)
	);
	canBus.publishTxSlot(appMessageSlotHandle, txData, APP_MESSAGE_NUM_BYTES);
}


void Broadcaster::broadcastFlagsMessage(Flags flags) {
	txData[0] = static_cast<uint8_t>(
			((flags.r2dbPressed & 1u) << FLAGS_R2DB_PRESSED_BIT) |
			((flags.sdcClosed & 1u) << FLAGS_SDC_CLOSED_BIT) |
			((flags.r2dEnabled & 1u) << FLAGS_R2D_ENABLED_BIT) |
			((flags.appFault & 1u) << FLAGS_APP_FAULT_BIT) |
			((flags.abppcFault & 1u) << FLAGS_ABPPC_FAULT_BIT)
	);
	canBus.publishTxSlot(flagsMessageSlotHandle, txData, FLAGS_MESSAGE_NUM_BYTES);
}

} // namespace drivers::broadcaster
