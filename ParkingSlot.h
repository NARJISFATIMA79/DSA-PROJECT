#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

class ParkingSlot {
private:
    int slotID;
    int zoneID;
    bool isAvailable;

public:

    ParkingSlot();
    ParkingSlot(int sID, int zID);
    
};