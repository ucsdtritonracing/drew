#include "task_init.hpp"
#include "example_task.hpp"

extern "C" void initTasks(void) {
    static tasks::ExampleTask exampleTask;
    exampleTask.start("ExampleTask");
}
