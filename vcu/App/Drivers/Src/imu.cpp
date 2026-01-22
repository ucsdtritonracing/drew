#include "imu.hpp"
#include "stm32g4xx_hal.h"
#include "cmsis_os.h"
#include <cstring>
#include <stdio.h>
#include "main.h"

namespace drivers::imu {



IMU::IMU() {
}

void IMU::init(UART_HandleTypeDef *huart) {
	if (HAL_UART_Receive_DMA(huart, imuBuf, drivers::imu::PACKET_SIZE) != HAL_OK) {
		Error_Handler();
	}
}

const State& IMU::getState() {
	return state;
}
uint8_t* IMU::getBuffer() {
	return imuBuf;
}

void IMU::updateState() {
	checksum = imuBuf[CHECKSUM_INDEX];
	memcpy(measurements, &imuBuf[3], 12 * sizeof(uint8_t));
	if (imuBuf[0] == imuBuf[1] && imuBuf[0] == HEADER_BYTE) {
		for (size_t i = 2; i < CHECKSUM_INDEX; i++) {
			checksum -= imuBuf[i]; // index, yaw, pitch, roll, acceleration, reserved added together = checksum
		}
		if (checksum == 0) {
			for (size_t i = 0; i < NUM_AXES * 2; i++) {
				if (i < NUM_AXES) {
					state.rotation[i] =
							static_cast<float>(static_cast<int16_t>((measurements[i * 2]
									| (measurements[1 + i * 2] << 8))))
									* DEGREE_SCALE;
				} else {
					state.acceleration[i - 3] =
							static_cast<float>(static_cast<int16_t>(((measurements[i * 2])
									| (measurements[1 + i * 2] << 8))))
									* MILLI_G_TO_MS2;
				}
			}
			state.status = okPacket;
		} else {
			state.status = badPacket;
		}
	} else {
		state.status = badPacket;
	}
}

} // namespace imu
