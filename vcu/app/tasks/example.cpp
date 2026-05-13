#include "tasks/example.hpp"
#include "cmsis_os.h"
#include <stdio.h>


namespace rtos::tasks {

void ExampleTask::setup() {
	// initializing methods
}

void ExampleTask::loop() {
	printf("hello!");
	osDelay(DELAY);
}

} // namespace rtos::tasks
