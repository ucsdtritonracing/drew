#include "main.h"
#include "drivers/wheels/wheels.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "vehicle/drivers.hpp"



constexpr uint32_t getTIMActiveChannel(uint32_t channel) {
    switch (channel) {
        case TIM_CHANNEL_1: return HAL_TIM_ACTIVE_CHANNEL_1;
        case TIM_CHANNEL_2: return HAL_TIM_ACTIVE_CHANNEL_2;
        case TIM_CHANNEL_3: return HAL_TIM_ACTIVE_CHANNEL_3;
        case TIM_CHANNEL_4: return HAL_TIM_ACTIVE_CHANNEL_4;
        default: return HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    }
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	uint32_t now = HAL_GetTick();

	const drivers::wheels::WheelInput& fl = vehicle::wheelsDriver.getWheel(drivers::wheels::WheelId::FL);
	const drivers::wheels::WheelInput& fr = vehicle::wheelsDriver.getWheel(drivers::wheels::WheelId::FR);
	const drivers::wheels::WheelInput& rl = vehicle::wheelsDriver.getWheel(drivers::wheels::WheelId::RL);
	const drivers::wheels::WheelInput& rr = vehicle::wheelsDriver.getWheel(drivers::wheels::WheelId::RR);

	if (htim == fl.htim && htim->Channel == getTIMActiveChannel(fl.channel)) {
		uint32_t capture = HAL_TIM_ReadCapturedValue(htim, fl.channel);
		vehicle::wheelsDriver.onCapture(drivers::wheels::WheelId::FL, now, capture);
	} else if (htim == fr.htim && htim->Channel == getTIMActiveChannel(fr.channel)) {
		uint32_t capture = HAL_TIM_ReadCapturedValue(htim, fr.channel);
		vehicle::wheelsDriver.onCapture(drivers::wheels::WheelId::FR, now, capture);
	} else if (htim == rl.htim && htim->Channel == getTIMActiveChannel(rl.channel)) {
		uint32_t capture = HAL_TIM_ReadCapturedValue(htim, rl.channel);
		vehicle::wheelsDriver.onCapture(drivers::wheels::WheelId::RL, now, capture);
	} else if (htim == rr.htim && htim->Channel == getTIMActiveChannel(rr.channel)) {
		uint32_t capture = HAL_TIM_ReadCapturedValue(htim, rr.channel);
		vehicle::wheelsDriver.onCapture(drivers::wheels::WheelId::RR, now, capture);
	}
}
