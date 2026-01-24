#ifndef PARKINGREQUEST_H
#define PARKINGREQUEST_H

enum RequestState {
    REQUESTED,
    ALLOCATED,
    OCCUPIED,
    RELEASED,
    CANCELLED
};

class ParkingRequest {
private:
    int vehicleID;
    int requestedZone;
    int requestTime;
    RequestState state;
    int allocatedSlotID;
    int allocatedZoneID;
    int releaseTime;
    bool crossZoneAllocation;

public:
    ParkingRequest();
    ParkingRequest(int vID, int reqZone, int reqTime);

    int getVehicleID() const;
    int getRequestedZone() const;
    int getRequestTime() const;
    RequestState getState() const;
    int getAllocatedSlotID() const;
    int getAllocatedZoneID() const;
    int getReleaseTime() const;
    bool isCrossZone() const;