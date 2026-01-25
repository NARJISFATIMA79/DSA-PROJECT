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

    // Check capacity and resize if needed
    if (zoneCount >= zoneCapacity) {
        resizeZoneArray(zoneCapacity * 2);
    }
    
    zones[zoneCount] = Zone(zoneID, numAreas);
    
    // Add areas first
    for (int i = 0; i < numAreas; i++) {
        zones[zoneCount].addArea(i, slotsPerArea);
    }
    
    // Generate simple sequential slot IDs: 1, 2, 3, ... within each zone
    int slotCounter = 1;
    for (int areaIdx = 0; areaIdx < numAreas; areaIdx++) {
        for (int slotIdx = 0; slotIdx < slotsPerArea; slotIdx++) {
            zones[zoneCount].addSlotToArea(areaIdx, slotCounter);
            slotCounter++;
        }
    }
    
    zoneCount++;
    updateAllocationEngine();
    return true;
}

bool ParkingSystem::expandCityZones(int additionalZones, char* errorMsg) {
    if (additionalZones <= 0) {
        if (errorMsg) strcpy(errorMsg, "Additional zones must be positive!");
        return false;
    }
    
    maxZoneID += additionalZones;
    
    // Resize zone array if needed
    if (maxZoneID > zoneCapacity) {
        resizeZoneArray(maxZoneID);
    }
    
    return true;
}

void ParkingSystem::updateAllocationEngine() {
    if (engine != nullptr) {
        delete engine;
    }
    engine = new AllocationEngine(zones, zoneCount);
}

int ParkingSystem::createRequest(int vehicleID, int requestedZone, int requestTime, char* errorMsg) {
    if (requestCount >= requestCapacity) {
        if (errorMsg) strcpy(errorMsg, "Request system full!");
        return -1;
    }
    
    // Validate that the requested zone is valid
    if (!isValidZoneID(requestedZone)) {
        if (errorMsg) {
            sprintf(errorMsg, "Invalid zone! Valid zones: 1-%d", maxZoneID);
        }
        return -1;
    }
    
    // Check if zone is actually active (set up)
    bool zoneActive = false;
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i].getZoneID() == requestedZone) {
            zoneActive = true;
            break;
        }
    }
    
    if (!zoneActive) {
        if (errorMsg) strcpy(errorMsg, "Zone not set up yet! Please setup zone first.");
        return -1;
    }
    
    if (requestTime < 0) {
        if (errorMsg) strcpy(errorMsg, "Request time must be non-negative!");
        return -1;
    }
    
    requests[requestCount] = ParkingRequest(vehicleID, requestedZone, requestTime);
    requestCount++;
    return requestCount - 1;
}

bool ParkingSystem::allocateParking(int requestIndex, char* errorMsg) {
    if (requestIndex < 0 || requestIndex >= requestCount) {
        if (errorMsg) strcpy(errorMsg, "Invalid request index!");
        return false;
    }
    
    if (engine == nullptr) {
        if (errorMsg) strcpy(errorMsg, "No zones set up!");
        return false;
    }
    
    if (requests[requestIndex].getState() != REQUESTED) {
        if (errorMsg) {
            strcpy(errorMsg, "Request not in REQUESTED state! Current: ");
            RequestState state = requests[requestIndex].getState();
            if (state == ALLOCATED) strcat(errorMsg, "ALLOCATED");
            else if (state == OCCUPIED) strcat(errorMsg, "OCCUPIED");
            else if (state == RELEASED) strcat(errorMsg, "RELEASED");
            else if (state == CANCELLED) strcat(errorMsg, "CANCELLED");
        }
        return false;
    }
    
    bool success = engine->allocateSlot(&requests[requestIndex]);
    if (success) {
        int slotID = requests[requestIndex].getAllocatedSlotID();
        int allocatedZone = requests[requestIndex].getAllocatedZoneID();
        rollbackMgr->pushOperation(requestIndex, slotID, allocatedZone);
    } else {
        if (errorMsg) strcpy(errorMsg, "No available slots in any zone!");
    }
    return success;
}

bool ParkingSystem::occupyParking(int requestIndex, char* errorMsg) {
    if (requestIndex < 0 || requestIndex >= requestCount) {
        if (errorMsg) strcpy(errorMsg, "Invalid request index!");
        return false;
    }
    
    if (requests[requestIndex].getState() != ALLOCATED) {
        if (errorMsg) strcpy(errorMsg, "Request must be ALLOCATED first!");
        return false;
    }
    
    return requests[requestIndex].setState(OCCUPIED);
}

