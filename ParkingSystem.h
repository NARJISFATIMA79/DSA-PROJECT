#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"

class ParkingSystem {
private:
    Zone* zones;
    int zoneCount;
    int zoneCapacity;
    int maxZoneID;  // Maximum allowed zone ID (based on city zones)
    
    ParkingRequest* requests;
    int requestCount;
    int requestCapacity;
    
    AllocationEngine* engine;
    RollbackManager* rollbackMgr;

    void updateAllocationEngine();
    bool isValidZoneID(int zoneID) const;
    void resizeZoneArray(int newCapacity);