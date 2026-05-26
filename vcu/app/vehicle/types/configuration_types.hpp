#pragma once

#include "drivers/pedals/adc.hpp"
#include <stdint.h>


namespace vehicle {

class VoltageRange {
public:
	VoltageRange(float min, float max)
		: min(min), max(max),
		  minAdcReading(drivers::adc::voltageToAdcReading(min)),
		  maxAdcReading(drivers::adc::voltageToAdcReading(max))
	{};

    bool validRange() const {
    	return max > min;
    }

    bool inRange(uint16_t adcReading) const {
    	return adcReading >= minAdcReading && adcReading <= maxAdcReading;
    }

    uint16_t getMin() const { return min; }
    uint16_t getMax() const { return max; }
    uint16_t getMinAdcReading() const { return minAdcReading; }
    uint16_t getMaxAdcReading() const { return maxAdcReading; }
    void setMin(float value) {
    	min = value;
    	minAdcReading = drivers::adc::voltageToAdcReading(min);
    }
    void setMax(float value) {
    	max = value;
    	maxAdcReading = drivers::adc::voltageToAdcReading(max);
    }

private:
    float min;
    float max;
    uint16_t minAdcReading;
    uint16_t maxAdcReading;
};

struct AnalogCalibration {
	VoltageRange faultThresholds;	// indicates faults if out of range
	VoltageRange signalThresholds;	// maps from 0 - 1

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
