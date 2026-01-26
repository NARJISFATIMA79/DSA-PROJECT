#include <iostream>
#include "ParkingSystem.h"

using namespace std;

void displayMenu() {
    cout << "\n============================================" << endl;
    cout << "   SMART PARKING MANAGEMENT SYSTEM v2.0    " << endl;
    cout << "============================================" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| SETUP & CONFIGURATION                    |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| 1.  Setup Zone                           |" << endl;
    cout << "| 2.  Expand City Zones                    |" << endl;
    cout << "| 3.  View Active Zones                    |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| PARKING OPERATIONS                       |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| 4.  Create Parking Request               |" << endl;
    cout << "| 5.  Allocate Parking                     |" << endl;
    cout << "| 6.  Occupy Parking                       |" << endl;
    cout << "| 7.  Release Parking                      |" << endl;
    cout << "| 8.  Cancel Request                       |" << endl;
    cout << "| 9.  Rollback Last K Allocations          |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| ANALYTICS & REPORTS                      |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| 10. System Dashboard                     |" << endl;
    cout << "| 11. View Zone Details                    |" << endl;
    cout << "| 12. View All Requests Status             |" << endl;
    cout << "| 13. Analytics Summary                    |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| 0.  Exit System                          |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "Enter choice: ";
}

string getStateName(RequestState state) {
    switch(state) {
        case REQUESTED: return "REQUESTED";
        case ALLOCATED: return "ALLOCATED";
        case OCCUPIED: return "OCCUPIED";
        case RELEASED: return "RELEASED";
        case CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

void displayError(const char* errorMsg) {
    cout << "\n+------------------------------------------+" << endl;
    cout << "| ERROR                                    |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| " << errorMsg << endl;
    cout << "+------------------------------------------+" << endl;
}


void displaySuccess(const char* message) {
    cout << "\n+------------------------------------------+" << endl;
    cout << "| SUCCESS                                  |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| " << message << endl;
    cout << "+------------------------------------------+" << endl;
}
int main() {
    cout << "\n============================================" << endl;
    cout << "  Welcome to Smart Parking Management!     " << endl;
    cout << "============================================" << endl;
    
    int initialZones;
    cout << "\nEnter number of zones in the city (1-100): ";
    cin >> initialZones;
    
    if (initialZones < 1 || initialZones > 100) {
        cout << "Invalid! Using default: 10 zones" << endl;
        initialZones = 10;
    }
    
    ParkingSystem system(initialZones);
    cout << "\n✓ System initialized with " << initialZones << " zones (Zone IDs: 1-" << initialZones << ")" << endl;
    
    int choice;
    char errorMsg[200];
    
    do {
        displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1: { // Setup Zone
                int zoneID, numAreas, slotsPerArea;
                cout << "\n============================================" << endl;
                cout << " SETUP ZONE                                 " << endl;
                cout << "============================================" << endl;
                cout << "Valid Zone IDs: 1-" << system.getMaxZoneID() << endl;
                cout << "Active Zones: " << system.getActiveZoneCount() << "/" << system.getMaxZoneID() << endl;
                cout << "\nEnter Zone ID: ";
                cin >> zoneID;
                cout << "Enter number of parking areas: ";
                cin >> numAreas;
                cout << "Enter slots per area: ";
                cin >> slotsPerArea;
                
                errorMsg[0] = '\0';
                if (system.setupZone(zoneID, numAreas, slotsPerArea, errorMsg)) {
                    cout << "\n+------------------------------------------+" << endl;
                    cout << "| Zone " << zoneID << " Setup Successfully            |" << endl;
                    cout << "+------------------------------------------+" << endl;
                    cout << "| Zone ID:        " << setw(4) << zoneID << "                      |" << endl;
                    cout << "| Areas:          " << setw(4) << numAreas << "                      |" << endl;
                    cout << "| Slots/Area:     " << setw(4) << slotsPerArea << "                      |" << endl;
                    cout << "| Total Slots:    " << setw(4) << (numAreas * slotsPerArea) << "                      |" << endl;
                    cout << "| Slot IDs:       1 to " << (numAreas * slotsPerArea) << "                   |" << endl;
                    cout << "+------------------------------------------+" << endl;
                    
                    // Verify the zone was created correctly
                    Zone* verifyZone = system.getZone(zoneID);
                    if (verifyZone != nullptr) {
                        cout << "\nVerification: Zone " << verifyZone->getZoneID() 
                             << " has " << verifyZone->getTotalSlots() << " total slots, "
                             << verifyZone->getAvailableSlots() << " available" << endl;
                    }
                } else {
                    displayError(errorMsg);
                }
                break;
            }

            case 2: { // Expand City Zones
                int additional;
                cout << "\n============================================" << endl;
                cout << " EXPAND CITY ZONES                          " << endl;
                cout << "============================================" << endl;
                cout << "Current Maximum Zone ID: " << system.getMaxZoneID() << endl;
                cout << "Enter additional zones to add: ";
                cin >> additional;
                
                errorMsg[0] = '\0';
                if (system.expandCityZones(additional, errorMsg)) {
                    cout << "\n+------------------------------------------+" << endl;
                    cout << "| City Expanded Successfully               |" << endl;
                    cout << "+------------------------------------------+" << endl;
                    cout << "| New Maximum Zone ID: " << system.getMaxZoneID() << "                |" << endl;
                    cout << "| Valid Zone Range: 1-" << system.getMaxZoneID() << "               |" << endl;
                    cout << "+------------------------------------------+" << endl;
                } else {
                    displayError(errorMsg);
                }
                break;
            }
            
            case 3: { // View Active Zones
                cout << "\n============================================" << endl;
                cout << " ACTIVE ZONES                               " << endl;
                cout << "============================================" << endl;
                cout << "City Zone Range: 1-" << system.getMaxZoneID() << endl;
                cout << "Active Zones: " << system.getActiveZoneCount() << "/" << system.getMaxZoneID() << endl;
                
                int activeZones[100];
                int count;
                system.getActiveZones(activeZones, count);
                
                if (count > 0) {
                    cout << "\nActive Zone IDs: ";
                    for (int i = 0; i < count; i++) {
                        cout << activeZones[i];
                        if (i < count - 1) cout << ", ";
                    }
                    cout << endl;
                    
                    cout << "\nInactive Zones: ";
                    bool hasInactive = false;
                    for (int i = 1; i <= system.getMaxZoneID(); i++) {
                        if (!system.isZoneActive(i)) {
                            if (hasInactive) cout << ", ";
                            cout << i;
                            hasInactive = true;
                        }
                    }
                    if (!hasInactive) cout << "None";
                    cout << endl;
                } else {
                    cout << "\n⚠ No zones have been set up yet!" << endl;
                }
                break;
            }

            case 4: { // Create Parking Request
                int vehicleID, requestedZone, requestTime;
                cout << "\n============================================" << endl;
                cout << " CREATE PARKING REQUEST                     " << endl;
                cout << "============================================" << endl;
                cout << "Valid Zones: 1-" << system.getMaxZoneID() << " (Active: " << system.getActiveZoneCount() << ")" << endl;
                cout << "\nEnter Vehicle ID: ";
                cin >> vehicleID;
                cout << "Enter Requested Zone ID: ";
                cin >> requestedZone;
                cout << "Enter Request Time: ";
                cin >> requestTime;
                
                errorMsg[0] = '\0';
                int requestIndex = system.createRequest(vehicleID, requestedZone, requestTime, errorMsg);
                if (requestIndex != -1) {
                    cout << "\n+------------------------------------------+" << endl;
                    cout << "| Request Created Successfully             |" << endl;
                    cout << "+------------------------------------------+" << endl;
                    cout << "| Request Index:  " << setw(4) << requestIndex << "                     |" << endl;
                    cout << "| Vehicle ID:     " << setw(4) << vehicleID << "                     |" << endl;
                    cout << "| Requested Zone: " << setw(4) << requestedZone << "                     |" << endl;
                    cout << "| Status:         REQUESTED                |" << endl;
                    cout << "+------------------------------------------+" << endl;
                } else {
                    displayError(errorMsg);
                }
                break;
            }
            
            case 5: { // Allocate Parking
                int requestIndex;
                cout << "\n============================================" << endl;
                cout << " ALLOCATE PARKING                           " << endl;
                cout << "============================================" << endl;
                cout << "Available Slots in System: " << system.getAvailableSlotsInSystem() << endl;
                cout << "\nEnter Request Index: ";
                cin >> requestIndex;
                
                // Show request details before allocation
                ParkingRequest* preReq = system.getRequest(requestIndex);
                if (preReq != nullptr) {
                    cout << "\nRequest Details:" << endl;
                    cout << "  Vehicle ID: " << preReq->getVehicleID() << endl;
                    cout << "  Requested Zone: " << preReq->getRequestedZone() << endl;
                    cout << "  Current State: " << getStateName(preReq->getState()) << endl;
                }
                
                errorMsg[0] = '\0';
                bool success = system.allocateParking(requestIndex, errorMsg);
                if (success) {
                    ParkingRequest* req = system.getRequest(requestIndex);
                    cout << "\n+------------------------------------------+" << endl;
                    cout << "| Parking Allocated Successfully           |" << endl;
                    cout << "+------------------------------------------+" << endl;
                    cout << "| Request Index:     " << setw(4) << requestIndex << "                  |" << endl;
                    cout << "| Vehicle ID:        " << setw(4) << req->getVehicleID() << "                  |" << endl;
                    cout << "| Requested Zone:    " << setw(4) << req->getRequestedZone() << "                  |" << endl;
                    cout << "| Allocated Zone:    " << setw(4) << req->getAllocatedZoneID() << "                  |" << endl;
                    cout << "| Allocated Slot:    " << setw(4) << req->getAllocatedSlotID() << "                  |" << endl;
                    cout << "| Cross-Zone:        " << (req->isCrossZone() ? "YES" : "NO ") << "                  |" << endl;
                    cout << "| Status:            ALLOCATED             |" << endl;
                    cout << "+------------------------------------------+" << endl;
                    
                    if (req->isCrossZone()) {
                        cout << "\n*** CROSS-ZONE ALLOCATION ***" << endl;
                        cout << "Requested zone " << req->getRequestedZone() 
                             << " was full. Allocated in zone " << req->getAllocatedZoneID() << endl;
                    }
                } else {
                    displayError(errorMsg);
                }
                break;
            }

                        case 6: { // Occupy Parking
                int requestIndex;
                cout << "\n============================================" << endl;
                cout << " OCCUPY PARKING                             " << endl;
                cout << "============================================" << endl;
                cout << "Enter Request Index: ";
                cin >> requestIndex;
                
                errorMsg[0] = '\0';
                if (system.occupyParking(requestIndex, errorMsg)) {
                    displaySuccess("Parking occupied successfully!");
                } else {
                    displayError(errorMsg);
                }
                break;
            }
            
            case 7: { // Release Parking
                int requestIndex, releaseTime;
                cout << "\n============================================" << endl;
                cout << " RELEASE PARKING                            " << endl;
                cout << "============================================" << endl;
                cout << "Enter Request Index: ";
                cin >> requestIndex;
                
                ParkingRequest* req = system.getRequest(requestIndex);
                if (req != nullptr) {
                    cout << "Request Time: " << req->getRequestTime() << endl;
                    cout << "Enter Release Time (must be > " << req->getRequestTime() << "): ";
                    cin >> releaseTime;
                    
                    errorMsg[0] = '\0';
                    if (system.releaseParking(requestIndex, releaseTime, errorMsg)) {
                        int duration = releaseTime - req->getRequestTime();
                        cout << "\n+------------------------------------------+" << endl;
                        cout << "| Parking Released Successfully            |" << endl;
                        cout << "+------------------------------------------+" << endl;
                        cout << "| Parking Duration: " << setw(4) << duration << " time units     |" << endl;
                        cout << "+------------------------------------------+" << endl;
                    } else {
                        displayError(errorMsg);
                    }
                } else {
                    displayError("Invalid request index!");
                }
                break;
            }

            case 8: { // Cancel Request
                int requestIndex;
                cout << "\n============================================" << endl;
                cout << " CANCEL REQUEST                             " << endl;
                cout << "============================================" << endl;
                cout << "Enter Request Index: ";
                cin >> requestIndex;
                
                errorMsg[0] = '\0';
                if (system.cancelRequest(requestIndex, errorMsg)) {
                    displaySuccess("Request cancelled successfully!");
                } else {
                    displayError(errorMsg);
                }
                break;
            }
            
            case 9: { // Rollback Last K Allocations
                int k;
                cout << "\n============================================" << endl;
                cout << " ROLLBACK ALLOCATIONS                       " << endl;
                cout << "============================================" << endl;
                cout << "Enter number of allocations to rollback: ";
                cin >> k;
                
                errorMsg[0] = '\0';
                if (system.rollbackLastKAllocations(k, errorMsg)) {
                    displaySuccess("Rollback completed successfully!");
                } else {
                    displayError(errorMsg);
                }
                break;
            }

            case 10: { // System Dashboard
                cout << "\n============================================" << endl;
                cout << " SYSTEM DASHBOARD                           " << endl;
                cout << "============================================" << endl;
                
                cout << "\n+--- City Configuration -------------------+" << endl;
                cout << "| Total City Zones:    " << setw(4) << system.getMaxZoneID() << "                 |" << endl;
                cout << "| Active Zones:        " << setw(4) << system.getActiveZoneCount() << "                 |" << endl;
                cout << "| Inactive Zones:      " << setw(4) << (system.getMaxZoneID() - system.getActiveZoneCount()) << "                 |" << endl;
                cout << "+------------------------------------------+" << endl;
                
                cout << "\n+--- System Statistics --------------------+" << endl;
                cout << "| Available Slots:     " << setw(4) << system.getAvailableSlotsInSystem() << "                 |" << endl;
                cout << "| System Utilization:  " << fixed << setprecision(1) << setw(5) << system.getSystemUtilization() << "%               |" << endl;
                
                int peakZone = system.getPeakUsageZone();
                if (peakZone != -1) {
                    cout << "| Peak Usage Zone:     " << setw(4) << peakZone << "                 |" << endl;
                }
                
                double avgDuration = system.getAverageParkingDuration();
                if (avgDuration > 0) {
                    cout << "| Avg Park Duration:   " << fixed << setprecision(1) << setw(6) << avgDuration << " units        |" << endl;
                }
                cout << "+------------------------------------------+" << endl;
                
                int cancelled, completed;
                system.getCancelledVsCompleted(cancelled, completed);
                if (cancelled + completed > 0) {
                    cout << "\n+--- Request Statistics --------------+" << endl;
                    cout << "│ Completed:           " << setw(4) << completed << "                   │" << endl;
                    cout << "│ Cancelled:           " << setw(4) << cancelled << "                   │" << endl;
                    cout << "│ Completion Rate:     " << fixed << setprecision(1) << setw(5) 
                         << ((double)completed / (cancelled + completed) * 100) << "%                 │" << endl;
                    cout << "+----------------------------------------+" << endl;
                }
                break;
            }
            
            case 11: { // View Zone Details
                cout << "\n============================================" << endl;
                cout << " ZONE DETAILS                               " << endl;
                cout << "============================================" << endl;
                
                if (system.getActiveZoneCount() == 0) {
                    cout << "\nNo zones have been set up yet!" << endl;
                    break;
                }
                
                cout << "\n+------+---------+-----------+----------+----------+" << endl;
                cout << "| Zone |  Total  | Available | Occupied |   Util.  |" << endl;
                cout << "+------+---------+-----------+----------+----------+" << endl;
                
                for (int zid = 1; zid <= system.getMaxZoneID(); zid++) {
                    Zone* zone = system.getZone(zid);
                    if (zone != nullptr && zone->getTotalSlots() > 0) {
                        int total = zone->getTotalSlots();
                        int available = zone->getAvailableSlots();
                        int occupied = total - available;
                        double util = system.getZoneUtilization(zid);
                        
                        cout << "| " << setw(4) << zid << " | " 
                             << setw(7) << total << " | " 
                             << setw(9) << available << " | " 
                             << setw(8) << occupied << " | " 
                             << setw(6) << fixed << setprecision(1) << util << "% |" << endl;
                    }
                }
                cout << "+------+---------+-----------+----------+----------+" << endl;
                break;
            }
            
            