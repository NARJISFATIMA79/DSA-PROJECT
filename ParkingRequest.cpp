#include "ParkingRequest.h"

ParkingRequest::ParkingRequest() 
    : vehicleID(0), requestedZone(0), requestTime(0), state(REQUESTED), 
      allocatedSlotID(-1), allocatedZoneID(-1), releaseTime(0), crossZoneAllocation(false) {}
