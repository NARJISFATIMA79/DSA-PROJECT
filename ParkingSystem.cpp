#include "ParkingSystem.h"
#include <cstring>
#include <cstdio>

ParkingSystem::ParkingSystem(int cityZones) 
    : zoneCount(0), zoneCapacity(cityZones), maxZoneID(cityZones), requestCount(0), requestCapacity(1000) {
    zones = new Zone[zoneCapacity];
    requests = new ParkingRequest[requestCapacity];
    engine = nullptr;
    rollbackMgr = new RollbackManager();
}

ParkingSystem::~ParkingSystem() {
    delete[] zones;
    delete[] requests;
    delete engine;
    delete rollbackMgr;
}

bool ParkingSystem::isValidZoneID(int zoneID) const {
    return zoneID >= 1 && zoneID <= maxZoneID;
}

void ParkingSystem::resizeZoneArray(int newCapacity) {
    Zone* newZones = new Zone[newCapacity];
    for (int i = 0; i < zoneCount; i++) {
        newZones[i] = zones[i];
    }
    delete[] zones;
    zones = newZones;
    zoneCapacity = newCapacity;
}

bool ParkingSystem::setupZone(int zoneID, int numAreas, int slotsPerArea, char* errorMsg) {
    // Validate zone ID is within city range
    if (!isValidZoneID(zoneID)) {
        if (errorMsg) {
            sprintf(errorMsg, "Invalid Zone ID! Must be between 1 and %d", maxZoneID);
        }
        return false;
    }

    // Check if zone already exists
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i].getZoneID() == zoneID) {
            if (errorMsg) strcpy(errorMsg, "Zone already exists!");
            return false;
        }
    }
    
    // Validate parameters
    if (numAreas <= 0 || slotsPerArea <= 0) {
        if (errorMsg) strcpy(errorMsg, "Areas and slots must be positive!");
        return false;
    }
    