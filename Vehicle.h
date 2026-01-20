// ===== Vehicle.h =====
#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle {
private:
    int vehicleID;
    int preferredZone;

public:
    Vehicle();
    Vehicle(int vID, int prefZone);
    
    int getVehicleID() const;
    int getPreferredZone() const;
};

#endif
