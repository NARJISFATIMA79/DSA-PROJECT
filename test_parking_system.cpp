// ===== test_parking_system.cpp =====

//.\test_parking.exe 
//g++ -o test_parking.exe test_parking_system.cpp ParkingSlot.cpp ParkingArea.cpp Zone.cpp Vehicle.cpp ParkingRequest.cpp AllocationEngine.cpp RollbackManager.cpp ParkingSystem.cpp (Run this command)

#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "ParkingSystem.h"
#include "ParkingRequest.h"

using namespace std;

// Test result tracking
int totalTests = 0;
int passedTests = 0;
int failedTests = 0;

// Color codes for output
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

void printTestHeader(const string& testName) {
    cout << "\n" << CYAN << "========================================" << RESET << endl;
    cout << CYAN << "TEST: " << testName << RESET << endl;
    cout << CYAN << "========================================" << RESET << endl;
}

void assertTest(bool condition, const string& testDescription) {
    totalTests++;
    if (condition) {
        passedTests++;
        cout << GREEN << "[PASS] " << RESET << testDescription << endl;
    } else {
        failedTests++;
        cout << RED << "[FAIL] " << RESET << testDescription << endl;
    }
}

void printTestSummary() {
    cout << "\n" << YELLOW << "========================================" << RESET << endl;
    cout << YELLOW << "TEST SUMMARY" << RESET << endl;
    cout << YELLOW << "========================================" << RESET << endl;
    cout << "Total Tests: " << totalTests << endl;
    cout << GREEN << "Passed: " << passedTests << RESET << endl;
    cout << RED << "Failed: " << failedTests << RESET << endl;
    cout << "Success Rate: " << fixed << setprecision(2) 
         << (totalTests > 0 ? (double)passedTests / totalTests * 100 : 0) << "%" << endl;
    cout << YELLOW << "========================================" << RESET << endl;
}

// ============================================================================
// TEST 1: Slot Allocation Correctness
// ============================================================================
void test_slot_allocation_correctness() {
    printTestHeader("1. Slot Allocation Correctness");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    // Setup zone with known slots
    bool setupSuccess = system.setupZone(1, 2, 3, errorMsg); // 2 areas, 3 slots per area = 6 total slots
    assertTest(setupSuccess, "Zone 1 setup with 2 areas and 3 slots per area");
    
    Zone* zone = system.getZone(1);
    assertTest(zone != nullptr, "Zone 1 exists after setup");
    assertTest(zone->getTotalSlots() == 6, "Zone 1 has exactly 6 slots");
    assertTest(zone->getAvailableSlots() == 6, "All 6 slots are initially available");
    
    // Create and allocate first request
    int req1 = system.createRequest(101, 1, 0, errorMsg);
    assertTest(req1 == 0, "First request created with index 0");
    
    bool allocSuccess = system.allocateParking(req1, errorMsg);
    assertTest(allocSuccess, "First parking allocation successful");
    
    ParkingRequest* request1 = system.getRequest(req1);
    assertTest(request1 != nullptr, "Request 1 retrieved successfully");
    assertTest(request1->getState() == ALLOCATED, "Request 1 state is ALLOCATED");
    assertTest(request1->getAllocatedSlotID() >= 1 && request1->getAllocatedSlotID() <= 6, 
               "Allocated slot ID is within valid range (1-6)");
    assertTest(request1->getAllocatedZoneID() == 1, "Allocated to requested zone 1");
    assertTest(!request1->isCrossZone(), "Allocation is not cross-zone");
    assertTest(zone->getAvailableSlots() == 5, "Available slots decreased to 5 after allocation");
    
    // Allocate multiple requests to verify sequential allocation
    int req2 = system.createRequest(102, 1, 1, errorMsg);
    system.allocateParking(req2, errorMsg);
    
    int req3 = system.createRequest(103, 1, 2, errorMsg);
    system.allocateParking(req3, errorMsg);
    
    assertTest(zone->getAvailableSlots() == 3, "Available slots is 3 after 3 allocations");
    
    ParkingRequest* request2 = system.getRequest(req2);
    ParkingRequest* request3 = system.getRequest(req3);
    
    // Verify different slots were allocated
    int slot1 = request1->getAllocatedSlotID();
    int slot2 = request2->getAllocatedSlotID();
    int slot3 = request3->getAllocatedSlotID();
    
    assertTest(slot1 != slot2 && slot1 != slot3 && slot2 != slot3, 
               "Different slots allocated to different requests");
    
    cout << "  -> Slot IDs allocated: " << slot1 << ", " << slot2 << ", " << slot3 << endl;
}

