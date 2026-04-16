#pragma once

#include "generics/snapshot.hpp"
#include <stdint.h>
#include <cstddef>


namespace vehicle::inverter {

struct Temperature1 {
	float moduleA;
	float moduleB;
	float moduleC;
	float gateDriverBoard;
};

struct Temperature2 {
	float controlBoard;
	float rtd1;
	float rtd2;
	float rtd3;
};

struct Temperature3 {
	float coolant;
	float hotSpot;
	float motor;
	float torqueShudder;
};

struct MotorPosition {
	float motorAngle;
	float motorSpeed;
	float electricalOutputFrequency;
	float deltaResolverFiltered;
};

struct TorqueInformation {
	float commandedTorque;
	float torqueFeedback;
};

struct TorqueCapability {
	float torqueCapability;
};

struct PostFaults {						// must match CAN message exactly
    bool hardwareGateDesaturation		: 1;
    bool hardwareOvercurrent			: 1;
    bool acceleratorShorted				: 1;
    bool acceleratorOpen				: 1;
    bool currentSensorLow				: 1;
    bool currentSensorHigh				: 1;
    bool moduleTemperatureLow			: 1;
    bool moduleTemperatureHigh			: 1;
    bool controlPcbTemperatureLow		: 1;
    bool controlPcbTemperatureHigh		: 1;
    bool gateDrivePcbTemperatureLow		: 1;
    bool gateDrivePcbTemperatureHigh	: 1;
    bool sense5vLow						: 1;
    bool sense5vHigh					: 1;
    bool sense12vLow					: 1;
    bool sense12vHigh					: 1;
    bool sense2p5vLow					: 1;
    bool sense2p5vHigh					: 1;
    bool sense1p5vLow					: 1;
    bool sense1p5vHigh					: 1;
    bool dcBusVoltageHigh				: 1;
    bool dcBusVoltageLow				: 1;
    bool prechargeTimeout				: 1;
    bool prechargeVoltageFailure		: 1;
    bool eepromChecksumInvalid			: 1;
    bool eepromDataOutOfRange			: 1;
    bool eepromUpdateRequired			: 1;
    bool hardwareDcBusOvervoltage		: 1;
    bool reservedBit28					: 1;
    bool reservedBit29					: 1;
    bool brakeShorted					: 1;
    bool brakeOpen						: 1;
};

struct RunFaults {						// must match CAN message exactly
    bool motorOverspeed					: 1;
    bool overcurrent					: 1;
    bool overvoltage					: 1;
    bool inverterOvertemperature		: 1;
    bool acceleratorInputShorted		: 1;
    bool acceleratorInputOpen			: 1;
    bool directionCommand				: 1;
    bool inverterResponseTimeout		: 1;
    bool hardwareGateDesaturation		: 1;
    bool hardwareOvercurrent			: 1;
    bool undervoltage					: 1;
    bool canCommandMessageLost			: 1;
    bool motorOvertemperature			: 1;
    bool reservedBit45					: 1;
    bool reservedBit46					: 1;
    bool reservedBit47					: 1;
    bool brakeInputShorted				: 1;
    bool brakeInputOpen					: 1;
    bool moduleAOvertemperature			: 1;
    bool moduleBOvertemperature			: 1;
    bool moduleCOvertemperature			: 1;
    bool pcbOvertemperature				: 1;
    bool gateDriveBoard1Overtemperature	: 1;
    bool gateDriveBoard2Overtemperature	: 1;
    bool gateDriveBoard3Overtemperature	: 1;
    bool currentSensor					: 1;
    bool reservedBit58					: 1;
    bool reservedBit59					: 1;
    bool reservedBit60					: 1;
    bool reservedBit61					: 1;
    bool resolverNotConnected			: 1;
    bool reservedBit63					: 1;
};

struct FaultFlags {
	PostFaults postFaults;
	RunFaults runFaults;
};

struct InternalStates {
	bool inverterEnableState			: 1;
	bool inverterEnableLockout			: 1;
	bool bmsActive						: 1;
	bool bmsLimitingTorque				: 1;
};

} // namespace vehicle::inverter
