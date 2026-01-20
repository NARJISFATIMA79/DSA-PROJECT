#include "RollbackManager.h"

RollbackManager::RollbackManager() : top(nullptr), size(0) {}

RollbackManager::~RollbackManager() {
    clear();
}

void RollbackManager::pushOperation(int reqIndex, int slotID, int zoneID) {
    RollbackOperation* newOp = new RollbackOperation;
    newOp->requestIndex = reqIndex;
    newOp->slotID = slotID;
    newOp->zoneID = zoneID;
    newOp->next = top;
    top = newOp;
    size++;
}