// ============================================================================
// TEST 2: Cross-Zone Allocation Handling
// ============================================================================
void test_cross_zone_allocation() {
    printTestHeader("2. Cross-Zone Allocation Handling");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    // Setup Zone 1 with limited capacity
    system.setupZone(1, 1, 2, errorMsg); // Only 2 slots
    
    // Setup Zone 2 with more capacity
    system.setupZone(2, 1, 3, errorMsg); // 3 slots
    
    Zone* zone1 = system.getZone(1);
    Zone* zone2 = system.getZone(2);
    
    assertTest(zone1->getTotalSlots() == 2, "Zone 1 has 2 slots");
    assertTest(zone2->getTotalSlots() == 3, "Zone 2 has 3 slots");
    
    // Fill up Zone 1
    int req1 = system.createRequest(201, 1, 0, errorMsg);
    system.allocateParking(req1, errorMsg);
    
    int req2 = system.createRequest(202, 1, 1, errorMsg);
    system.allocateParking(req2, errorMsg);
    
    assertTest(zone1->getAvailableSlots() == 0, "Zone 1 is now full (0 available slots)");
    
    // Try to allocate to Zone 1 (should trigger cross-zone allocation)
    int req3 = system.createRequest(203, 1, 2, errorMsg);
    bool crossZoneSuccess = system.allocateParking(req3, errorMsg);
    
    assertTest(crossZoneSuccess, "Cross-zone allocation succeeded when requested zone is full");
    
    ParkingRequest* request3 = system.getRequest(req3);
    assertTest(request3->getState() == ALLOCATED, "Cross-zone request is ALLOCATED");
    assertTest(request3->getRequestedZone() == 1, "Requested zone is still 1");
    assertTest(request3->getAllocatedZoneID() == 2, "Actually allocated to Zone 2");
    assertTest(request3->isCrossZone(), "Cross-zone flag is set to true");
    
    assertTest(zone1->getAvailableSlots() == 0, "Zone 1 still has 0 slots available");
    assertTest(zone2->getAvailableSlots() == 2, "Zone 2 has 2 slots available (1 used)");
    
    cout << "  -> Request 3: Requested Zone " << request3->getRequestedZone() 
         << ", Allocated Zone " << request3->getAllocatedZoneID() 
         << ", Cross-Zone: " << (request3->isCrossZone() ? "YES" : "NO") << endl;
    
    // Verify cross-zone penalty in cost calculation
    system.occupyParking(req3, errorMsg);
    system.releaseParking(req3, 10, errorMsg);
    
    double cost = system.calculateParkingCost(req3);
    double expectedBaseCost = (10 - 2) * 10.0; // 8 time units * 10 rate
    double expectedTotal = expectedBaseCost * 1.5; // 50% penalty
    
    assertTest(cost == expectedTotal, "Cross-zone penalty (50%) correctly applied to cost");
    cout << "  -> Parking cost: " << cost << " (Base: " << expectedBaseCost 
         << " + 50% penalty)" << endl;
}

