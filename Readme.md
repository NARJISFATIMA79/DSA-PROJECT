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

