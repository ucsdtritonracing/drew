#pragma once

#include <cstddef>
#include <stdint.h>


namespace drivers::adc {

constexpr float ADC_MAX_VOLTAGE = 3.3f;
constexpr size_t ADC_MAX_VALUE = 4096;

constexpr float adcReadingToVoltage(uint16_t adcReading) {
	return adcReading * ADC_MAX_VOLTAGE / ADC_MAX_VALUE;
}

constexpr float voltageToAdcReading(float voltage) {
	return voltage * ADC_MAX_VALUE / ADC_MAX_VOLTAGE;
}

} // namespace drivers::adc
