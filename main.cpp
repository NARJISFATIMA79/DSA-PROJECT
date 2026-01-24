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