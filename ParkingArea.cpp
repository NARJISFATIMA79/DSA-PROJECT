#include "ParkingArea.h"

ParkingArea::ParkingArea() : areaID(0), zoneID(0), slots(nullptr), slotCount(0), capacity(0) {}

ParkingArea::ParkingArea(int aID, int zID, int cap) 
    : areaID(aID), zoneID(zID), slotCount(0), capacity(cap) {
    slots = new ParkingSlot[capacity];
}

ParkingArea::~ParkingArea() {
    delete[] slots;
}

ParkingArea::ParkingArea(const ParkingArea& other) 
    : areaID(other.areaID), zoneID(other.zoneID), slotCount(other.slotCount), capacity(other.capacity) {
    if (other.slots != nullptr && capacity > 0) {
        slots = new ParkingSlot[capacity];
        for (int i = 0; i < slotCount; i++) {
            slots[i] = other.slots[i];
        }
    } else {
        slots = nullptr;
    }
}

ParkingArea& ParkingArea::operator=(const ParkingArea& other) {
    if (this != &other) {
        delete[] slots;
        
        areaID = other.areaID;
        zoneID = other.zoneID;
        slotCount = other.slotCount;
        capacity = other.capacity;
        
        if (other.slots != nullptr && capacity > 0) {
            slots = new ParkingSlot[capacity];
            for (int i = 0; i < slotCount; i++) {
                slots[i] = other.slots[i];
            }
        } else {
            slots = nullptr;
        }
    }
    return *this;
}

void ParkingArea::addSlot(int slotID) {
    if (slotCount < capacity) {
        slots[slotCount] = ParkingSlot(slotID, zoneID);
        slotCount++;
    }
}

ParkingSlot* ParkingArea::findAvailableSlot() {
    for (int i = 0; i < slotCount; i++) {
        if (slots[i].getAvailability()) {
            return &slots[i];
        }
    }
    return nullptr;
}

ParkingSlot* ParkingArea::getSlotByID(int slotID) {
    for (int i = 0; i < slotCount; i++) {
        if (slots[i].getSlotID() == slotID) {
            return &slots[i];
        }
    }
    return nullptr;
}

int ParkingArea::getSlotCount() const {
    return slotCount;
}

int ParkingArea::getAvailableCount() const {
    int count = 0;
    for (int i = 0; i < slotCount; i++) {
        if (slots[i].getAvailability()) {
            count++;
        }
    }
    return count;
}