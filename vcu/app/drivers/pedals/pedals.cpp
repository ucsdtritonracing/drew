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
	const auto app1Thresholds = vehicle::vehicleConfiguration.pedalsConfig.app1Thresholds;
	const auto app2Thresholds = vehicle::vehicleConfiguration.pedalsConfig.app2Thresholds;
	const auto bsefThresholds = vehicle::vehicleConfiguration.pedalsConfig.bsefThresholds;
	const auto bserThresholds = vehicle::vehicleConfiguration.pedalsConfig.bserThresholds;

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


	float app1Average = (numAPP1Valid == 0) ? 0 : app1Sum / numAPP1Valid;
	float app2Average = (numAPP2Valid == 0) ? 0 : app2Sum / numAPP2Valid;
	float bsefAverage = (numBSEFValid == 0) ? 0 : bsefSum / numBSEFValid;
	float bserAverage = (numBSERValid == 0) ? 0 : bserSum / numBSERValid;


	const vehicle::pedals::State state = {
		.app1 = app1Thresholds.normalize(app1Average),
		.app2 = app2Thresholds.normalize(app2Average),
		.bsef = bsefThresholds.normalize(bsefAverage),
		.bser = bserThresholds.normalize(bserAverage),
		.app1Raw = static_cast<uint16_t>(app1Average),
		.app2Raw = static_cast<uint16_t>(app2Average),
		.bsefRaw = static_cast<uint16_t>(bsefAverage),
		.bserRaw = static_cast<uint16_t>(bserAverage),
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
