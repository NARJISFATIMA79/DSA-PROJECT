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
    ParkingArea();

     ParkingArea(int aID, int zID, int cap);
    ~ParkingArea();
    
    ParkingArea(const ParkingArea& other);
    ParkingArea& operator=(const ParkingArea& other);
    
    void addSlot(int slotID);
    ParkingSlot* findAvailableSlot();
    ParkingSlot* getSlotByID(int slotID);
    int getSlotCount() const;
    int getAvailableCount() const;
};

#endif