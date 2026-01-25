# Parking Allocation System — Design Documentation

## 1. Zone and Slot Representation

The system models parking infrastructure using a strict hierarchical structure:

**Zone → ParkingArea → ParkingSlot**

* **Zone** represents a logical or physical region (e.g., Zone A, Zone B).  
* Each Zone contains multiple **ParkingArea** objects.  
* Each ParkingArea contains multiple **ParkingSlot** objects.  

This design mirrors real-world layouts (city → building → spot) and keeps responsibilities clean:

* Zone handles aggregation and high-level availability.  
* ParkingArea manages slot grouping.  
* ParkingSlot represents the atomic resource.  

Dynamic arrays are used to store areas and slots, with explicit copy constructors and assignment operators to ensure safe deep copying and ownership management.

---

## 2. Allocation Strategy

The allocation strategy follows a **deterministic first-fit approach**:

1. Search the requested zone for an available slot.  
2. If found, allocate immediately.  
3. If not found, search other zones sequentially (cross-zone allocation).  

This strategy prioritizes:

* Predictability over optimization  
* Simplicity over heuristics  

No randomization or load balancing is applied. The same request sequence always produces the same allocation result.

### Cross-Zone Allocation

When allocation occurs outside the requested zone:

* The request is marked with a `crossZoneAllocation` flag.  
* The allocated zone ID is stored separately from the requested zone.  

> **Note:** The system is architected to support cross-zone penalties, but no numerical cost or pricing logic is currently applied. This is treated as a documented extension point rather than an implemented feature.

---

## 3. Request Lifecycle State Machine

Each parking request follows a strictly enforced finite state machine:


Allowed transitions:

* `REQUESTED → ALLOCATED | CANCELLED`  
* `ALLOCATED → OCCUPIED | CANCELLED`  
* `OCCUPIED → RELEASED`  

All other transitions are explicitly rejected.

This guarantees:

* No invalid partial states  
* No illegal rollbacks  
* Safe cancellation handling  

The state machine acts like a turnstile: forward-only unless explicitly cancelled.

---

## 4. Rollback Design

Rollback is implemented using a **stack-based undo mechanism** (`RollbackManager`).

### Design Rationale

* Every successful allocation operation is pushed onto a rollback stack.  
* Each stack entry records:
  * Request index  
  * Slot ID  
  * Zone ID  

### Rollback Process

* On cancellation or failure, operations are popped in reverse order.  
* Slots are restored to available state.  
* Request state is reverted safely.  

This design supports:

* Cancellation rollback  
* Partial failure recovery  
* Consistent system state restoration  

The rollback manager is cleared after successful completion, preventing stale operations.

---

## 5. Time and Space Complexity

### Allocation

* **Time Complexity:**  
  * Worst case: `O(Z × A × S)`  
  * Where Z = zones, A = areas per zone, S = slots per area  
* **Space Complexity:**  
  * `O(1)` extra space per allocation (excluding rollback stack)  

### Rollback

* **Time Complexity:** `O(k)` where k = number of operations rolled back  
* **Space Complexity:** `O(k)` for rollback stack  

### State Transitions

* **Time Complexity:** `O(1)`  
* **Space Complexity:** `O(1)`  
