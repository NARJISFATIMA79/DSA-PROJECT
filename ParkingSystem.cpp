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