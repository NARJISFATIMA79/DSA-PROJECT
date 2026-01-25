// ===== Zone.cpp =====
#include "Zone.h"

Zone::Zone() : zoneID(0), areas(nullptr), areaCount(0), capacity(0) {}

Zone::Zone(int zID, int cap) : zoneID(zID), areaCount(0), capacity(cap) {
    if (cap > 0) {
        areas = new ParkingArea[capacity];
    } else {
        areas = nullptr;
    }
}

Zone::~Zone() {
    delete[] areas;
}

Zone::Zone(const Zone& other) 
    : zoneID(other.zoneID), areaCount(other.areaCount), capacity(other.capacity) {
    if (other.areas != nullptr && capacity > 0) {
        areas = new ParkingArea[capacity];
        for (int i = 0; i < areaCount; i++) {
            areas[i] = other.areas[i];
        }
    } else {
        areas = nullptr;
    }
}

Zone& Zone::operator=(const Zone& other) {
    if (this != &other) {
        delete[] areas;
        
        zoneID = other.zoneID;
        areaCount = other.areaCount;
        capacity = other.capacity;
        
        if (other.areas != nullptr && capacity > 0) {
            areas = new ParkingArea[capacity];
            for (int i = 0; i < areaCount; i++) {
                areas[i] = other.areas[i];
            }
        } else {
            areas = nullptr;
        }
    }
    return *this;
}

int Zone::getZoneID() const {
    return zoneID;
}

void Zone::addArea(int areaID, int slotCapacity) {
    if (areaCount < capacity) {
        areas[areaCount] = ParkingArea(areaID, zoneID, slotCapacity);
        areaCount++;
    }
}

void Zone::addSlotToArea(int areaIndex, int slotID) {
    if (areaIndex >= 0 && areaIndex < areaCount) {
        areas[areaIndex].addSlot(slotID);
    }
}

ParkingSlot* Zone::findAvailableSlot() {
    for (int i = 0; i < areaCount; i++) {
        ParkingSlot* slot = areas[i].findAvailableSlot();
        if (slot != nullptr) {
            return slot;
        }
    }
    return nullptr;
}

ParkingSlot* Zone::getSlotByID(int slotID) {
    for (int i = 0; i < areaCount; i++) {
        ParkingSlot* slot = areas[i].getSlotByID(slotID);
        if (slot != nullptr) {
            return slot;
        }
    }
    return nullptr;
}

int Zone::getTotalSlots() const {
    int total = 0;
    for (int i = 0; i < areaCount; i++) {
        total += areas[i].getSlotCount();
    }
    return total;
}

int Zone::getAvailableSlots() const {
    int available = 0;
    for (int i = 0; i < areaCount; i++) {
        available += areas[i].getAvailableCount();
    }
    return available;
}
