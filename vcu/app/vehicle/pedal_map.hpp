#pragma once

#include <stddef.h>


namespace vehicle {

class PedalMap {
public:
	static constexpr size_t NUM_EDITABLE_POINTS = 16;
	static constexpr size_t NUM_TOTAL_POINTS = NUM_EDITABLE_POINTS + 2;

	PedalMap();

	float getPedalOutput(float pedalInput) const;
	float getPoint(size_t index) const;
	void setPoint(size_t index, float value);
	bool isValid() const;
private:
	float points[NUM_TOTAL_POINTS]{};
};

} // namespace vehicle
