#pragma once

#include "drivers/pedals/adc.hpp"
#include <stdint.h>


namespace vehicle {

class Range {
public:
	Range(float min, float max)
		: min(min), max(max),
		  minAdcReading(min * drivers::adc::ADC_MAX_VALUE),
		  maxAdcReading(max * drivers::adc::ADC_MAX_VALUE)
	{};

    bool validRange() const {
    	return (max > min) && (max <= 1) && (min >= 0);
    }

    bool inRange(uint16_t adcReading) const {
    	return adcReading >= minAdcReading && adcReading <= maxAdcReading;
    }

    float getMin() const { return min; }
    float getMax() const { return max; }
    uint16_t getMinAdcReading() const { return minAdcReading; }
    uint16_t getMaxAdcReading() const { return maxAdcReading; }

private:
    float min;
    float max;
    uint16_t minAdcReading;
    uint16_t maxAdcReading;
};

struct AnalogCalibration {
	Range faultThresholds;	// indicates faults if out of range
	Range signalThresholds;	// maps from 0 - 1

	float normalize(float adcReading) const {
    	if (!faultThresholds.inRange(adcReading)) {
    		return 0;
    	}
    	if (adcReading < signalThresholds.getMinAdcReading()) {
    		return 0;
    	}
    	if (adcReading > signalThresholds.getMaxAdcReading()) {
    		return 1;
    	}

    	return (adcReading - signalThresholds.getMinAdcReading()) / (signalThresholds.getMaxAdcReading() - signalThresholds.getMinAdcReading());
	}
};

} // namespace vehicle
