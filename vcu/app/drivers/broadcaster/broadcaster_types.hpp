#pragma once


namespace drivers::broadcaster {

struct Flags {
	bool r2dbPressed;
	bool sdcClosed;
	bool r2dEnabled;
	bool appFault;
	bool abppcFault;
};

} // namespace drivers::broadcaster
