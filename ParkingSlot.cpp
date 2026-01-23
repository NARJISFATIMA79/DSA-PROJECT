#include "ParkingSlot.h"

ParkingSlot::ParkingSlot() : slotID(0), zoneID(0), isAvailable(true) {}

ParkingSlot::ParkingSlot(int sID, int zID) : slotID(sID), zoneID(zID), isAvailable(true) {}

int ParkingSlot::getSlotID() const {
    return slotID;
}

int ParkingSlot::getZoneID() const {
    return zoneID;
}

bool ParkingSlot::getAvailability() const {
    return isAvailable;
}

void ParkingSlot::setAvailability(bool status) {
    isAvailable = status;
}