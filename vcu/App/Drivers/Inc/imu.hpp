#pragma once
#include "cmsis_os.h"
#include "main.h"

namespace drivers::imu {

constexpr size_t NUM_AXES =    3;
constexpr size_t PACKET_SIZE = 19;

enum PacketStatus {
	badPacket, okPacket
};

extern uint8_t imuBuf[PACKET_SIZE];

struct State {
	float rotation[NUM_AXES] = {}; // yaw pitch roll
	float acceleration[NUM_AXES] = {}; // in xyz axes
	PacketStatus status; // flag to indicate if we received a bad packet, outdated data
};

/*
 * Pass updated IMU readings to driver
 */


class IMU {
public:
	IMU();
	void init(UART_HandleTypeDef *huart, uint8_t buf[]);
	/*
	 * Poll the IMU for updated data
	 *
	 */
	State getState();
	/*
	 * Pass updated IMU readings to driver
	 */

	void updateState(uint8_t m[]);
private:
	State state;
	uint8_t checksum;
	uint8_t measurements[NUM_AXES*4] = {};  // 2 bytes per axis, 2 sets of axes

	static constexpr uint8_t HEADER_BYTE = 		0xAA;
	static constexpr double DEGREE_SCALE = 		0.01;
	static constexpr double MILLI_G_TO_MS2 = 	0.0098067;
	static constexpr uint8_t CSUM_LOC = 		18; // location of checksum byte
};

} // namespace imu
