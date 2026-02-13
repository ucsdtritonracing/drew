#pragma once
#include "cmsis_os.h"
#include "freertos.h"
#include <type_traits>


namespace tasks {

/*
 * Task template class.
 *
 *
 * Create a new task by providing your derived class as the template parameter, e.g:
 *
 * namespace tasks {
 * class ExampleTask : public Task<ExampleTask> {
 * public:
 * 		void setup();
 *		void loop();
 * };
 * }
 */
template<typename Derived, osPriority_t Priority = osPriorityNormal, uint32_t StackSize = 128>
class Task {
public:
	Task() = default;

	void start(const char* name) {
		if (taskHandle) {
			return;
		}
//		osThreadAttr_t attr = {};
//		attr.name = name;
//		attr.stack_size = StackSize;
//		attr.priority = Priority;
//		taskHandle = osThreadNew(taskEntry, this, &attr);

		const osThreadAttr_t attr = {
		  .name = name,
		  .attr_bits = osThreadDetached,
		  .cb_mem = &tcb,
		  .cb_size = sizeof(StaticTask_t),
		  .stack_mem = stack,
		  .stack_size = sizeof(stack),
		  .priority = osPriorityNormal,
		};
		taskHandle = osThreadNew(taskEntry, this, &attr);
	}

	osThreadId_t getHandle() {
		return taskHandle;
	}

protected:
	osThreadId_t taskHandle = nullptr;

private:
	StaticTask_t tcb;
	StackType_t stack[StackSize];

	static void taskEntry(void* arg) {
		Derived* self = static_cast<Derived*>(arg);

		if constexpr (has_setup<Derived>::value) {
			self->setup();
		}

		static_assert(has_loop<Derived>::value, "Task must implement loop() method");
		while (true) {
			self->loop();
		}
	}

	/*
	 * Template magic to deduce at compile time if the derived class has setup and loop methods.
	 * Combined with static_assert to provide more informative errors at compile time.
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
