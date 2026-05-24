#include "vehicle/pedal_map.hpp"
#include <cstdint>
#include <cstring>
#include <cmath>


namespace vehicle {

PedalMap::PedalMap() {
	points[0] = 0;
	points[NUM_TOTAL_POINTS - 1] = 1;
	for (size_t i = 1; i <= NUM_EDITABLE_POINTS; i++) {
		const float input = static_cast<float>(i) / (NUM_TOTAL_POINTS - 1);
		points[i] = 1.0f / (1.0f + std::exp(-11.0f * (input - 0.5f)));
	}
}

float PedalMap::getPedalOutput(float pedalInput) const {
	if (pedalInput < 0) {
		return 0;
	}
	if (pedalInput >= 1) {
		return 1;
	}

	const size_t numSegments = NUM_TOTAL_POINTS - 1;
	const size_t index = static_cast<size_t>(pedalInput * numSegments);

	const float low = points[index];
	const float high = points[index + 1];

	const float offset = pedalInput - ((float) index / numSegments);
	return low + (numSegments) * (high - low) * offset;
}

void PedalMap::setPoint(size_t index, float value) {
	if (index >= NUM_EDITABLE_POINTS) {
		return;
	}
	if (value < 0 || value > 1) {
		return;
	}
	points[index + 1] = value;
}

float PedalMap::getPoint(size_t index) const {
	if (index == 0 || index > NUM_EDITABLE_POINTS) {
		return 0;
	}

	return points[index];
}

bool PedalMap::isValid() const {
	if (points[0] != 0 || points[NUM_TOTAL_POINTS - 1] != 1) {
		return false;
	}

	for (size_t i = 1; i < NUM_TOTAL_POINTS; i++) {
		if (points[i - 1] > points[i]) {
			return false;
		}
	}

	return true;
}

} // namespace vehicle
