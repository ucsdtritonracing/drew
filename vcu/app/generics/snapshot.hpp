#pragma once

#include <atomic>
#include <stdint.h>

/*
 * Double-buffering template.
 *
 * Guarantees no torn writes for a single-producer multi-consumer data flow.
 */

template <typename T>
class Snapshot {
public:
	Snapshot() : readIndex(0) {}

	void update(const T& newData) {
		uint8_t writeIndex = readIndex.load(std::memory_order_relaxed) ^ 1;
		buffer[writeIndex] = newData;
		readIndex.store(writeIndex, std::memory_order_release);
	}

	T get() const {
		uint8_t index = readIndex.load(std::memory_order_acquire);
		return buffer[index];
	}

private:
	T buffer[2];
	std::atomic<uint8_t> readIndex;
};
