// ===== RollbackManager.cpp =====
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

bool RollbackManager::popOperation(int& reqIndex, int& slotID, int& zoneID) {
    if (top == nullptr) {
        return false;
    }
    
    RollbackOperation* temp = top;
    reqIndex = temp->requestIndex;
    slotID = temp->slotID;
    zoneID = temp->zoneID;
    top = top->next;
    delete temp;
    size--;
    return true;
}

int RollbackManager::getSize() const {
    return size;
}

void RollbackManager::clear() {
    while (top != nullptr) {
        RollbackOperation* temp = top;
        top = top->next;
        delete temp;
    }
    size = 0;
}