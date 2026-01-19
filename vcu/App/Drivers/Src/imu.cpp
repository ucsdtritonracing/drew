#include "imu.hpp"
#include "stm32g4xx_hal.h"
#include "cmsis_os.h"
#include <cstring>
#include <stdio.h>
#include "main.h"

namespace drivers::imu {

uint8_t imuBuf[PACKET_SIZE] = {};

IMU::IMU() {}

void IMU::init(UART_HandleTypeDef *huart, uint8_t buf[]) {
	if (HAL_UART_Receive_DMA(huart, buf, drivers::imu::PACKET_SIZE) != HAL_OK) {
		Error_Handler();
	}
}

State IMU::getState() {
	return state;
}

void IMU::updateState(uint8_t m[]) {
	checksum = m[CSUM_LOC];
	memcpy(measurements, &m[3], 12 * sizeof(uint8_t));
	if (m[0] == m[1] && m[0] == HEADER_BYTE) {
		for (size_t i = 2; i < CSUM_LOC; i++) {
			checksum -= m[i]; // index, yaw, pitch, roll, acceleration, reserved added together = checksum
		}
		if (checksum == 0) {
			for (size_t i = 0; i < NUM_AXES * 2; i++) {
				if (i < NUM_AXES) {
					state.rotation[i] =
							static_cast<float>(static_cast<int16_t>((measurements[i* 2]
									| (measurements[1 + i * 2] << 8))))
									* DEGREE_SCALE;
				} else {
					state.acceleration[i - 3] =
							static_cast<float>(static_cast<int16_t>(((measurements[i* 2])
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
