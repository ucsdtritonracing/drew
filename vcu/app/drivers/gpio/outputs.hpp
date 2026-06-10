#pragma once

#include "cmsis_os.h"
#include "stm32g4xx_hal.h"


namespace drivers::gpio {

// EV.9.7 Ready to Drive Sound
const uint32_t READY_TO_DRIVE_SOUND_DURATION_MS = 2000;		// EV.9.7.2.a

struct GPIOPin {
	GPIO_TypeDef *gpio;
	uint16_t pinNumber;
};

class Outputs {
public:
	Outputs();
	void init(GPIOPin soundPin, GPIOPin brakeLightPin);
	void playSound();
	void setBrakeLight(bool on);

private:
	void stopSound();
	static void soundTimerCallback(void *handle);

	GPIOPin soundPin;
	GPIOPin brakeLightPin;
	osTimerId_t soundTimer;
	StaticTimer_t soundTimerTCB;
};

} // namespace drivers::gpio
