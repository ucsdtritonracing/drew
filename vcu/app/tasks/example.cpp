#include "tasks/example.hpp"
#include "cmsis_os.h"
#include <stdio.h>


namespace tasks {

void ExampleTask::setup() {
	// initializing methods
}

void ExampleTask::loop() {
	printf("hello!");
	osDelay(DELAY);
}

} // namespace tasks
