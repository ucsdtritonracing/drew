#pragma once

#include "vehicle/types/mode_types.hpp"


namespace drivers::broadcaster {

struct Flags {
	bool r2dbPressed;
	bool sdcClosed;
	bool appFault;
	bool abppcFault;
	vehicle::Mode mode;
};

} // namespace drivers::broadcaster
