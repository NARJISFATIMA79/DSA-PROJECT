#include "RollbackManager.h"

RollbackManager::RollbackManager() : top(nullptr), size(0) {}

RollbackManager::~RollbackManager() {
    clear();
}
