#include "pedals.hpp"
#include "vehicle_state_types.hpp"
#include "vehicle_configuration.hpp"
#include "main.h"
#include "stm32g4xx_hal_adc.h"


namespace drivers::pedals {

void Pedals::init(ADC_HandleTypeDef& hadc) {
	this->hadc = &hadc;
	HAL_ADC_Start_DMA(&hadc, reinterpret_cast<uint32_t*>(buffer), BUFFER_SIZE * ADC_CHANNELS); 	// HAL expects uint32_t buffers, but DMA writes uint16_t
}

vehicle::Pedals Pedals::processBuffer(size_t start, size_t length) {
	const float app1Lo = vehicle::vehicleConfiguration.apps1LoThresholdVoltage;
	const float app1Hi = vehicle::vehicleConfiguration.apps1HiThresholdVoltage;
	const float app2Lo = vehicle::vehicleConfiguration.apps2LoThresholdVoltage;
	const float app2Hi = vehicle::vehicleConfiguration.apps2HiThresholdVoltage;

	const float bsefLo = vehicle::vehicleConfiguration.bsefLoThresholdVoltage;
	const float bsefHi = vehicle::vehicleConfiguration.bsefHiThresholdVoltage;
	const float bserLo = vehicle::vehicleConfiguration.bserLoThresholdVoltage;
	const float bserHi = vehicle::vehicleConfiguration.bserHiThresholdVoltage;

	float app1Sum = 0;
	float app2Sum = 0;
	float bsefSum = 0;
	float bserSum = 0;

	size_t numAPP1Valid = 0;
	size_t numAPP2Valid = 0;
	size_t numBSEFValid = 0;
	size_t numBSERValid = 0;

	for (size_t i = start; i < start + length; i++) {
		ADCReading values = buffer[i];
		if (values.app1 >= app1Lo && values.app1 <= app1Hi) {
			app1Sum += values.app1;
			numAPP1Valid++;
		}
		if (values.app2 >= app2Lo && values.app2 <= app2Hi) {
			app2Sum += values.app2;
			numAPP2Valid++;
		}
		if (values.bsef >= bsefLo && values.bsef <= bsefHi) {
			bsefSum += values.bsef;
			numBSEFValid++;
		}
		if (values.bser >= bserLo && values.bser <= bserHi) {
			bserSum += values.bser;
			numBSERValid++;
		}
	}

	const vehicle::Pedals out = {
		.app1 = (numAPP1Valid == 0) ? 0 : (app1Sum / numAPP1Valid - app1Lo) / (app1Hi - app1Lo),
		.app2 = (numAPP2Valid == 0) ? 0 : (app2Sum / numAPP2Valid - app2Lo) / (app2Hi - app2Lo),
		.bsef = (numBSEFValid == 0) ? 0 : (bsefSum / numBSEFValid - bsefLo) / (bsefHi - bsefLo),
		.bser = (numBSERValid == 0) ? 0 : (bserSum / numBSERValid - bserLo) / (bserHi - bserLo),
		.app1Valid = (numAPP1Valid > 0),
		.app2Valid = (numAPP2Valid > 0),
		.bsefValid = (numBSEFValid > 0),
		.bserValid = (numBSERValid > 0)
	};

	return out;
}

vehicle::Pedals Pedals::processHalfBuffer() {
	return processBuffer(0, BUFFER_SIZE / 2);
}

vehicle::Pedals Pedals::processFullBuffer() {
	return processBuffer(BUFFER_SIZE / 2, BUFFER_SIZE / 2);
}

const ADC_HandleTypeDef* Pedals::getHADC() {
	return hadc;
}

} // namespace drivers::pedals