// ============================================================================
// TEST 3: Cancellation and Rollback Correctness
// ============================================================================
void test_cancellation_and_rollback() {
    printTestHeader("3. Cancellation and Rollback Correctness");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    system.setupZone(1, 2, 5, errorMsg); // 10 slots total
    
    Zone* zone = system.getZone(1);
    int initialAvailable = zone->getAvailableSlots();
    
    // Create and allocate 5 requests
    int requests[5];
    for (int i = 0; i < 5; i++) {
        requests[i] = system.createRequest(300 + i, 1, i, errorMsg);
        system.allocateParking(requests[i], errorMsg);
    }
    
    assertTest(zone->getAvailableSlots() == initialAvailable - 5, 
               "5 slots occupied after 5 allocations");
    
    // Test single cancellation
    ParkingRequest* req2 = system.getRequest(requests[2]);
    int cancelledSlot = req2->getAllocatedSlotID();
    
    bool cancelSuccess = system.cancelRequest(requests[2], errorMsg);
    assertTest(cancelSuccess, "Request cancellation successful");
    assertTest(req2->getState() == CANCELLED, "Request state changed to CANCELLED");
    
    // Verify slot is freed
    ParkingSlot* freedSlot = zone->getSlotByID(cancelledSlot);
    assertTest(freedSlot != nullptr, "Freed slot found in zone");
    assertTest(freedSlot->getAvailability(), "Freed slot is now available");
    assertTest(zone->getAvailableSlots() == initialAvailable - 4, 
               "Available slots increased after cancellation");
    
    // Test rollback of last K allocations
    int beforeRollback = zone->getAvailableSlots();
    
    bool rollbackSuccess = system.rollbackLastKAllocations(3, errorMsg);
    assertTest(rollbackSuccess, "Rollback of last 3 allocations successful");
    
    int afterRollback = zone->getAvailableSlots();
    assertTest(afterRollback >= beforeRollback + 3, 
               "At least 3 more slots available after rollback");
    
    // Verify rolled back requests are cancelled
    for (int i = 3; i < 5; i++) {
        ParkingRequest* req = system.getRequest(requests[i]);
        if (req->getState() != CANCELLED && i >= 2) {
            // The last allocated ones should be cancelled
            assertTest(req->getState() == CANCELLED, 
                       "Rolled back request is in CANCELLED state");
        }
    }
    
    cout << "  -> Before rollback: " << beforeRollback << " available slots" << endl;
    cout << "  -> After rollback: " << afterRollback << " available slots" << endl;
    cout << "  -> Slots freed: " << (afterRollback - beforeRollback) << endl;
}

// ============================================================================
// TEST 4: Invalid State Transition Detection
// ============================================================================
void test_invalid_state_transitions() {
    printTestHeader("4. Invalid State Transition Detection");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    system.setupZone(1, 1, 5, errorMsg);
    
    // Test 1: Cannot occupy without allocation
    int req1 = system.createRequest(401, 1, 0, errorMsg);
    bool occupyFail = system.occupyParking(req1, errorMsg);
    assertTest(!occupyFail, "Cannot occupy parking without allocation");
    
    ParkingRequest* request1 = system.getRequest(req1);
    assertTest(request1->getState() == REQUESTED, 
               "Request remains in REQUESTED state after failed occupy");
    
    // Test 2: Cannot release without occupation
    bool releaseFail = system.releaseParking(req1, 10, errorMsg);
    assertTest(!releaseFail, "Cannot release parking without occupation");
    
    // Test 3: Proper state progression
    system.allocateParking(req1, errorMsg);
    assertTest(request1->getState() == ALLOCATED, "Request successfully allocated");
    
    bool occupySuccess = system.occupyParking(req1, errorMsg);
    assertTest(occupySuccess, "Successfully occupied after allocation");
    assertTest(request1->getState() == OCCUPIED, "Request state is OCCUPIED");
    
    // Test 4: Cannot allocate an already allocated request
    int req2 = system.createRequest(402, 1, 1, errorMsg);
    system.allocateParking(req2, errorMsg);
    
    // Try to allocate again (should fail)
    bool reallocateFail = system.allocateParking(req2, errorMsg);
    assertTest(!reallocateFail, "Cannot re-allocate an already allocated request");
    
    // Test 5: Cannot cancel an occupied request
    bool cancelOccupiedFail = system.cancelRequest(req1, errorMsg);
    assertTest(!cancelOccupiedFail, "Cannot cancel an occupied request");
    assertTest(request1->getState() == OCCUPIED, 
               "Request remains OCCUPIED after failed cancel");
    
    // Test 6: Can only release after occupation
    bool releaseSuccess = system.releaseParking(req1, 20, errorMsg);
    assertTest(releaseSuccess, "Successfully released occupied parking");
    assertTest(request1->getState() == RELEASED, "Request state is RELEASED");
    
    // Test 7: Cannot release again
    bool rereleaseFlail = system.releaseParking(req1, 30, errorMsg);
    assertTest(!rereleaseFlail, "Cannot re-release an already released request");
    
    // Test 8: Release time validation
    int req3 = system.createRequest(403, 1, 5, errorMsg);
    system.allocateParking(req3, errorMsg);
    system.occupyParking(req3, errorMsg);
    
    bool invalidTimeFail = system.releaseParking(req3, 3, errorMsg); // Before request time
    assertTest(!invalidTimeFail, "Cannot release with time before request time");
    
    cout << "  -> All invalid state transitions properly rejected" << endl;
}

