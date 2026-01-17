#pragma once
#include "cmsis_os.h"

namespace drivers::imu {

constexpr size_t NUM_AXES =    3;
constexpr size_t PACKET_SIZE = 19;

struct State;

enum PacketStatus {
	badPacket, okPacket
};
struct Message {
	uint8_t data[PACKET_SIZE] = {};
};
struct State {
	uint16_t rotation[NUM_AXES] = {}; // yaw pitch roll
	uint16_t acceleration[NUM_AXES] = {}; // in xyz axes
	PacketStatus status; // flag to indicate if we received a bad packet, outdated data
};

/*
 * Pass updated IMU readings to driver
 */


class IMU {
public:
	IMU();
	void init();
	/*
	 * Poll the IMU for updated data
	 *
	 */
	State getState();
	/*
	 * Pass updated IMU readings to driver
	 */

	void updateState(Message m);
private:
	State state;
	uint8_t checksum;
	uint8_t measurements[NUM_AXES*4] = {};  //

	static constexpr uint8_t HEADER_BYTE = 		0xAA;
	static constexpr double DEGREE_SCALE = 		0.01;
	static constexpr double MILLI_G_TO_MS2 = 	0.0098067;
	static constexpr uint8_t CSUM_LOC = 		18;
};

} // namespace imu
