// ===== RollbackManager.h =====
#ifndef ROLLBACKMANAGER_H
#define ROLLBACKMANAGER_H

#include "ParkingRequest.h"

struct RollbackOperation {
    int requestIndex;
    int slotID;
    int zoneID;
    RollbackOperation* next;
};

class RollbackManager {
private:
    RollbackOperation* top;
    int size;

public:
    RollbackManager();
    ~RollbackManager();
    
    void pushOperation(int reqIndex, int slotID, int zoneID);
    bool popOperation(int& reqIndex, int& slotID, int& zoneID);
    int getSize() const;
    void clear();
};

#endif