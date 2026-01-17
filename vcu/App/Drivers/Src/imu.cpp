#include "imu.hpp"
#include "stm32g4xx_hal.h"
#include "cmsis_os.h"
#include <cstring>

namespace drivers::IMU {

IMU::IMU() {
	// TODO: Potentially add constructor arguments (change imu.hpp to match)
	// TODO: Implement constructor
}

void IMU::init(){
}

State IMU::getState(){
	return state;
}

void IMU::updateState(Message m) {
	checksum = m.data[CSUM_LOC];
	memcpy(measurements, &m.data[3], 12*sizeof(uint8_t));
	if(m.data[0] == m.data[1] && m.data[0] == HEADER_BYTE){
		for(size_t i = 2; i < CSUM_LOC; i++){
			checksum -= m.data[i]; // index, yaw, pitch, roll, acceleration, reserved added together = checksum
			}
		if(checksum == 0 ){
			for(size_t i = 0; i<NUM_AXES*2; i++){
				i < NUM_AXES ? state.rotation[i] = ((measurements[i*2] << 8) + measurements[1+i*2])*DEGREE_SCALE :
							   state.acceleration[i] = ((measurements[i*2] << 8) + measurements[1+i*2])*MILLI_G_TO_MS2;
				}
			state.status = okPacket;
		}
		else {
			state.status = badPacket;
		}
	}
	else {
		state.status = badPacket;
	}
}

}
