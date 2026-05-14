#include "drivers/sound/sound.hpp"
#include "stm32g4xx_hal.h"
#include "cmsis_os.h"


namespace drivers::sound {

Sound::Sound() {
    osTimerAttr_t attributes = {
        .name = "Sound",
		.attr_bits = 0x0,
        .cb_mem = &tcb,
        .cb_size = sizeof(tcb)
    };

	timer = osTimerNew(timerCallback, osTimerOnce, this, &attributes);
}

void Sound::init(GPIO_TypeDef *gpio, uint16_t pin) {
	this->gpio = gpio;
	this->pin = pin;
}

void Sound::play() {
	osTimerStop(timer);
	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
	osTimerStart(timer, pdMS_TO_TICKS(READY_TO_DRIVE_SOUND_DURATION_MS));
}

void Sound::stop() {
	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}

void Sound::timerCallback(void *handle) {
	Sound *self = static_cast<Sound*>(handle);
	self->stop();
}

} // namespace drivers::sound
