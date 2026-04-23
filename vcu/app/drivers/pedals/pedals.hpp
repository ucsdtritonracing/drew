#pragma once

#include "vehicle/types/pedals_types.hpp"
#include "generics/snapshot.hpp"
#include "stm32g4xx_hal.h"


namespace drivers::pedals {

class Pedals {
public:
	/*
	 * @brief Initialize the Pedals driver
	 *
	 * @param hadc The ADC handle for this driver
	 */
	void init(ADC_HandleTypeDef& hadc);

	/*
	 * @brief Process and update from the first half of the buffer
	 */
	void processHalfBuffer();

	/*
	 * @brief Process and update from the second half of the buffer
	 */
	void processFullBuffer();

	/*
	 * @brief Get a pointer to the ADC DMA buffer
	 */
	const uint32_t* getBuffer();

	/*
	 * @brief Get the ADC handle
	 */
	const ADC_HandleTypeDef* getHADC();


	static constexpr size_t BUFFER_SIZE = 32;

private:
	struct ADCReading {
		uint16_t app1;
		uint16_t app2;
		uint16_t bsef;
		uint16_t bser;
	};

	/*
	 * @brief Helper to process from the raw buffer
	 *
	 * @param start Start index of the buffer to process from
	 * @param length Number of values to process from the buffer
	 */
	void processBuffer(size_t start, size_t length);

	ADC_HandleTypeDef *hadc;
	ADCReading buffer[BUFFER_SIZE];

	static constexpr float ADC_MAX_VOLTAGE = 3.3f;
	static constexpr size_t ADC_MAX_VALUE = 4096;
	static constexpr size_t ADC_CHANNELS = 4;
};

} // namespace drivers::pedals
