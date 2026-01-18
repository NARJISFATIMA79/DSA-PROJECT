#include "ParkingArea.h"

ParkingArea::ParkingArea() : areaID(0), zoneID(0), slots(nullptr), slotCount(0), capacity(0) {}

ParkingArea::ParkingArea(int aID, int zID, int cap) 
    : areaID(aID), zoneID(zID), slotCount(0), capacity(cap) {
    slots = new ParkingSlot[capacity];
}