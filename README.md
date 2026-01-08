# Stack-Based Web Browser Navigation - DSA Project

## Project Overview
A complete **Data Structures & Algorithms** project implementing stack-based web browser navigation. The backend is **100% C code** that handles all stack operations, connected to a beautiful web frontend.

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Web Browser Interface                     │
│              (HTML/CSS/JavaScript Frontend)                  │
└────────────────────┬────────────────────────────────────────┘
                     │ HTTP API Calls
                     │ (localhost:8000)
                     ▼
┌─────────────────────────────────────────────────────────────┐
│              C Backend HTTP Server                           │
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  Stack API   │  │  Navigation  │  │  File Server │     │
│  │ /api/*       │  │   Routes     │  │   HTML/CSS   │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
└────────────────┬───────────────────────────────────────────┘
                 │
    ┌────────────┴────────────┐
    ▼                         ▼
┌──────────────┐      ┌──────────────┐
│  Stack Data  │      │  Navigation  │
│ Structure    │      │   Logic      │
│ (stack.c)    │      │ (navigation.c)
└──────────────┘      └──────────────┘
```

## Files Description

### Backend (C Code - 100% C Implementation)
- **stack.h / stack.c**: Pure stack data structure implementation
  - Push, Pop, Peek operations
  - Boundary checking (max 100 pages)
  - URL and title storage
  
- **navigation.h / navigation.c**: Browser navigation system
  - WebBrowser structure with back/forward stacks
  - navigateTo(), goBack(), goForward() functions
  - Stack manipulation for history management

- **server.h / server.c**: HTTP Server in C
  - Windows Sockets (Winsock2) implementation
  - JSON response generation (no external libraries)
  - API endpoints for all operations
  - File serving for HTML/CSS/JS

- **server_main.c**: Server entry point

### Frontend (Web Interface)
- **index.html**: Beautiful responsive UI
  - Browser address bar with navigation buttons
  - Visual stack representations
  - Quick links to popular websites
  - Real-time status dashboard
  - Navigation history log

- **styles.css**: Modern styling
  - Gradient backgrounds
  - Smooth animations
  - Responsive design

- **script.js**: Frontend logic
  - API client for C backend
  - UI updates from server responses
  - Event handling

## Compilation & Execution

### Windows (PowerShell)

**Step 1: Compile the C Server**
```powershell
cd c:\Users\chand\Desktop\pbl_projects
gcc -o server server_main.c server.c stack.c navigation.c -lws2_32
```

**Step 2: Run the Server**
```powershell
.\server.exe
```

Output should show:
```
+===============================================+
| Stack-Based Browser - C Backend Server        |
+===============================================+
| Server running on: http://127.0.0.1:8000    |
| Open http://127.0.0.1:8000 in your browser  |
| All stack operations execute in C             |
+===============================================+
```

**Step 3: Open Browser**
Open your web browser and navigate to:
```
http://127.0.0.1:8000
```

## How It Works

### Stack Operations in C

1. **Navigate to Website**
   - Frontend sends: `POST /api/navigate {url, title}`
   - C backend: Pushes current page to backStack, updates currentPage
   - Response: JSON with updated stacks

2. **Go Back**
   - Frontend sends: `POST /api/back`
   - C backend: Pops from backStack → pushes to forwardStack
   - Response: JSON with updated state

3. **Go Forward**
   - Frontend sends: `POST /api/forward`
   - C backend: Pops from forwardStack → pushes to backStack
   - Response: JSON with updated state

### Data Flow Example

```
User clicks "Google" quick link
    ↓
JavaScript: apiCall('/api/navigate', 'POST', {url: 'https://www.google.com', title: 'Google'})
    ↓
C Server receives POST request
    ↓
server.c: extractJsonValue() parses URL and title
    ↓
navigation.c: navigateTo() executes:
    - push(backStack, currentURL, currentTitle)  [C stack operation]
    - currentURL = url
    - currentTitle = title
    - clear(forwardStack)
    ↓
server.c: createJsonResponse() builds JSON response with updated stacks
    ↓
JavaScript receives response and updates UI
    ↓
Browser displays:
    - Current page info
    - Updated back stack visualization
    - Updated forward stack visualization
    - Status dashboard
```

## Features

✅ **Pure C Backend** - All stack operations in C (stack.c, navigation.c)
✅ **HTTP Server in C** - No external frameworks, native Windows Sockets
✅ **Zero External Dependencies** - Only Windows standard libraries
✅ **JSON Generation** - Manual JSON building (no external libraries)
✅ **Beautiful UI** - Modern responsive web interface
✅ **Real-time Updates** - Instant visualization of stack operations
✅ **Full LIFO Implementation** - Proper stack data structure
✅ **History Management** - Back/Forward stacks working together
✅ **CORS Enabled** - Browser can communicate with server
✅ **File Serving** - Server serves HTML/CSS/JS files

## Technical Details

### Stack Implementation (C)
```c
typedef struct {
    StackItem items[MAX_STACK_SIZE];
    int top;
} Stack;

void push(Stack* stack, const char* url, const char* title);
int pop(Stack* stack, char* url, char* title);
```

### Browser Navigation (C)
```c
typedef struct {
    Stack* backStack;
    Stack* forwardStack;
    char currentURL[MAX_URL_LENGTH];
    char currentTitle[MAX_URL_LENGTH];
} WebBrowser;

void navigateTo(WebBrowser* browser, const char* url, const char* title);
int goBack(WebBrowser* browser);
int goForward(WebBrowser* browser);
```

### API Communication
```javascript
// Frontend API call
apiCall('/api/navigate', 'POST', {
    url: 'https://example.com',
    title: 'Example'
})

// Server response format
{
    "status": "success",
    "message": "Navigated successfully",
    "currentPage": {"title": "Example", "url": "https://example.com"},
    "backStack": [...],
    "forwardStack": [...],
    "backCount": 5,
    "forwardCount": 0
}
```

## Project Structure

```
pbl_projects/
├── C Backend (100% C)
│   ├── stack.h / stack.c          (Stack data structure)
│   ├── navigation.h / navigation.c (Navigation logic)
│   ├── server.h / server.c         (HTTP Server)
│   └── server_main.c               (Entry point)
│
├── Web Frontend
│   ├── index.html                  (UI markup)
│   ├── styles.css                  (Styling)
│   └── script.js                   (Frontend logic)
│
├── CLI Version (Optional)
│   └── Main.c                      (Terminal-based interface)
│
└── README.md                        (This file)
```

## Learning Outcomes

This project demonstrates:
1. **Stack Data Structure** - LIFO principle implementation
2. **Browser Navigation** - Real-world stack application
3. **Network Programming** - HTTP server in C (Winsock2)
4. **Frontend-Backend Integration** - Web interface with C backend
5. **JSON Handling** - Manual JSON parsing and generation
6. **Memory Management** - Dynamic allocation in C
7. **API Design** - RESTful API in C

## Troubleshooting

**Error: "Cannot find gcc"**
- Install MinGW-w64 or GCC for Windows

**Error: "Address already in use"**
- Port 8000 is already in use
- Kill other processes or modify PORT in server.h

**Cannot connect to server**
- Make sure server.exe is running
- Check firewall settings
- Verify localhost:8000 is accessible

**Stack operations not working**
- Check C backend server logs
- Verify JSON format in API calls
- Check browser console for errors (F12)
