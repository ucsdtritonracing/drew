#pragma once

namespace drivers::IMU {

struct State {

};

class IMU {
public:
	IMU();

	/*
	 * Get the current state of the IMU
	 * 	- include all data the IMU can read (probably accel, gyro, any other sensor fusion options)
	 */
	State getState();

	/*
	 * Poll the IMU for new data using I2C
	 * 	- use STM32 HAL methods
	 */
	void poll();

private:
	State state;
};

}
