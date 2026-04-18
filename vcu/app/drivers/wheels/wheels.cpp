#include "drivers/wheels/wheels.hpp"
#include "vehicle/vehicle_state.hpp"
#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "main.h"
#include "stm32g4xx_hal_tim.h"


namespace drivers::wheels {

void Wheels::init(WheelInput flwss, WheelInput frwss, WheelInput rlwss, WheelInput rrwss, uint32_t pclk) {
	wheels[WheelId::FL] = std::move(flwss);
	wheels[WheelId::FR] = std::move(frwss);
	wheels[WheelId::RL] = std::move(rlwss);
	wheels[WheelId::RR] = std::move(rrwss);
	this->pclk = pclk;

	HAL_TIM_IC_Start_IT(flwss.htim, flwss.channel);
	HAL_TIM_IC_Start_IT(frwss.htim, frwss.channel);
	HAL_TIM_IC_Start_IT(rlwss.htim, rlwss.channel);
	HAL_TIM_IC_Start_IT(rrwss.htim, rrwss.channel);
}

void filter(WheelData& data, size_t teeth, float metersPerRevolution, uint32_t now) {
	if (now - data.lastUpdateTime > Wheels::TIMEOUT_MS) {
		data.filteredSpeed = 0;
		data.filterInitialized = false;
		return;
	}

	if (data.latestPeriod == 0) {
		return;
	}

	float frequency = (float) Wheels::pclk / (float) data.latestPeriod;
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
	filter(data[WheelId::FL], vehicle::VehicleConfiguration::FRONT_TRIGGER_WHEEL_TEETH, vehicle::VehicleConfiguration::FRONT_WHEEL_METERS_PER_REVOLUTION, now);
	filter(data[WheelId::FR], vehicle::VehicleConfiguration::FRONT_TRIGGER_WHEEL_TEETH, vehicle::VehicleConfiguration::FRONT_WHEEL_METERS_PER_REVOLUTION, now);
	filter(data[WheelId::RL], vehicle::VehicleConfiguration::REAR_TRIGGER_WHEEL_TEETH, vehicle::VehicleConfiguration::REAR_WHEEL_METERS_PER_REVOLUTION, now);
	filter(data[WheelId::RR], vehicle::VehicleConfiguration::REAR_TRIGGER_WHEEL_TEETH, vehicle::VehicleConfiguration::REAR_WHEEL_METERS_PER_REVOLUTION, now);

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
		d.latestPeriod = capture - d.lastCapture;
	}
	d.lastCapture = capture;
	d.lastUpdateTime = now;
	d.initialized = true;
}

const WheelInput& Wheels::getWheel(WheelId id) const {
	return wheels[id];
}

} // namespace drivers::wheels
