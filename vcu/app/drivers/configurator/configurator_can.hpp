#pragma once

#include <cstdint>
#include <cstdlib>


namespace drivers::configurator {

// scale
constexpr float TORQUE_MESSAGE_SCALE				= 0.001;
constexpr float THRESHOLD_MESSAGE_SCALE				= 0.0001;
constexpr float PEDAL_MAP_MESSAGE_SCALE				= 0.0001;


// length
constexpr size_t WHEELS_MESSAGE_LENGTH				= 8;
constexpr size_t PEDALS_MESSAGE_LENGTH				= 5;
constexpr size_t IMU_MESSAGE_LENGTH					= 8;
constexpr size_t FLAGS_MESSAGE_LENGTH				= 1;

constexpr size_t CONFIG_MODE_MESSAGE_LENGTH			= 1;
constexpr size_t WRITE_CONFIG_MESSAGE_LENGTH		= 0;
constexpr size_t FLASH_CONFIG_MESSAGE_LENGTH		= 0;

constexpr size_t THRESHOLD_MESSAGE_LENGTH			= 8;
constexpr size_t TORQUE_MESSAGE_LENGTH				= 4;
constexpr size_t PEDAL_MAP_MESSAGE_LENGTH			= 2;
constexpr size_t BSE_ENGAGE_MESSAGE_LENGTH			= 4;


// broadcast
constexpr uint32_t CAN_ID_DREW_WHEELS = 0x12A;
constexpr uint32_t CAN_ID_DREW_BSE    = 0x12B;
constexpr uint32_t CAN_ID_DREW_APP    = 0x12C;
constexpr uint32_t CAN_ID_DREW_FLAGS  = 0x12F;
constexpr uint32_t CAN_ID_DREW_IMU    = 0x12E;


// configuration
constexpr uint32_t CAN_ID_CONFIGURATION_MODE        = 0x300;
constexpr uint32_t CAN_ID_WRITE_CONFIGURATION       = 0x301;
constexpr uint32_t CAN_ID_FLASH_CONFIGURATION       = 0x302;


// configuration command
constexpr uint32_t CAN_ID_CMD_APP1_THRESHOLD        = 0x310;
constexpr uint32_t CAN_ID_CMD_APP2_THRESHOLD        = 0x311;
constexpr uint32_t CAN_ID_CMD_BSEF_THRESHOLD        = 0x312;
constexpr uint32_t CAN_ID_CMD_BSER_THRESHOLD        = 0x313;
constexpr uint32_t CAN_ID_CMD_BSE_ENGAGE			= 0x314;

constexpr uint32_t CAN_ID_CMD_MAX_TORQUE_REQUEST    = 0x320;

constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_BASE        = 0x330;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_1     = 0x330;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_2     = 0x331;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_3     = 0x332;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_4     = 0x333;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_5     = 0x334;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_6     = 0x335;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_7     = 0x336;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_8     = 0x337;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_9     = 0x338;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_10    = 0x339;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_11    = 0x33A;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_12    = 0x33B;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_13    = 0x33C;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_14    = 0x33D;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_15    = 0x33E;
constexpr uint32_t CAN_ID_CMD_PEDAL_MAP_POINT_16    = 0x33F;


// configuration broadcast
constexpr uint32_t CAN_ID_CFG_APP1_THRESHOLD        = 0x410;
constexpr uint32_t CAN_ID_CFG_APP2_THRESHOLD        = 0x411;
constexpr uint32_t CAN_ID_CFG_BSEF_THRESHOLD        = 0x412;
constexpr uint32_t CAN_ID_CFG_BSER_THRESHOLD        = 0x413;
constexpr uint32_t CAN_ID_CFG_BSE_ENGAGE			= 0x414;

constexpr uint32_t CAN_ID_CFG_MAX_TORQUE_REQUEST    = 0x420;

constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_BASE        = 0x430;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_1     = 0x430;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_2     = 0x431;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_3     = 0x432;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_4     = 0x433;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_5     = 0x434;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_6     = 0x435;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_7     = 0x436;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_8     = 0x437;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_9     = 0x438;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_10    = 0x439;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_11    = 0x43A;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_12    = 0x43B;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_13    = 0x43C;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_14    = 0x43D;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_15    = 0x43E;
constexpr uint32_t CAN_ID_CFG_PEDAL_MAP_POINT_16    = 0x43F;

} // namespace drivers::configurator
