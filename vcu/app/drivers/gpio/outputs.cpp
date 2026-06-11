#include "drivers/gpio/outputs.hpp"
#include "stm32g4xx_hal.h"
#include "cmsis_os.h"


namespace drivers::gpio {

Outputs::Outputs() {
    osTimerAttr_t attributes = {
        .name = "Sound",
		.attr_bits = 0x0,
        .cb_mem = &soundTimerTCB,
        .cb_size = sizeof(soundTimerTCB)
    };

	soundTimer = osTimerNew(soundTimerCallback, osTimerOnce, this, &attributes);
}

void Outputs::init(GPIOPin soundPin, GPIOPin brakeLightPin) {
	this->soundPin = soundPin;
	this->brakeLightPin = brakeLightPin;
}

void Outputs::playSound() {
	osTimerStop(soundTimer);
	HAL_GPIO_WritePin(soundPin.gpio, soundPin.pinNumber, GPIO_PIN_SET);
	osTimerStart(soundTimer, pdMS_TO_TICKS(READY_TO_DRIVE_SOUND_DURATION_MS));
}

void Outputs::stopSound() {
	HAL_GPIO_WritePin(soundPin.gpio, soundPin.pinNumber, GPIO_PIN_RESET);
}

void Outputs::soundTimerCallback(void *handle) {
	Outputs *self = static_cast<Outputs*>(handle);
	self->stopSound();
}

void Outputs::setBrakeLight(bool on) {
	HAL_GPIO_WritePin(brakeLightPin.gpio, brakeLightPin.pinNumber, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

} // namespace drivers::gpio
