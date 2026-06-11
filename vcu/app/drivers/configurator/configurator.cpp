#include "drivers/configurator/configurator.hpp"
#include "drivers/configurator/configurator_can.hpp"
#include "drivers/storage/storage.hpp"
#include "vehicle/pedal_map.hpp"
#include "vehicle/torque.hpp"
#include "vehicle/types/threshold_types.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/types/configuration_types.hpp"
#include <string.h>


namespace drivers::configurator {

Configurator::Configurator(drivers::can::CANBus& canBus)
    : CANPeripheral(canBus) {}


void Configurator::init() {
	bindHandler<&Configurator::processCommandConfigurationMode>(CAN_ID_CONFIGURATION_MODE);
	bindHandler<&Configurator::processCommandConfigurationMode>(CAN_ID_CONFIGURATION_MODE);
	bindHandler<&Configurator::processCommandWriteConfiguration>(CAN_ID_WRITE_CONFIGURATION);
	bindHandler<&Configurator::processCommandFlashConfiguration>(CAN_ID_FLASH_CONFIGURATION);
	bindHandler<&Configurator::processCommandThreshold>(CAN_ID_CMD_APP1_THRESHOLD);
	bindHandler<&Configurator::processCommandThreshold>(CAN_ID_CMD_APP2_THRESHOLD);
	bindHandler<&Configurator::processCommandThreshold>(CAN_ID_CMD_BSEF_THRESHOLD);
	bindHandler<&Configurator::processCommandThreshold>(CAN_ID_CMD_BSER_THRESHOLD);
	bindHandler<&Configurator::processCommandBSEEngage>(CAN_ID_CMD_BSE_ENGAGE);
	bindHandler<&Configurator::processCommandTorque>(CAN_ID_CMD_MAX_TORQUE_REQUEST);
	for (size_t i = 0; i < vehicle::PedalMap::NUM_EDITABLE_POINTS; i++) {
		bindHandler<&Configurator::processCommandPedalMap>(CAN_ID_CMD_PEDAL_MAP_BASE + i);
	}

	app1SlotHandle = bindTxSlot(CAN_ID_CFG_APP1_THRESHOLD, drivers::can::TELEMETRY);
	app2SlotHandle = bindTxSlot(CAN_ID_CFG_APP2_THRESHOLD, drivers::can::TELEMETRY);
	bsefSlotHandle = bindTxSlot(CAN_ID_CFG_BSEF_THRESHOLD, drivers::can::TELEMETRY);
	bserSlotHandle = bindTxSlot(CAN_ID_CFG_BSER_THRESHOLD, drivers::can::TELEMETRY);
	bseEngageSlotHandle = bindTxSlot(CAN_ID_CFG_BSE_ENGAGE, drivers::can::TELEMETRY);
	torqueSlotHandle = bindTxSlot(CAN_ID_CFG_MAX_TORQUE_REQUEST, drivers::can::TELEMETRY);
	for (size_t i = 0; i < vehicle::PedalMap::NUM_EDITABLE_POINTS; i++) {
		pedalMapPointSlotHandles[i] = bindTxSlot(CAN_ID_CFG_PEDAL_MAP_BASE + i, drivers::can::TELEMETRY);
	}

    if (drivers::storage::ConfigStorage::load(stagedConfiguration)) {
    	vehicle::vehicleConfiguration = stagedConfiguration;
    }
}


bool Configurator::requestingConfigurationMode() const {
	return configurationModeRequested;
}


void Configurator::processCommandWriteConfiguration(const drivers::can::Message& message) {
	if (message.numBytes != WRITE_CONFIG_MESSAGE_LENGTH) {
	if (message.numBytes != WRITE_CONFIG_MESSAGE_LENGTH) {
		return;
	}
	if (stagedConfiguration.valid() && vehicle::vehicleState.getMode() == vehicle::Mode::CONFIGURATION) {
		vehicle::vehicleConfiguration = stagedConfiguration;
	}
}
void Configurator::processCommandFlashConfiguration(const drivers::can::Message& message) {
	if (message.numBytes != FLASH_CONFIG_MESSAGE_LENGTH) {
		return;
	}
	if (stagedConfiguration.valid() && vehicle::vehicleState.getMode() == vehicle::Mode::CONFIGURATION) {
		if (drivers::storage::ConfigStorage::save(stagedConfiguration)) {
			vehicle::vehicleConfiguration = stagedConfiguration;
			configurationModeRequested = false;
		}
	}
}
void Configurator::processCommandConfigurationMode(const drivers::can::Message& message) {
	if (message.numBytes != CONFIG_MODE_MESSAGE_LENGTH) {
	if (message.numBytes != CONFIG_MODE_MESSAGE_LENGTH) {
		return;
	}

	bool enable = message.data[0] != 0;

	if (!configurationModeRequested && enable) {
		stagedConfiguration = vehicle::vehicleConfiguration;
	}
	configurationModeRequested = enable;

	bool enable = message.data[0] != 0;

	if (!configurationModeRequested && enable) {
		stagedConfiguration = vehicle::vehicleConfiguration;
	}
	configurationModeRequested = enable;
}


void Configurator::processCommandTorque(const drivers::can::Message& message) {
	if (message.numBytes != TORQUE_MESSAGE_LENGTH) {
void Configurator::processCommandTorque(const drivers::can::Message& message) {
	if (message.numBytes != TORQUE_MESSAGE_LENGTH) {
		return;
	}

	const uint32_t raw = static_cast<uint32_t>(message.data[0])		  |
						 static_cast<uint32_t>(message.data[1] << 8)  |
						 static_cast<uint32_t>(message.data[2] << 16) |
						 static_cast<uint32_t>(message.data[3] << 24);

	const float value = raw * TORQUE_MESSAGE_SCALE;

	if (value < 0 || value > torque::MAX_TORQUE_LIMIT_NM) {
	const uint32_t raw = static_cast<uint32_t>(message.data[0])		  |
						 static_cast<uint32_t>(message.data[1] << 8)  |
						 static_cast<uint32_t>(message.data[2] << 16) |
						 static_cast<uint32_t>(message.data[3] << 24);

	const float value = raw * TORQUE_MESSAGE_SCALE;

	if (value < 0 || value > torque::MAX_TORQUE_LIMIT_NM) {
		return;
	}
	stagedConfiguration.torqueConfig.maxTorqueNm = value;
}

void Configurator::processCommandThreshold(const drivers::can::Message& message) {
	if (message.numBytes != THRESHOLD_MESSAGE_LENGTH) {
void Configurator::processCommandThreshold(const drivers::can::Message& message) {
	if (message.numBytes != THRESHOLD_MESSAGE_LENGTH) {
		return;
	}

	vehicle::ThresholdConfig *thresholds;

	switch (message.id) {
	case CAN_ID_CMD_APP1_THRESHOLD:
		thresholds = &stagedConfiguration.pedalsConfig.app1Thresholds;
		break;
	case CAN_ID_CMD_APP2_THRESHOLD:
		thresholds = &stagedConfiguration.pedalsConfig.app2Thresholds;
		break;
	case CAN_ID_CMD_BSEF_THRESHOLD:
		thresholds = &stagedConfiguration.pedalsConfig.bsefThresholds;
		break;
	case CAN_ID_CMD_BSER_THRESHOLD:
		thresholds = &stagedConfiguration.pedalsConfig.bserThresholds;
		break;
	default:
		return;
	}

	const uint16_t rawFaultLo =		static_cast<uint16_t>(message.data[0]) |
									static_cast<uint16_t>(message.data[1] << 8);
	const uint16_t rawFaultHi =		static_cast<uint16_t>(message.data[2]) |
						 			static_cast<uint16_t>(message.data[3] << 8);
	const uint16_t rawSignalLo =	static_cast<uint16_t>(message.data[4]) |
									static_cast<uint16_t>(message.data[5] << 8);
	const uint16_t rawSignalHi =	static_cast<uint16_t>(message.data[6]) |
									static_cast<uint16_t>(message.data[7] << 8);

	const float faultLo = rawFaultLo * THRESHOLD_MESSAGE_SCALE;
	const float faultHi = rawFaultHi * THRESHOLD_MESSAGE_SCALE;
	const float signalLo = rawSignalLo * THRESHOLD_MESSAGE_SCALE;
	const float signalHi = rawSignalHi * THRESHOLD_MESSAGE_SCALE;

	thresholds->faultThresholds = {faultLo, faultHi};
	thresholds->signalThresholds = {signalLo, signalHi};
}

void Configurator::processCommandBSEEngage(const drivers::can::Message& message) {
	if (message.numBytes != BSE_ENGAGE_MESSAGE_LENGTH) {
		return;
	}

	const uint16_t rawBSEFEngage =		static_cast<uint16_t>(message.data[0]) |
										static_cast<uint16_t>(message.data[1] << 8);
	const uint16_t rawBSEREngage =		static_cast<uint16_t>(message.data[2]) |
						 				static_cast<uint16_t>(message.data[3] << 8);

	stagedConfiguration.pedalsConfig.bsefBrakeEngagedThreshold = rawBSEFEngage * THRESHOLD_MESSAGE_SCALE;
	stagedConfiguration.pedalsConfig.bserBrakeEngagedThreshold = rawBSEREngage * THRESHOLD_MESSAGE_SCALE;
}


void Configurator::processCommandPedalMap(const drivers::can::Message& message) {
	if (message.id < CAN_ID_CMD_PEDAL_MAP_BASE || message.id >= CAN_ID_CMD_PEDAL_MAP_BASE + vehicle::PedalMap::NUM_EDITABLE_POINTS) {
		return;
	}

	size_t index = message.id - CAN_ID_CMD_PEDAL_MAP_BASE + 1;

	const uint32_t raw = static_cast<uint32_t>(message.data[0])		  |
						 static_cast<uint32_t>(message.data[1] << 8)  |
						 static_cast<uint32_t>(message.data[2] << 16) |
						 static_cast<uint32_t>(message.data[3] << 24);
	const float value = raw * PEDAL_MAP_MESSAGE_SCALE;

	stagedConfiguration.torqueConfig.pedalMap.setPoint(index, value);
}


void Configurator::broadcastAPP1Parameter(vehicle::VehicleConfiguration &config) {
	packThreshold(config.pedalsConfig.app1Thresholds, txData);
	canBus.publishTxSlot(app1SlotHandle, txData, THRESHOLD_MESSAGE_LENGTH);
}
void Configurator::broadcastAPP2Parameter(vehicle::VehicleConfiguration &config) {
	packThreshold(config.pedalsConfig.app2Thresholds, txData);
	canBus.publishTxSlot(app2SlotHandle, txData, THRESHOLD_MESSAGE_LENGTH);
}
void Configurator::broadcastBSEFParameter(vehicle::VehicleConfiguration &config) {
	packThreshold(config.pedalsConfig.bsefThresholds, txData);
	canBus.publishTxSlot(bsefSlotHandle, txData, THRESHOLD_MESSAGE_LENGTH);
}
void Configurator::broadcastBSERParameter(vehicle::VehicleConfiguration &config) {
	packThreshold(config.pedalsConfig.bserThresholds, txData);
	canBus.publishTxSlot(bserSlotHandle, txData, THRESHOLD_MESSAGE_LENGTH);
}
void Configurator::broadcastBSEEngageParameter(vehicle::VehicleConfiguration &config) {
	const uint16_t scaledBSEFEngaged = static_cast<uint16_t>(config.pedalsConfig.bsefBrakeEngagedThreshold / THRESHOLD_MESSAGE_SCALE);
	const uint16_t scaledBSEREngaged = static_cast<uint16_t>(config.pedalsConfig.bserBrakeEngagedThreshold / THRESHOLD_MESSAGE_SCALE);
	txData[0] = static_cast<uint8_t>(scaledBSEFEngaged);
	txData[1] = static_cast<uint8_t>(scaledBSEFEngaged >> 8);
	txData[2] = static_cast<uint8_t>(scaledBSEREngaged);
	txData[3] = static_cast<uint8_t>(scaledBSEREngaged >> 8);
	canBus.publishTxSlot(bserSlotHandle, txData, THRESHOLD_MESSAGE_LENGTH);
}
void Configurator::broadcastTorqueParameter(vehicle::VehicleConfiguration &config) {
	const uint32_t scaled = config.torqueConfig.maxTorqueNm / TORQUE_MESSAGE_SCALE;
	txData[0] = static_cast<uint8_t>(scaled);
	txData[1] = static_cast<uint8_t>(scaled >> 8);
	txData[2] = static_cast<uint8_t>(scaled >> 16);
	txData[3] = static_cast<uint8_t>(scaled >> 24);
	canBus.publishTxSlot(torqueSlotHandle, txData, TORQUE_MESSAGE_LENGTH);
}
void Configurator::broadcastPedalMapParameters(vehicle::VehicleConfiguration &config) {
	for (size_t i = 0; i < vehicle::PedalMap::NUM_EDITABLE_POINTS; i++) {
		const uint16_t scaled = config.torqueConfig.pedalMap.getPoint(i) / PEDAL_MAP_MESSAGE_SCALE;
		txData[0] = static_cast<uint8_t>(scaled);
		txData[1] = static_cast<uint8_t>(scaled >> 8);
		canBus.publishTxSlot(pedalMapPointSlotHandles[i], txData, PEDAL_MAP_MESSAGE_LENGTH);
	}
}
void Configurator::broadcastParameters() {
	switch (vehicle::vehicleState.getMode()) {
	case vehicle::Mode::CONFIGURATION:
		broadcastAPP1Parameter(stagedConfiguration);
		broadcastAPP2Parameter(stagedConfiguration);
		broadcastBSEFParameter(stagedConfiguration);
		broadcastBSERParameter(stagedConfiguration);
		broadcastBSEEngageParameter(stagedConfiguration);
		broadcastPedalMapParameters(stagedConfiguration);
		broadcastTorqueParameter(stagedConfiguration);
		break;
	case vehicle::Mode::IDLE:
	case vehicle::Mode::READY_TO_DRIVE:
		broadcastTorqueParameter(vehicle::vehicleConfiguration);
		break;
	default:
		break;
	}
}

void Configurator::packThreshold(vehicle::ThresholdConfig thresholds, uint8_t *data) {
	uint16_t faultLo = static_cast<uint16_t>(thresholds.faultThresholds.getMin() / THRESHOLD_MESSAGE_SCALE);
	uint16_t faultHi = static_cast<uint16_t>(thresholds.faultThresholds.getMax() / THRESHOLD_MESSAGE_SCALE);
	uint16_t signalLo = static_cast<uint16_t>(thresholds.signalThresholds.getMin() / THRESHOLD_MESSAGE_SCALE);
	uint16_t signalHi = static_cast<uint16_t>(thresholds.signalThresholds.getMax() / THRESHOLD_MESSAGE_SCALE);


	data[0] = static_cast<uint8_t>(faultLo);
	data[1] = static_cast<uint8_t>(faultLo >> 8);
	data[2] = static_cast<uint8_t>(faultHi);
	data[3] = static_cast<uint8_t>(faultHi >> 8);
	data[4] = static_cast<uint8_t>(signalLo);
	data[5] = static_cast<uint8_t>(signalLo >> 8);
	data[6] = static_cast<uint8_t>(signalHi);
	data[7] = static_cast<uint8_t>(signalHi >> 8);
}

} // namespace drivers::configurator








