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

    public:
    ParkingSystem(int cityZones);
    ~ParkingSystem();
    
    // Core functionality
    bool setupZone(int zoneID, int numAreas, int slotsPerArea, char* errorMsg = nullptr);
    bool expandCityZones(int additionalZones, char* errorMsg = nullptr);
    int createRequest(int vehicleID, int requestedZone, int requestTime, char* errorMsg = nullptr);
    bool allocateParking(int requestIndex, char* errorMsg = nullptr);
    bool occupyParking(int requestIndex, char* errorMsg = nullptr);