// ============================================================================
// TEST 5: Analytics Correctness After Rollback
// ============================================================================
void test_analytics_after_rollback() {
    printTestHeader("5. Analytics Correctness After Rollback");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    system.setupZone(1, 2, 5, errorMsg); // 10 slots
    system.setupZone(2, 1, 5, errorMsg); // 5 slots
    
    // Create multiple requests with different outcomes
    int req1 = system.createRequest(501, 1, 0, errorMsg);
    system.allocateParking(req1, errorMsg);
    system.occupyParking(req1, errorMsg);
    system.releaseParking(req1, 10, errorMsg); // Completed
    
    int req2 = system.createRequest(502, 1, 2, errorMsg);
    system.allocateParking(req2, errorMsg);
    system.occupyParking(req2, errorMsg);
    system.releaseParking(req2, 15, errorMsg); // Completed
    
    int req3 = system.createRequest(503, 2, 5, errorMsg);
    system.allocateParking(req3, errorMsg);
    system.occupyParking(req3, errorMsg);
    system.releaseParking(req3, 20, errorMsg); // Completed
    
    // Check analytics before rollback
    int cancelled1, completed1;
    system.getCancelledVsCompleted(cancelled1, completed1);
    assertTest(completed1 == 3, "3 requests completed before rollback");
    assertTest(cancelled1 == 0, "0 requests cancelled before rollback");
    
    double avgDuration1 = system.getAverageParkingDuration();
    double expectedAvg = ((10-0) + (15-2) + (20-5)) / 3.0;
    assertTest(fabs(avgDuration1 - expectedAvg) < 0.01, 
               "Average parking duration correct before rollback");
    
    double utilization1 = system.getSystemUtilization();
    assertTest(utilization1 >= 0 && utilization1 <= 100, 
               "System utilization in valid range [0-100]");
    
    cout << "  -> Before rollback: " << completed1 << " completed, " 
         << cancelled1 << " cancelled" << endl;
    cout << "  -> Average duration: " << fixed << setprecision(2) 
         << avgDuration1 << " time units" << endl;
    
    // Perform rollback
    int req4 = system.createRequest(504, 1, 25, errorMsg);
    system.allocateParking(req4, errorMsg);
    
    int req5 = system.createRequest(505, 1, 26, errorMsg);
    system.allocateParking(req5, errorMsg);
    
    system.rollbackLastKAllocations(2, errorMsg);
    
    // Check analytics after rollback
    int cancelled2, completed2;
    system.getCancelledVsCompleted(cancelled2, completed2);
    
    assertTest(cancelled2 >= 2, "At least 2 requests cancelled after rollback");
    assertTest(completed2 == 3, "Completed count unchanged after rollback");
    
    double avgDuration2 = system.getAverageParkingDuration();
    assertTest(fabs(avgDuration2 - avgDuration1) < 0.01, 
               "Average duration unchanged (only considers completed)");
    
    cout << "  -> After rollback: " << completed2 << " completed, " 
         << cancelled2 << " cancelled" << endl;
    cout << "  -> Average duration remains: " << fixed << setprecision(2) 
         << avgDuration2 << " time units" << endl;
    
    // Verify peak usage zone
    int peakZone = system.getPeakUsageZone();
    assertTest(peakZone >= 1 && peakZone <= 2, "Peak usage zone is valid");
    cout << "  -> Peak usage zone: " << peakZone << endl;
    
    // Verify zone utilization
    double zone1Util = system.getZoneUtilization(1);
    double zone2Util = system.getZoneUtilization(2);
    assertTest(zone1Util >= 0 && zone1Util <= 100, "Zone 1 utilization in valid range");
    assertTest(zone2Util >= 0 && zone2Util <= 100, "Zone 2 utilization in valid range");
    
    cout << "  -> Zone 1 utilization: " << fixed << setprecision(1) << zone1Util << "%" << endl;
    cout << "  -> Zone 2 utilization: " << fixed << setprecision(1) << zone2Util << "%" << endl;
}

