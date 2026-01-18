#include "Zone.h"

Zone::Zone() : zoneID(0), areas(nullptr), areaCount(0), capacity(0) {}

Zone::Zone(int zID, int cap) : zoneID(zID), areaCount(0), capacity(cap) {
    if (cap > 0) {
        areas = new ParkingArea[capacity];
    } else {
        areas = nullptr;
    }
}