#include "AllocationEngine.h"

AllocationEngine::AllocationEngine() : zones(nullptr), zoneCount(0) {}

AllocationEngine::AllocationEngine(Zone* z, int count) : zones(z), zoneCount(count) {}

bool AllocationEngine::allocateSlot(ParkingRequest* request) {
    if (request->getState() != REQUESTED) {
        return false;
    }