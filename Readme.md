## 📌 Development Log

### Day 1 – Project Setup
- Created the overall project structure  
- Added all required header (`.h`) and source (`.cpp`) files  
- Defined classes and their responsibilities (initial skeleton, no logic)

> This phase focused on laying down the architecture before implementation.

### Day 2 – Initial Implementation
- Started writing code for selected modules  
- Implemented core methods and basic program flow  
- Focused on clarity and correct logic rather than optimization

> The system is now functional at a basic level and ready for further expansion.

## 🛠️ Development Progress

### Day 3 – Modularization & Validation Logic
- Added class declarations and function prototypes across multiple header (`.h`) files  
- Implemented **zone ID validation** to prevent invalid or out-of-range parking requests  
- Introduced **multiple constructors** to support flexible object initialization  
- Improved code organization by separating responsibilities into dedicated modules

> Focused on correctness and structure — making sure invalid data never enters the system.

---

### Day 4 – Memory Management & Rollback Handling
- Implemented **dynamic array setup** for managing variable parking capacity at runtime  
- Added **rollback allocation functionality** to safely undo partial or failed parking assignments  
- Strengthened error-handling paths to maintain system consistency  
- Refined interactions between modules to ensure stable state transitions

> This phase was about safety: if something fails, the system knows how to recover.

## 🛠️ Development Progress

### Day 5 – Menu System & Control Flow (Main Logic)
- Designed and implemented a **case-driven menu system** in `main()` to control overall program flow  
- Organized the menu into logical sections:
  - **Setup & Configuration**
  - **Parking Operations**
  - **Analytics & Reports**
- Implemented core menu cases:
  - Setup Zone  
  - Expand City Zones  
  - View Active Zones  
  - Create Parking Request  
  - Allocate Parking  
  - Occupy Parking  
  - Release Parking  
  - Cancel Request  
  - Rollback Last K Allocations  
- Enforced **operation order dependencies** (e.g., zones must exist before parking requests)  
- Added menu-level input validation to block invalid or duplicate actions  
- Connected menu options cleanly to backend modules without embedding business logic in `main()`


---

### Day 6 – State Management, Request Tracking & System Intelligence
- Implemented **parking request tracking** across its full lifecycle:
  - Created → Allocated → Occupied → Released / Cancelled  
- Added **unique request indexing** to reference, update, cancel, and rollback operations reliably  
- Implemented **getters and setters** for controlled access to:
  - Zone IDs  
  - Total slots and available slots  
  - Occupied slots  
  - Request identifiers and status  
- Introduced **explicit state transition rules** to prevent illegal operations:
  - A slot cannot be occupied unless allocated  
  - A request cannot be released unless occupied  
- Strengthened **rollback logic** to safely undo the last **K parking allocations** using request history  
- Ensured all modules stay synchronized when state changes occur (zones, slots, requests)  
- Improved internal consistency checks to avoid partial updates and invalid system states


---

### 🔍 Key Functional Highlights
- Structured, case-driven menu system  
- Full request lifecycle management  
- Safe state transitions and validation logic  
- Robust cancellation and rollback handling  
- Clean separation between user interface flow and core business logic  
- Scalable foundation for analytics and reporting features
