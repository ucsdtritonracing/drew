#include "apps.hpp"
#include "main.h"
#include "stm32g4xx_hal_adc.h"


namespace drivers::apps {

void APPS::init(ADC_HandleTypeDef *hadc, float validLowThresholdVoltage, float validHighThresholdVoltage) {
	this->hadc = hadc;
	this->validLowThreshold = static_cast<uint16_t>(validLowThresholdVoltage / ADC_MAX_VOLTAGE * ADC_MAX_VALUE);
	this->validHighThreshold = static_cast<uint16_t>(validHighThresholdVoltage / ADC_MAX_VOLTAGE * ADC_MAX_VALUE);
	HAL_ADC_Start_DMA(hadc, reinterpret_cast<uint32_t*>(buffer), BUFFER_SIZE); 	// HAL expects uint32_t buffers, but DMA knows it is uint16_t
}

State APPS::processBuffer(size_t start, size_t length) {
	float value = 0;
	size_t numValid = 0;

	for (size_t i = start; i < start + length; i++) {
		if (buffer[i] >= validLowThreshold && buffer[i] <= validHighThreshold) {
			value += buffer[i];
			numValid++;
		}
	}

	if (numValid == 0) {
		return {0, false};
	}

	return {(value / numValid - validLowThreshold) / (validHighThreshold - validLowThreshold), true};
}

State APPS::processHalfBuffer() {
	return processBuffer(0, BUFFER_SIZE / 2);
}

State APPS::processFullBuffer() {
	return processBuffer(BUFFER_SIZE / 2, BUFFER_SIZE / 2);
}

const ADC_HandleTypeDef* APPS::getHADC() {
	return hadc;
}

} // namespace drivers::apps
