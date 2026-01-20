// Zone.h 
#ifndef ZONE_H
#define ZONE_H

#include "ParkingArea.h"

class Zone {
private:
    int zoneID;
    ParkingArea* areas;
    int areaCount;
    int capacity;

public:
Zone();
    Zone(int zID, int cap);
    ~Zone();
    
    Zone(const Zone& other);
    Zone& operator=(const Zone& other);
    
    int getZoneID() const;
};