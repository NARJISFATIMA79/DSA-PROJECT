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

    int getSlotID() const;
    int getZoneID() const;
    bool getAvailability() const;
    
    void setAvailability(bool status);
    
};

#endif