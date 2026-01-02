# Stack-Based Browser Navigation System
## Project Report Outline

---

## 1. Introduction

This project implements a **Stack-Based Web Browser Navigation System** using pure C programming language as a Data Structures and Algorithms (DSA) course project.

**Key Concept:** Real-world web browsers use stacks to manage browser history (back/forward functionality). This project demonstrates how stacks work in practical applications.

**Project Goal:** 
- Implement a complete stack data structure from scratch
- Apply stacks to solve a real-world problem (browser navigation)
- Create both CLI and web interface to visualize stack operations

---

## 2. Problem Statement

**Challenge:** 
When users browse websites, they need to:
1. Navigate to new websites
2. Go back to previously visited sites
3. Go forward to sites visited after going back
4. Clear history (reset) when starting a new browsing session

**How Stacks Solve This:**
- **Back Stack:** Stores all visited pages (LIFO - Last In First Out)
- **Forward Stack:** Stores pages visited after going back
- **Navigation:** Uses push/pop operations to manage history

---

## 3. Solution Overview

A complete browser navigation system built entirely in C with:
- **Backend:** Pure C implementation of stacks and browser navigation
- **Frontend:** Interactive web interface for visualization
- **HTTP Server:** C-based server to bridge frontend and backend
- **Operations:** Navigate, Back, Forward, Reset

---

## 4. Core Implementation

### 4.1 Stack Data Structure (`stack.c` & `stack.h`)

**File: stack.h** (Header)
```c
typedef struct {
    char url[MAX_URL_LENGTH];
    char title[MAX_URL_LENGTH];
} StackItem;

typedef struct {
    StackItem items[MAX_STACK_SIZE];
    int top;
} Stack;
```

**File: stack.c** (Implementation)

**Key Functions:**

| Function | Purpose | Time Complexity |
|----------|---------|-----------------|
| `push()` | Add item to stack | O(1) |
| `pop()` | Remove and return item | O(1) |
| `peek()` | View top without removing | O(1) |
| `isEmpty()` | Check if stack is empty | O(1) |
| `isFull()` | Check if stack is full | O(1) |
| `stackSize()` | Get number of items | O(1) |

**LIFO Principle:**
```
Push: A → B → C
Stack: [A, B, C]
Pop: C → B → A (Reverse order)
```

**Why This Matters:**
- Demonstrates understanding of fundamental data structure
- Shows boundary conditions (overflow/underflow)
- Proves O(1) time complexity operations

---

### 4.2 Browser Navigation Logic (`navigation.c` & `navigation.h`)

**File: navigation.h** (Structures)
```c
typedef struct {
    Stack* backStack;           // History of visited pages
    Stack* forwardStack;        // Pages visited after going back
    char currentURL[MAX_URL_LENGTH];
    char currentTitle[MAX_URL_LENGTH];
} WebBrowser;
```

**File: navigation.c** (Implementation)

**Key Functions:**

#### A. Navigate To (`navigateTo()`)
```
User clicks: Google

Before:
- Current: New Tab
- Back: []
- Forward: []

Action:
- Push current page to back stack
- Update current page to Google
- Clear forward stack

After:
- Current: Google
- Back: [New Tab]
- Forward: []
```

**Code Logic:**
1. If not on home page, push current to back stack
2. Update current page
3. Clear forward stack (because navigating to new page)

#### B. Go Back (`goBack()`)
```
User clicks: Back button

Before:
- Current: Google
- Back: [New Tab]
- Forward: []

Action:
- Push current page (Google) to forward stack
- Pop from back stack (New Tab)
- Update current page

After:
- Current: New Tab
- Back: []
- Forward: [Google]
```

**LIFO Demonstration:**
- We push to forward in reverse order
- Going back multiple times shows LIFO behavior

#### C. Go Forward (`goForward()`)
```
User clicks: Forward button

Before:
- Current: New Tab
- Back: []
- Forward: [Google]

Action:
- Push current page to back stack
- Pop from forward stack
- Update current page

After:
- Current: Google
- Back: [New Tab]
- Forward: []
```

#### D. Reset Browser (`resetBrowser()`)
```
User clicks: Reset button

Action:
- Clear both stacks (top = -1)
- Reset to home page (about:blank)
- Clear all history

Result:
- Current: New Tab
- Back: []
- Forward: []
```

**Real-World Application:**
This mimics exactly how Chrome, Firefox, Edge handle browser history!

---

### 4.3 Entry Point (`Main.c`)

**File: Main.c** (CLI Application)

**Program Flow:**
```
1. Initialize browser (create stacks)
2. Display menu to user
3. Accept user input
4. Call appropriate function (navigate/back/forward/reset/display)
5. Update and display browser state
6. Repeat until user exits
```

**Menu Options:**
```
1. Navigate to Website
   - User enters URL and title
   - Calls navigateTo()
   - Displays updated history

2. Go Back
   - Calls goBack()
   - Shows previous page

3. Go Forward
   - Calls goForward()
   - Shows next page

4. Display History
   - Shows all visited pages
   - Shows back and forward stacks

5. Reset Browser
   - Calls resetBrowser()
   - Clears all history

6. Exit
   - Frees memory
   - Ends program
```

