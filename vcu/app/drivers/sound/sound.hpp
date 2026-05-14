#pragma once

#include "cmsis_os.h"
#include "stm32g4xx_hal.h"


namespace drivers::sound {

// EV.9.7 Ready to Drive Sound
const uint32_t READY_TO_DRIVE_SOUND_DURATION_MS = 2000;		// EV.9.7.2.a

class Sound {
public:
	Sound();
	void init(GPIO_TypeDef *gpio, uint16_t pin);
	void play();

private:
	void stop();
	static void timerCallback(void *handle);

	GPIO_TypeDef *gpio;
	uint16_t pin;
	osTimerId_t timer;
	StaticTimer_t tcb;
};

} // namespace drivers::sound
