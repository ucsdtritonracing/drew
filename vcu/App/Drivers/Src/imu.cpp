#include "imu.hpp"
#include "stm32g4xx_hal.h"

namespace drivers::IMU {

IMU::IMU() {
	// TODO: Potentially add constructor arguments (change imu.hpp to match)
	// TODO: Implement constructor
}

State getState() {
	// TODO: Implement state getter
	return NULL;
}

void poll() {
	// TODO: Implement polling using I2C methods in STM32 HAL library
	// TODO: Choose interrupt, DMA-interrupt, or blocking (which is better)
	// 			- keep in mind, being called from FreeRTOS task
	// TODO: Update state variable
}

}