bool ParkingSystem::releaseParking(int requestIndex, int releaseTime, char* errorMsg) {
    if (requestIndex < 0 || requestIndex >= requestCount) {
        if (errorMsg) strcpy(errorMsg, "Invalid request index!");
        return false;
    }
    
    if (requests[requestIndex].getState() != OCCUPIED) {
        if (errorMsg) strcpy(errorMsg, "Parking must be OCCUPIED to release!");
        return false;
    }
    
    if (releaseTime <= requests[requestIndex].getRequestTime()) {
        if (errorMsg) strcpy(errorMsg, "Release time must be after request time!");
        return false;
    }
    
    if (requests[requestIndex].setState(RELEASED)) {
        requests[requestIndex].setReleaseTime(releaseTime);
        
        int slotID = requests[requestIndex].getAllocatedSlotID();
        int zoneID = requests[requestIndex].getAllocatedZoneID();
        
        for (int i = 0; i < zoneCount; i++) {
            if (zones[i].getZoneID() == zoneID) {
                ParkingSlot* slot = zones[i].getSlotByID(slotID);
                if (slot != nullptr) {
                    slot->setAvailability(true);
                }
                break;
            }
        }
        return true;
    }
    return false;
}

bool ParkingSystem::cancelRequest(int requestIndex, char* errorMsg) {
    if (requestIndex < 0 || requestIndex >= requestCount) {
        if (errorMsg) strcpy(errorMsg, "Invalid request index!");
        return false;
    }
    
    RequestState currentState = requests[requestIndex].getState();
    
    if (currentState != REQUESTED && currentState != ALLOCATED) {
        if (errorMsg) strcpy(errorMsg, "Can only cancel REQUESTED or ALLOCATED requests!");
        return false;
    }
    
    if (currentState == ALLOCATED) {
        int slotID = requests[requestIndex].getAllocatedSlotID();
        int zoneID = requests[requestIndex].getAllocatedZoneID();
        
        for (int i = 0; i < zoneCount; i++) {
            if (zones[i].getZoneID() == zoneID) {
                ParkingSlot* slot = zones[i].getSlotByID(slotID);
                if (slot != nullptr) {
                    slot->setAvailability(true);
                }
                break;
            }
        }
    }
    
    requests[requestIndex].setState(CANCELLED);
    return true;
}

bool ParkingSystem::rollbackLastKAllocations(int k, char* errorMsg) {
    if (k <= 0) {
        if (errorMsg) strcpy(errorMsg, "k must be positive!");
        return false;
    }
    
    if (rollbackMgr->getSize() == 0) {
        if (errorMsg) strcpy(errorMsg, "No allocations to rollback!");
        return false;
    }
    
    int rolled = 0;
    while (rolled < k && rollbackMgr->getSize() > 0) {
        int reqIndex, slotID, zoneID;
        if (rollbackMgr->popOperation(reqIndex, slotID, zoneID)) {
            if (reqIndex >= 0 && reqIndex < requestCount) {
                RequestState state = requests[reqIndex].getState();
                
                if (state == ALLOCATED || state == OCCUPIED) {
                    for (int i = 0; i < zoneCount; i++) {
                        if (zones[i].getZoneID() == zoneID) {
                            ParkingSlot* slot = zones[i].getSlotByID(slotID);
                            if (slot != nullptr) {
                                slot->setAvailability(true);
                            }
                            break;
                        }
                    }
                    
                    requests[reqIndex].forceCancel();
                    rolled++;
                }
            }
        }
    }
    return rolled > 0;
}

double ParkingSystem::getAverageParkingDuration() const {
    int totalDuration = 0;
    int completedCount = 0;
    
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].getState() == RELEASED) {
            int duration = requests[i].getReleaseTime() - requests[i].getRequestTime();
            totalDuration += duration;
            completedCount++;
        }
    }
    
    if (completedCount == 0) {
        return 0.0;
    }
    return (double)totalDuration / completedCount;
}

double ParkingSystem::getZoneUtilization(int zoneID) const {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i].getZoneID() == zoneID) {
            int total = zones[i].getTotalSlots();
            int available = zones[i].getAvailableSlots();
            if (total == 0) {
                return 0.0;
            }
            return (double)(total - available) / total * 100.0;
        }
    }
    return 0.0;
}