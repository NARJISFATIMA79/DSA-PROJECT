#include "ParkingRequest.h"

ParkingRequest::ParkingRequest() 
    : vehicleID(0), requestedZone(0), requestTime(0), state(REQUESTED), 
      allocatedSlotID(-1), allocatedZoneID(-1), releaseTime(0), crossZoneAllocation(false) {}
ParkingRequest::ParkingRequest(int vID, int reqZone, int reqTime)
    : vehicleID(vID), requestedZone(reqZone), requestTime(reqTime), state(REQUESTED),
      allocatedSlotID(-1), allocatedZoneID(-1), releaseTime(0), crossZoneAllocation(false) {}

int ParkingRequest::getVehicleID() const {
    return vehicleID;
}

int ParkingRequest::getRequestedZone() const {
    return requestedZone;
}

int ParkingRequest::getRequestTime() const {
    return requestTime;
}

RequestState ParkingRequest::getState() const {
    return state;
}

int ParkingRequest::getAllocatedSlotID() const {
    return allocatedSlotID;
}

int ParkingRequest::getAllocatedZoneID() const {
    return allocatedZoneID;
}

int ParkingRequest::getReleaseTime() const {
    return releaseTime;
}

bool ParkingRequest::isCrossZone() const {
    return crossZoneAllocation;
}

bool ParkingRequest::setState(RequestState newState) {
    if (state == REQUESTED && (newState == ALLOCATED || newState == CANCELLED)) {
        state = newState;
        return true;
    }
    if (state == ALLOCATED && (newState == OCCUPIED || newState == CANCELLED)) {
        state = newState;
        return true;
    }
    if (state == OCCUPIED && newState == RELEASED) {
        state = newState;
        return true;
    }
    return false;
}