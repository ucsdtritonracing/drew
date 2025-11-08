#pragma once
#include "cmsis_os.h"
#include <type_traits>

namespace tasks {

template<typename Derived, osPriority_t Priority = osPriorityNormal, uint32_t StackSize = 128>
class Task {
public:
	Task() = default;

	Task(Task&&) 					= delete;
	Task(const Task&)				= delete;
	Task& operator=(const Task&) 	= delete;
	Task& operator=(Task&&) 		= delete;

	void start(const char* name) {
		if (taskHandle) {
			return;
		}
		osThreadAttr_t attr = {};
		attr.name = name;
		attr.stack_size = StackSize;
		attr.priority = Priority;
		taskHandle = osThreadNew(taskEntry, this, &attr);
	}

	osThreadId_t getHandle() {
		return taskHandle;
	}

protected:
	osThreadId_t taskHandle = nullptr;

private:
	static void taskEntry(void* arg) {
		Derived* self = static_cast<Derived*>(arg);

		if constexpr (has_setup<Derived>::value) {
			self->setup();
		}

		static_assert(has_loop<Derived>::value, "Task must implement loop() method");
		for (;;) {
			self->loop();
		}
	}

	/*
	 * Template magic to deduce at compile time if the derived class has setup and loop methods
	 * Provide more informative errors at compile time
	 */
	template<typename T, typename = void>
	struct has_setup : std::false_type {};

	template<typename T>
	struct has_setup<T, std::void_t<decltype(std::declval<T>().setup())>> : std::true_type {};


	template<typename T, typename = void>
	struct has_loop : std::false_type {};

	template<typename T>
	struct has_loop<T, std::void_t<decltype(std::declval<T>().loop())>> : std::true_type {};
};

} // namespace tasks
