#include "drivers/storage/storage.hpp"

namespace drivers::storage {

bool ConfigStorage::save(const vehicle::VehicleConfiguration &config) {
	Record record;
	record.magic = CONFIG_MAGIC;
	record.config = config;
	record.checksum = computeChecksum(config);

	return eraseAndWrite(record);
}

bool ConfigStorage::load(vehicle::VehicleConfiguration &config) {
	const auto *stored = reinterpret_cast<const Record*>(CONFIG_FLASH_ADDR);

	if (stored->magic != CONFIG_MAGIC || stored->checksum != computeChecksum(stored->config)) {
		return false;
	}
	if (stored->config.version != CONFIG_VERSION) {
		return false;
	}
	if (!stored->config.valid()) {
		return false;
	}

	config = stored->config;
	return true;
}

bool ConfigStorage::eraseAndWrite(const Record &stored) {
	HAL_FLASH_Unlock();

	// Erase the page
	FLASH_EraseInitTypeDef eraseInit { };
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInit.Banks = FLASH_BANK_2;
	eraseInit.Page = 127; // Last page of bank 2
	eraseInit.NbPages = 1;

	uint32_t pageError = 0;
	if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
		HAL_FLASH_Lock();
		return false;
	}

	// Write 64 bits at a time
	const uint64_t *data = reinterpret_cast<const uint64_t*>(&stored);
	size_t numDoublewords = (sizeof(Record) + 7) / 8;

	for (size_t i = 0; i < numDoublewords; i++) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, CONFIG_FLASH_ADDR + i * 8, data[i]) != HAL_OK) {
			HAL_FLASH_Lock();
			return false;
		}
	}

	HAL_FLASH_Lock();
	return true;
}

uint32_t ConfigStorage::computeChecksum(const vehicle::VehicleConfiguration &config) {
	const uint8_t *data = reinterpret_cast<const uint8_t*>(&config);
	uint32_t crc = 0;
	for (size_t i = 0; i < sizeof(vehicle::VehicleConfiguration); i++) {
		crc ^= data[i];
	}
	return crc;
}

} // namespace drivers::storage
