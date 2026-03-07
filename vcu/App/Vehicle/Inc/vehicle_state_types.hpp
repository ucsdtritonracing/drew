namespace vehicle {

struct Pedals {
	float app1 = 0;
	float app2 = 0;
	float bsef = 0;
	float bser = 0;

	bool app1Valid = false;
	bool app2Valid = false;
	bool bsefValid = false;
	bool bserValid = false;
};

struct WheelSpeeds {
	float frontLeft 	= 0;
	float frontRight 	= 0;
	float rearLeft 		= 0;
	float rearRight 	= 0;
};

struct Steering {
	float angleDegrees = 0;
};

} // namespace vehicle
