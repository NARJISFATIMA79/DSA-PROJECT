#ifndef ALLOCATIONENGINE_H
#define ALLOCATIONENGINE_H


#include "ParkingRequest.h"

class AllocationEngine {
private:
    Zone* zones;
    int zoneCount;

    public:
    AllocationEngine();
    AllocationEngine(Zone* z, int count);
    
    bool allocateSlot(ParkingRequest* request);
};

#endif
