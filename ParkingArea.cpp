#ifndef PARKINGAREA_H
#define PARKINGAREA_H

#include "ParkingSlot.h"

class ParkingArea {
private:
    int areaID;
    int zoneID;
    ParkingSlot* slots;
    int slotCount;
    int capacity;

public:
};