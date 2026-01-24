// ===== Vehicle.cpp =====
#include "Vehicle.h"

Vehicle::Vehicle() : vehicleID(0), preferredZone(0) {}

Vehicle::Vehicle(int vID, int prefZone) : vehicleID(vID), preferredZone(prefZone) {}

int Vehicle::getVehicleID() const {
    return vehicleID;
}

int Vehicle::getPreferredZone() const {
    return preferredZone;
}