// ============================================================================
// TEST 6: Zone Expansion and Validation
// ============================================================================
void test_zone_expansion_and_validation() {
    printTestHeader("6. Zone Expansion and Validation");
    
    ParkingSystem system(3); // Start with 3 zones
    char errorMsg[200];
    
    assertTest(system.getMaxZoneID() == 3, "Initial max zone ID is 3");
    
    // Try to setup invalid zone (outside range)
    bool invalidSetup = system.setupZone(5, 1, 5, errorMsg);
    assertTest(!invalidSetup, "Cannot setup zone 5 (outside initial range 1-3)");
    
    // Expand city zones
    bool expandSuccess = system.expandCityZones(3, errorMsg);
    assertTest(expandSuccess, "City expansion by 3 zones successful");
    assertTest(system.getMaxZoneID() == 6, "Max zone ID increased to 6");
    
    // Now setup zone 5 should work
    bool validSetup = system.setupZone(5, 2, 4, errorMsg);
    assertTest(validSetup, "Zone 5 setup successful after expansion");
    
    Zone* zone5 = system.getZone(5);
    assertTest(zone5 != nullptr, "Zone 5 exists");
    assertTest(zone5->getTotalSlots() == 8, "Zone 5 has correct number of slots (2*4=8)");
    
    // Try to setup zone beyond new limit
    bool stillInvalid = system.setupZone(10, 1, 5, errorMsg);
    assertTest(!stillInvalid, "Cannot setup zone 10 (beyond max zone ID 6)");
    
    // Setup remaining zones
    system.setupZone(1, 1, 3, errorMsg);
    system.setupZone(3, 1, 5, errorMsg);
    
    assertTest(system.getActiveZoneCount() == 3, "3 zones are active");
    assertTest(system.isZoneActive(1), "Zone 1 is active");
    assertTest(system.isZoneActive(3), "Zone 3 is active");
    assertTest(system.isZoneActive(5), "Zone 5 is active");
    assertTest(!system.isZoneActive(2), "Zone 2 is not active (not set up)");
    
    cout << "  -> Total city zones: " << system.getMaxZoneID() << endl;
    cout << "  -> Active zones: " << system.getActiveZoneCount() << endl;
}

