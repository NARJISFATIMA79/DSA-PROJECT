#include "AllocationEngine.h"

AllocationEngine::AllocationEngine() : zones(nullptr), zoneCount(0) {}

AllocationEngine::AllocationEngine(Zone* z, int count) : zones(z), zoneCount(count) {}

bool AllocationEngine::allocateSlot(ParkingRequest* request) {
    if (request->getState() != REQUESTED) {
        return false;
    }

     int requestedZone = request->getRequestedZone();
    
    // First, try to allocate in the requested zone
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i].getZoneID() == requestedZone) {
            ParkingSlot* slot = zones[i].findAvailableSlot();
            if (slot != nullptr) {
                slot->setAvailability(false);
                // Use the zone's actual zone ID, not the slot's stored zone ID
                int actualZoneID = zones[i].getZoneID();
                request->setAllocatedSlot(slot->getSlotID(), actualZoneID, false);
                request->setState(ALLOCATED);
                return true;
            }
            break; // Zone found but no slots available
        }
    }