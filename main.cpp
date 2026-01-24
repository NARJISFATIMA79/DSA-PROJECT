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