**Key Learning Points:**
- Menu-driven interface design
- User input validation
- State management
- Memory management (malloc/free)

---

### 4.4 HTTP Server (`server.c` & `server.h`)

**File: server.c** (Implementation)

**Purpose:** Connect web frontend to C backend

**Key Functions:**

#### A. Server Initialization (`startServer()`)
- Creates Windows Socket (Winsock2)
- Binds to localhost:8000
- Listens for incoming connections
- Creates global browser instance

#### B. Handle Requests (`handleRequest()`)
Routes API calls to appropriate functions:

| Endpoint | Method | Function |
|----------|--------|----------|
| `/api/navigate` | POST | `navigateTo()` |
| `/api/back` | POST | `goBack()` |
| `/api/forward` | POST | `goForward()` |
| `/api/reset` | POST | `resetBrowser()` |
| `/api/status` | GET | Returns current state |

#### C. JSON Response (`createJsonResponse()`)
Sends back current browser state:
```json
{
  "status": "success",
  "currentPage": {
    "url": "https://www.google.com",
    "title": "Google"
  },
  "backStack": [
    {"url": "about:blank", "title": "New Tab"}
  ],
  "forwardStack": [],
  "backCount": 1,
  "forwardCount": 0
}
```

**Why HTTP Server:**
- Bridges C backend with web frontend
- Demonstrates network programming
- Shows practical integration skills

---

## 5. Frontend Integration

### 5.1 Web Interface (`index.html`)
- Beautiful responsive design
- Address bar for navigation
- Quick links (Google, GitHub, etc.)
- Real-time stack visualization
- Back/Forward/Reset buttons
- Browser status dashboard

### 5.2 Styling (`styles.css`)
- Modern gradient design
- Responsive layout (works on mobile)
- Animations for stack operations
- Professional UI/UX

### 5.3 API Client (`script.js`)
- Calls C backend via HTTP
- Updates UI with server responses
- Handles all user interactions
- Shows real-time stack changes

**Key Point:** JavaScript does NOT implement stacks - it only visualizes them. All stack operations happen in C!

---

## 6. How It Works - Complete Flow

### Scenario: User navigates to 3 websites, goes back, then goes forward

```
Step 1: User navigates to Google
├─ JavaScript calls: POST /api/navigate {url: "google.com", title: "Google"}
├─ C Server receives request
├─ Calls: navigateTo(browser, "google.com", "Google")
├─ stack.c:push() adds to back stack
├─ Returns JSON with updated stacks
└─ JavaScript updates UI

Step 2: User navigates to GitHub
├─ Similar process
├─ Back stack now has: [New Tab, Google]
└─ Forward stack cleared: []

Step 3: User navigates to Stack Overflow
├─ Similar process
├─ Back stack now has: [New Tab, Google, GitHub]
└─ Forward stack cleared: []

Step 4: User clicks Back
├─ JavaScript calls: POST /api/back
├─ C Server calls: goBack(browser)
├─ stack.c:pop() removes from back stack
├─ stack.c:push() adds current to forward stack
├─ Back stack now has: [New Tab, Google]
├─ Forward stack now has: [Stack Overflow]
└─ JavaScript shows: Current page = GitHub

Step 5: User clicks Forward
├─ JavaScript calls: POST /api/forward
├─ C Server calls: goForward(browser)
├─ Back stack now has: [New Tab, Google, GitHub]
├─ Forward stack now has: []
└─ JavaScript shows: Current page = Stack Overflow

Step 6: User clicks Reset
├─ JavaScript calls: POST /api/reset
├─ C Server calls: resetBrowser(browser)
├─ Both stacks cleared
├─ Current page reset to "New Tab"
└─ All history removed
```

---

## 7. Key Highlights for Each File

### `stack.c` - Why This Matters
✓ Shows fundamental data structure knowledge
✓ Demonstrates LIFO principle
✓ Implements boundary checking (overflow/underflow)
✓ All operations are O(1) time complexity
✓ Shows proper memory management

### `navigation.c` - Why This Matters
✓ Shows real-world application of stacks
✓ Demonstrates how browsers actually work
✓ Shows complex logic using simple stack operations
✓ Includes reset functionality for complete solution
✓ Proves DSA knowledge is practical

### `Main.c` - Why This Matters
✓ Shows complete working program
✓ Menu-driven interface design
✓ User input validation
✓ Demonstrates program flow
✓ Shows how modules work together

### `server.c` - Why This Matters
✓ Shows networking knowledge (HTTP, JSON)
✓ Demonstrates integration of C backend with web
✓ Shows practical problem-solving
✓ Proves ability to build complete systems
✓ Network-based architecture

---

## 8. Data Flow Diagram

