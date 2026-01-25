#include "example_task.hpp"
#include "cmsis_os.h"
#include <stdio.h>

namespace tasks {

void ExampleTask::setup() {
	// initializing methods
	counter = 100;
}

void ExampleTask::loop() {
	counter++;
	osDelay(DELAY);
}

} // namespace tasks
