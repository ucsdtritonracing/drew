#include "drivers/pedals/pedals.hpp"
#include "drivers/pedals/adc.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "main.h"
#include "stm32g4xx_hal_adc.h"


namespace drivers::pedals {

void Pedals::init(ADC_HandleTypeDef& hadc) {
	this->hadc = &hadc;
	HAL_ADC_Start_DMA(&hadc, reinterpret_cast<uint32_t*>(buffer), BUFFER_SIZE * ADC_CHANNELS); 	// HAL expects uint32_t buffers, but DMA writes uint16_t
}

void Pedals::processBuffer(size_t start, size_t length) {
	const auto app1Thresholds = vehicle::vehicleConfiguration.app1Thresholds;
	const auto app2Thresholds = vehicle::vehicleConfiguration.app2Thresholds;
	const auto bsefThresholds = vehicle::vehicleConfiguration.bsefThresholds;
	const auto bserThresholds = vehicle::vehicleConfiguration.bserThresholds;

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
		if (app1Thresholds.faultThresholds.inRange(values.app1)) {
			app1Sum += values.app1;
			numAPP1Valid++;
		}
		if (app2Thresholds.faultThresholds.inRange(values.app2)) {
			app2Sum += values.app2;
			numAPP2Valid++;
		}
		if (bsefThresholds.faultThresholds.inRange(values.bsef)) {
			bsefSum += values.bsef;
			numBSEFValid++;
		}
		if (bserThresholds.faultThresholds.inRange(values.bser)) {
			bserSum += values.bser;
			numBSERValid++;
		}
	}

	const vehicle::pedals::State state = {
		.app1 = (numAPP1Valid == 0) ? 0 : app1Thresholds.normalize(app1Sum / numAPP1Valid),
		.app2 = (numAPP2Valid == 0) ? 0 : app2Thresholds.normalize(app2Sum / numAPP2Valid),
		.bsef = (numBSEFValid == 0) ? 0 : bsefThresholds.normalize(bsefSum / numBSEFValid),
		.bser = (numBSERValid == 0) ? 0 : bserThresholds.normalize(bserSum / numBSERValid),
		.app1Valid = (numAPP1Valid > 0),
		.app2Valid = (numAPP2Valid > 0),
		.bsefValid = (numBSEFValid > 0),
		.bserValid = (numBSERValid > 0)
	};

	vehicle::vehicleState.setPedals(state);
}

void Pedals::processHalfBuffer() {
	processBuffer(0, BUFFER_SIZE / 2);
}

void Pedals::processFullBuffer() {
	processBuffer(BUFFER_SIZE / 2, BUFFER_SIZE / 2);
}

const ADC_HandleTypeDef* Pedals::getHADC() {
	return hadc;
}

} // namespace drivers::pedals
