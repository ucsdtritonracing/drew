#include "vehicle/drivers.hpp"
#include "rtos/objects.hpp"
#include "stm32g4xx_hal.h"



void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc == vehicle::pedalsDriver.getHADC()) {
		osThreadFlagsSet(rtos::PedalsTask.getHandle(), rtos::tasks::PedalsTask::PEDAL_BUFFER_HALF_COMPLETE_FLAG);
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc == vehicle::pedalsDriver.getHADC()) {
		osThreadFlagsSet(rtos::PedalsTask.getHandle(), rtos::tasks::PedalsTask::PEDAL_BUFFER_FULL_COMPLETE_FLAG);
	}
}
