#include "drivers/wheels/wheels.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "main.h"
#include "stm32g4xx_hal_tim.h"


namespace drivers::wheels {

void Wheels::init(WheelInput flwss, WheelInput frwss, WheelInput rlwss, WheelInput rrwss, uint32_t tickFrequency) {
	wheels[WheelId::FL] = std::move(flwss);
	wheels[WheelId::FR] = std::move(frwss);
	wheels[WheelId::RL] = std::move(rlwss);
	wheels[WheelId::RR] = std::move(rrwss);
	this->tickFrequency = tickFrequency;

	HAL_TIM_IC_Start_IT(flwss.htim, flwss.channel);
	HAL_TIM_IC_Start_IT(frwss.htim, frwss.channel);
	HAL_TIM_IC_Start_IT(rlwss.htim, rlwss.channel);
	HAL_TIM_IC_Start_IT(rrwss.htim, rrwss.channel);
}

void Wheels::filter(WheelData& data, size_t teeth, float metersPerRevolution, uint32_t now) {
	if (now - data.lastUpdateTime > Wheels::TIMEOUT_MS) {
		data.filteredSpeed = 0;
		data.filterInitialized = false;
		return;
	}

	if (data.latestPeriod == 0) {
		return;
	}

	float frequency = (float) Wheels::tickFrequency / (float) data.latestPeriod;
	float revolutionsPerSecond = frequency / (float) teeth;
    float rawSpeed = revolutionsPerSecond * metersPerRevolution;

	if (!data.filterInitialized) {
		data.filterInitialized = true;
		data.filteredSpeed = rawSpeed;
	} else {
		data.filteredSpeed = Wheels::ALPHA * rawSpeed + (1.0f - Wheels::ALPHA) * data.filteredSpeed;
	}
}

void Wheels::updateSpeeds() {
	uint32_t now = HAL_GetTick();
	filter(data[WheelId::FL], vehicle::vehicleConfiguration.frontTriggerWheelTeeth, vehicle::vehicleConfiguration.frontWheelMetersPerRevolution, now);
	filter(data[WheelId::FR], vehicle::vehicleConfiguration.frontTriggerWheelTeeth, vehicle::vehicleConfiguration.frontWheelMetersPerRevolution, now);
	filter(data[WheelId::RL], vehicle::vehicleConfiguration.rearTriggerWheelTeeth, vehicle::vehicleConfiguration.rearWheelMetersPerRevolution, now);
	filter(data[WheelId::RR], vehicle::vehicleConfiguration.rearTriggerWheelTeeth, vehicle::vehicleConfiguration.rearWheelMetersPerRevolution, now);

	vehicle::vehicleState.setWheelSpeeds(vehicle::wheels::State{
		data[WheelId::FL].filteredSpeed,
		data[WheelId::FR].filteredSpeed,
		data[WheelId::RL].filteredSpeed,
		data[WheelId::RR].filteredSpeed
	});
}

void Wheels::onCapture(WheelId id, uint32_t now, uint32_t capture) {
	WheelData& d = data[id];
	if (d.initialized) {
		if (capture >= d.lastCapture) {
			d.latestPeriod = capture - d.lastCapture;
		} else {
			d.latestPeriod = wheels[id].htim->Init.Period - d.lastCapture + capture + 1;
		}
	}
	d.lastCapture = capture;
	d.lastUpdateTime = now;
	d.initialized = true;
}

const WheelInput& Wheels::getWheel(WheelId id) const {
	return wheels[id];
}

} // namespace drivers::wheels