// ============================================================================
// TEST 7: Complete Request Lifecycle
// ============================================================================
void test_complete_request_lifecycle() {
    printTestHeader("7. Complete Request Lifecycle");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    system.setupZone(1, 1, 5, errorMsg);
    
    Zone* zone = system.getZone(1);
    int initialAvailable = zone->getAvailableSlots();
    
    // Create request
    int req = system.createRequest(701, 1, 0, errorMsg);
    assertTest(req >= 0, "Request created successfully");
    
    ParkingRequest* request = system.getRequest(req);
    assertTest(request != nullptr, "Request retrieved successfully");
    assertTest(request->getState() == REQUESTED, "Initial state is REQUESTED");
    assertTest(request->getVehicleID() == 701, "Vehicle ID correct");
    assertTest(request->getRequestTime() == 0, "Request time correct");
    
    // Allocate
    bool allocSuccess = system.allocateParking(req, errorMsg);
    assertTest(allocSuccess, "Parking allocated successfully");
    assertTest(request->getState() == ALLOCATED, "State changed to ALLOCATED");
    assertTest(request->getAllocatedSlotID() > 0, "Slot ID assigned");
    assertTest(zone->getAvailableSlots() == initialAvailable - 1, 
               "Available slots decreased by 1");
    
    int allocatedSlot = request->getAllocatedSlotID();
    ParkingSlot* slot = zone->getSlotByID(allocatedSlot);
    assertTest(slot != nullptr, "Allocated slot found");
    assertTest(!slot->getAvailability(), "Allocated slot marked as unavailable");
    
    // Occupy
    bool occupySuccess = system.occupyParking(req, errorMsg);
    assertTest(occupySuccess, "Parking occupied successfully");
    assertTest(request->getState() == OCCUPIED, "State changed to OCCUPIED");
    
    // Release
    bool releaseSuccess = system.releaseParking(req, 25, errorMsg);
    assertTest(releaseSuccess, "Parking released successfully");
    assertTest(request->getState() == RELEASED, "State changed to RELEASED");
    assertTest(request->getReleaseTime() == 25, "Release time recorded correctly");
    assertTest(slot->getAvailability(), "Released slot marked as available");
    assertTest(zone->getAvailableSlots() == initialAvailable, 
               "Available slots back to initial count");
    
    // Calculate cost
    double cost = system.calculateParkingCost(req);
    double expectedCost = (25 - 0) * 10.0; // 25 time units * 10 rate
    assertTest(cost == expectedCost, "Parking cost calculated correctly");
    
    cout << "  -> Complete lifecycle: REQUESTED -> ALLOCATED -> OCCUPIED -> RELEASED" << endl;
    cout << "  -> Parking duration: 25 time units" << endl;
    cout << "  -> Total cost: " << fixed << setprecision(2) << cost << " units" << endl;
}

// ============================================================================
// TEST 8: Multiple Zone Allocation Strategy
// ============================================================================
void test_multiple_zone_allocation_strategy() {
    printTestHeader("8. Multiple Zone Allocation Strategy");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    // Setup three zones with different capacities
    system.setupZone(1, 1, 2, errorMsg); // 2 slots
    system.setupZone(2, 1, 3, errorMsg); // 3 slots
    system.setupZone(3, 1, 4, errorMsg); // 4 slots
    
    // Request zone 1 until full, then observe cross-zone behavior
    int requests[10];
    for (int i = 0; i < 10; i++) {
        requests[i] = system.createRequest(800 + i, 1, i, errorMsg);
        system.allocateParking(requests[i], errorMsg);
    }
    
    Zone* zone1 = system.getZone(1);
    Zone* zone2 = system.getZone(2);
    Zone* zone3 = system.getZone(3);
    
    // Check zone 1 is full
    assertTest(zone1->getAvailableSlots() == 0, "Zone 1 is full (0 available)");
    
    // Check overflow went to other zones
    int zone2Occupied = zone2->getTotalSlots() - zone2->getAvailableSlots();
    int zone3Occupied = zone3->getTotalSlots() - zone3->getAvailableSlots();
    int totalCapacity = zone1->getTotalSlots() + zone2->getTotalSlots() + zone3->getTotalSlots();
    
    // Total capacity is 2+3+4=9, so we can only allocate 9 out of 10 requests
    assertTest(zone2Occupied + zone3Occupied == 7, 
               "Exactly 7 overflow requests allocated to zones 2 and 3 (zone 1 has 2)");
    
    // Verify cross-zone allocations
    int crossZoneCount = 0;
    int successfulAllocations = 0;
    for (int i = 0; i < 10; i++) {
        ParkingRequest* req = system.getRequest(requests[i]);
        if (req->getState() == ALLOCATED || req->getState() == OCCUPIED) {
            successfulAllocations++;
            if (req->isCrossZone()) {
                crossZoneCount++;
                assertTest(req->getRequestedZone() == 1, 
                           "Cross-zone request originally requested zone 1");
                assertTest(req->getAllocatedZoneID() != 1, 
                           "Cross-zone request allocated to different zone");
            }
        }
    }
    
    assertTest(successfulAllocations == totalCapacity, 
               "Allocated exactly as many requests as total capacity");
    assertTest(crossZoneCount == 7, "Exactly 7 cross-zone allocations (overflow from zone 1)");
    
    cout << "  -> Zone 1 occupancy: " << (zone1->getTotalSlots() - zone1->getAvailableSlots()) 
         << "/" << zone1->getTotalSlots() << endl;
    cout << "  -> Zone 2 occupancy: " << zone2Occupied << "/" << zone2->getTotalSlots() << endl;
    cout << "  -> Zone 3 occupancy: " << zone3Occupied << "/" << zone3->getTotalSlots() << endl;
    cout << "  -> Total capacity: " << totalCapacity << " slots" << endl;
    cout << "  -> Successful allocations: " << successfulAllocations << "/10 requests" << endl;
    cout << "  -> Cross-zone allocations: " << crossZoneCount << endl;
}

