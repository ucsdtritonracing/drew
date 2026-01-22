#pragma once

#include "vehicle_state.hpp"
#include "main.h"


namespace drivers::apps {

struct State {
	float app;
	bool valid;
};

class APPS {
public:
	/*
	 * @brief Initialize the APPS driver
	 *
	 * @param hadc ADC handle
	 */
	void init(ADC_HandleTypeDef *hadc, float validLowThresholdVoltage, float validHighThresholdVoltage);

	/*
	 * @brief Process and update the APP value from the first half of the buffer
	 */
	State processHalfBuffer();

	/*
	 * @brief Process and update the APP value from the second half of the buffer
	 */
	State processFullBuffer();

	/*
	 * @brief Get a pointer to the ADC DMA buffer
	 */
	const uint32_t* getBuffer();

	/*
	 * @brief Get the ADC handle associated with this APPS
	 */
	const ADC_HandleTypeDef* getHADC();


	static constexpr size_t BUFFER_SIZE = 64;

private:
	/*
	 * @brief Helper to process the APP from the raw buffer
	 *
	 * @param start Start index of the buffer to process from
	 * @param length Number of values to process from the buffer
	 */
	State processBuffer(size_t start, size_t length);

	ADC_HandleTypeDef *hadc;
	uint16_t buffer[BUFFER_SIZE];
	uint16_t validLowThreshold;
	uint16_t validHighThreshold;

	static constexpr float ADC_MAX_VOLTAGE = 3.3f;
	static constexpr size_t ADC_MAX_VALUE = 4096;
};

} // namespace drivers::apps