```
User Interface (index.html)
        ↓
JavaScript API Call (script.js)
        ↓
HTTP Request → POST /api/navigate
        ↓
C HTTP Server (server.c)
        ↓
Browser Navigation (navigation.c)
        ↓
Stack Operations (stack.c)
        ↓
Returns JSON with updated stacks
        ↓
JavaScript Updates UI
        ↓
User sees updated Back/Forward stacks
```

---

## 9. Compilation Instructions

**Requirements:**
- GCC compiler (MinGW for Windows)
- Winsock2 library (Windows native)
- No external dependencies needed

**Compile Command:**
```bash
gcc -o server.exe Main.c server.c navigation.c stack.c -lws2_32
```

**Run Command:**
```bash
.\server.exe
```

**Access Browser:**
```
http://127.0.0.1:8000
```

---

## 10. Testing Results

### Test Case 1: Basic Navigation
```
✓ Navigate to Google
✓ Back stack shows "New Tab"
✓ Current page shows "Google"
✓ Forward stack empty
```

### Test Case 2: Multiple Navigations
```
✓ Navigate: New Tab → Google → GitHub → Wikipedia
✓ Back stack: [New Tab, Google, GitHub]
✓ Forward stack: []
✓ Total pages visited: 4
```

### Test Case 3: Back Operation
```
✓ Click Back from Wikipedia
✓ Current page: GitHub
✓ Back stack: [New Tab, Google]
✓ Forward stack: [Wikipedia]
```

### Test Case 4: Forward Operation
```
✓ Click Forward
✓ Current page: Wikipedia
✓ Back stack: [New Tab, Google, GitHub]
✓ Forward stack: []
```

### Test Case 5: Reset Operation
```
✓ Click Reset
✓ Current page: New Tab
✓ Back stack: []
✓ Forward stack: []
✓ All history cleared
```

### Test Case 6: LIFO Verification
```
✓ Push A, B, C
✓ Pop returns C, B, A (reverse order)
✓ LIFO principle confirmed
```

---

## 11. Learning Outcomes

### Data Structures Concepts Demonstrated
✓ Stack implementation from scratch
✓ LIFO principle
✓ Push/Pop/Peek operations
✓ Boundary conditions (overflow/underflow)
✓ Time complexity analysis (O(1))

### Software Engineering Concepts
✓ Modular code design (separate files)
✓ Header files and declarations
✓ Code reusability
✓ Memory management (malloc/free)
✓ Error handling

### Web Development Concepts
✓ HTTP protocol understanding
✓ JSON data format
✓ Frontend-Backend communication
✓ RESTful API design
✓ Responsive UI design

### Practical Programming Skills
✓ Multi-file C projects
✓ Network programming (Winsock2)
✓ File I/O operations
✓ String manipulation
✓ Data serialization

---

## 12. Project Statistics

| Metric | Count |
|--------|-------|
| C Source Files | 4 |
| Header Files | 3 |
| Web Files | 3 |
| Total Lines of Code | 800+ |
| Stack Size | Max 100 items |
| API Endpoints | 5 |
| Functions | 20+ |
| Time Complexity | O(1) per operation |

---

## 13. How to Present This Project

### To Lecturer:
1. Show working application (live demo)
2. Explain stack operations (back/forward/reset)
3. Highlight real-world application
4. Show source code structure
5. Demonstrate compilation and execution

### Key Points to Emphasize:
- "Stack is used in actual browsers for history management"
- "LIFO principle - Last In First Out - demonstrated clearly"
- "All operations are O(1) time complexity"
- "Complete integration from C backend to web frontend"
- "Memory-safe implementation with boundary checking"

### Timeline:
- 2 min: Run the application live
- 3 min: Explain stack concept
- 3 min: Show code walkthrough
- 2 min: Demonstrate back/forward/reset operations

---

## 14. Files to Attach to Report

**Essential Code Files:**
1. `stack.c` - Core data structure
2. `stack.h` - Stack declarations
3. `navigation.c` - Application logic
4. `navigation.h` - Navigation structures
5. `server.c` - HTTP server
6. `Main.c` - CLI program

**Frontend Files:**
7. `index.html` - Web interface
8. `script.js` - API client

**Compilation Command:**
```
gcc -o server.exe Main.c server.c navigation.c stack.c -lws2_32
```

---

## 15. Conclusion

This project successfully demonstrates:
- ✓ Complete understanding of stack data structure
- ✓ Practical application in real-world scenario
- ✓ Integration of multiple technologies (C, HTTP, Web)
- ✓ Professional code organization
- ✓ Problem-solving approach

The Stack-Based Browser Navigation System serves as a comprehensive portfolio project showcasing both theoretical DSA knowledge and practical implementation skills.

---

## 16. References

- **Stack Data Structure:** LIFO principle, push/pop operations
- **Real-World Application:** Browser navigation history
- **HTTP Protocol:** Client-server communication
- **C Programming:** Winsock2 networking, file I/O
- **Web Technologies:** HTML, CSS, JavaScript, JSON

---

**Project Completion Date:** January 2, 2026
**Status:** Complete and Tested ✓