// ============================================================================
// TEST 9: Rollback with Mixed Request States
// ============================================================================
void test_rollback_with_mixed_states() {
    printTestHeader("9. Rollback with Mixed Request States");
    
    ParkingSystem system(5);
    char errorMsg[200];
    
    system.setupZone(1, 2, 5, errorMsg);
    
    Zone* zone = system.getZone(1);
    
    // Create requests with different states
    int req1 = system.createRequest(901, 1, 0, errorMsg);
    system.allocateParking(req1, errorMsg);
    system.occupyParking(req1, errorMsg); // OCCUPIED
    
    int req2 = system.createRequest(902, 1, 1, errorMsg);
    system.allocateParking(req2, errorMsg); // ALLOCATED
    
    int req3 = system.createRequest(903, 1, 2, errorMsg);
    system.allocateParking(req3, errorMsg); // ALLOCATED
    
    int req4 = system.createRequest(904, 1, 3, errorMsg);
    system.allocateParking(req4, errorMsg);
    system.occupyParking(req4, errorMsg);
    system.releaseParking(req4, 20, errorMsg); // RELEASED
    
    int req5 = system.createRequest(905, 1, 4, errorMsg);
    system.allocateParking(req5, errorMsg); // ALLOCATED
    
    ParkingRequest* request1 = system.getRequest(req1);
    ParkingRequest* request2 = system.getRequest(req2);
    ParkingRequest* request4 = system.getRequest(req4);
    ParkingRequest* request5 = system.getRequest(req5);
    
    assertTest(request1->getState() == OCCUPIED, "Request 1 is OCCUPIED");
    assertTest(request2->getState() == ALLOCATED, "Request 2 is ALLOCATED");
    assertTest(request4->getState() == RELEASED, "Request 4 is RELEASED");
    assertTest(request5->getState() == ALLOCATED, "Request 5 is ALLOCATED");
    
    int availableBefore = zone->getAvailableSlots();
    
    // Rollback last 3 allocations (should affect req3, req4, req5)
    // Note: req4 is already RELEASED, so it depends on implementation
    bool rollbackSuccess = system.rollbackLastKAllocations(3, errorMsg);
    assertTest(rollbackSuccess, "Rollback successful with mixed states");
    
    int availableAfter = zone->getAvailableSlots();
    assertTest(availableAfter > availableBefore, 
               "Available slots increased after rollback");
    
    // Check that recent allocations were cancelled
    assertTest(request5->getState() == CANCELLED, 
               "Most recent allocation (req5) was cancelled");
    
    cout << "  -> Slots before rollback: " << availableBefore << " available" << endl;
    cout << "  -> Slots after rollback: " << availableAfter << " available" << endl;
    cout << "  -> Slots freed: " << (availableAfter - availableBefore) << endl;
}

