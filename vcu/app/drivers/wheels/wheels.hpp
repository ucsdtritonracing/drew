#pragma once

#include "vehicle/types/wheels_types.hpp"
#include "generics/snapshot.hpp"
#include "stm32g4xx_hal.h"


namespace drivers::wheels {

struct WheelInput {
	TIM_HandleTypeDef* htim;
	uint32_t channel;
};

struct WheelData {
	uint32_t latestPeriod;
	uint32_t lastCapture;
	uint32_t lastUpdateTime;
	bool initialized = false;

    float filteredSpeed;
    bool filterInitialized = false;
};

enum WheelId {
	FL, FR, RL, RR
};

class Wheels {
public:
	/*
	 * @brief Initialize the wheel speed sensor driver
	 *
	 * @param flwss Front Left Wheel Speed Sensor input timer and channel
	 * @param frwss Front Right Wheel Speed Sensor input timer and channel
	 * @param rlwss Rear Left Wheel Speed Sensor input timer and channel
	 * @param rrwss Rear Right Wheel Speed Sensor input timer and channel
	 * @param pclk PCLK1 clock frequency in Hz
	 */
	void init(WheelInput flwss, WheelInput frwss, WheelInput rlwss, WheelInput rrwss, uint32_t pclk);

	void updateSpeeds();
	void onCapture(WheelId id, uint32_t currentTick, uint32_t capture);
	const WheelInput& getWheel(WheelId id) const;

private:
	static constexpr size_t NUM_WHEELS = 4;
	static constexpr size_t TIMEOUT_MS = 500;
	static constexpr float ALPHA = 0.2f;
	void filter(WheelData& data, size_t teeth, float metersPerRevolution, uint32_t now);

	WheelData data[NUM_WHEELS];
	WheelInput wheels[NUM_WHEELS];

	uint32_t pclk;
};

} // namespace drivers::pedals
