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