// ============================================================================
// TEST 10: System Capacity and Stress Testing
// ============================================================================
void test_system_capacity() {
    printTestHeader("10. System Capacity and Stress Testing");
    
    ParkingSystem system(10);
    char errorMsg[200];
    
    // Setup multiple zones
    for (int z = 1; z <= 5; z++) {
        system.setupZone(z, 2, 3, errorMsg); // Each zone: 2 areas * 3 slots = 6 slots
    }
    
    int totalSystemSlots = 5 * 6; // 30 slots total
    assertTest(system.getAvailableSlotsInSystem() == totalSystemSlots, 
               "System has 30 total slots available");
    
    // Fill entire system
    int requestCount = 0;
    for (int i = 0; i < 40; i++) { // Try to create more requests than slots
        int req = system.createRequest(1000 + i, (i % 5) + 1, i, errorMsg);
        if (req >= 0) {
            bool allocated = system.allocateParking(req, errorMsg);
            if (allocated) {
                requestCount++;
            }
        }
    }
    
    assertTest(requestCount == totalSystemSlots, 
               "Exactly 30 requests allocated (system at full capacity)");
    assertTest(system.getAvailableSlotsInSystem() == 0, 
               "System has 0 available slots when full");
    
    double utilization = system.getSystemUtilization();
    assertTest(fabs(utilization - 100.0) < 0.1, 
               "System utilization is 100% when full");
    
    // Try one more allocation (should fail)
    int overflowReq = system.createRequest(2000, 1, 100, errorMsg);
    bool shouldFail = system.allocateParking(overflowReq, errorMsg);
    assertTest(!shouldFail, "Cannot allocate when system is full");
    
    // Release some parking
    for (int i = 0; i < 10; i++) {
        ParkingRequest* req = system.getRequest(i);
        if (req && req->getState() == ALLOCATED) {
            system.occupyParking(i, errorMsg);
            system.releaseParking(i, 200 + i, errorMsg);
        }
    }
    
    int availableAfterRelease = system.getAvailableSlotsInSystem();
    assertTest(availableAfterRelease >= 10, 
               "At least 10 slots available after releasing 10 parkings");
    
    double utilizationAfter = system.getSystemUtilization();
    assertTest(utilizationAfter < 100.0, "System utilization decreased after releases");
    
    cout << "  -> Total system capacity: " << totalSystemSlots << " slots" << endl;
    cout << "  -> Requests allocated: " << requestCount << endl;
    cout << "  -> Available after releases: " << availableAfterRelease << endl;
    cout << "  -> Final utilization: " << fixed << setprecision(1) 
         << utilizationAfter << "%" << endl;
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================
int main() {
    cout << "\n";
    cout << "============================================================" << endl;
    cout << "   SMART PARKING MANAGEMENT SYSTEM - TEST SUITE            " << endl;
    cout << "============================================================" << endl;
    cout << "\nRunning comprehensive test cases...\n" << endl;
    
    // Run all tests
    test_slot_allocation_correctness();
    test_cross_zone_allocation();
    test_cancellation_and_rollback();
    test_invalid_state_transitions();
    test_analytics_after_rollback();
    test_zone_expansion_and_validation();
    test_complete_request_lifecycle();
    test_multiple_zone_allocation_strategy();
    test_rollback_with_mixed_states();
    test_system_capacity();
    
    // Print final summary
    printTestSummary();
    
    if (failedTests == 0) {
        cout << "\n" << GREEN << " ALL TESTS PASSED! " << RESET << endl;
        return 0;
    } else {
        cout << "\n" << RED << " SOME TESTS FAILED! " << RESET << endl;
        return 1;
    }
}