#include "vehicle/vehicle_configuration.hpp"


namespace vehicle {

VehicleConfiguration vehicleConfiguration{};


//getters
float VehicleConfiguration::getApps1LoThresholdVoltage() const { return apps1LoThresholdVoltage; }
float VehicleConfiguration::getApps1HiThresholdVoltage() const { return apps1HiThresholdVoltage; }
float VehicleConfiguration::getApps2LoThresholdVoltage() const { return apps2LoThresholdVoltage; }
float VehicleConfiguration::getApps2HiThresholdVoltage() const { return apps2HiThresholdVoltage; }
float VehicleConfiguration::getBsefLoThresholdVoltage() const { return bsefLoThresholdVoltage; }
float VehicleConfiguration::getBsefHiThresholdVoltage() const { return bsefHiThresholdVoltage; }
float VehicleConfiguration::getBserLoThresholdVoltage() const { return bserLoThresholdVoltage; }
float VehicleConfiguration::getBserHiThresholdVoltage() const { return bserHiThresholdVoltage; }
float VehicleConfiguration::getBsefBrakeEngagedThreshold() const { return bsefBrakeEngagedThreshold; }
float VehicleConfiguration::getBserBrakeEngagedThreshold() const { return bserBrakeEngagedThreshold; }

// Setters
void VehicleConfiguration::setApps1LoThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    apps1LoThresholdVoltage = value;
}

void VehicleConfiguration::setApps1HiThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    apps1HiThresholdVoltage = value;
}

void VehicleConfiguration::setApps2LoThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    apps2LoThresholdVoltage = value;
}

void VehicleConfiguration::setApps2HiThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    apps2HiThresholdVoltage = value;
}

void VehicleConfiguration::setBsefLoThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    bsefLoThresholdVoltage = value;
}

void VehicleConfiguration::setBsefHiThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    bsefHiThresholdVoltage = value;
}

void VehicleConfiguration::setBserLoThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    bserLoThresholdVoltage = value;
}

void VehicleConfiguration::setBserHiThresholdVoltage(float value) {
    if (value < MIN_THRESHOLD_VOLTAGE || value > MAX_THRESHOLD_VOLTAGE) {
        return;
    }
    bserHiThresholdVoltage = value;
}

void VehicleConfiguration::setBsefBrakeEngagedThreshold(float value) {
    if (value < MIN_BRAKE_ENGAGED_THRESHOLD || value > MAX_BRAKE_ENGAGED_THRESHOLD) {
        return;
    }
    bsefBrakeEngagedThreshold = value;
}

void VehicleConfiguration::setBserBrakeEngagedThreshold(float value) {
    if (value < MIN_BRAKE_ENGAGED_THRESHOLD || value > MAX_BRAKE_ENGAGED_THRESHOLD) {
        return;
    }
    bserBrakeEngagedThreshold = value;
}

} // namespace vehicle
