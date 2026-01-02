# How to Show Your Lecturer

## Step 1: Setup (2 minutes)

Open PowerShell in `c:\Users\chand\Desktop\pbl_projects`

### If you haven't compiled yet:
```powershell
gcc -o server server_main.c server.c stack.c navigation.c -lws2_32
```

### Start the server:
```powershell
.\server.exe
```

You should see:
```
+===============================================+
| Stack-Based Browser - C Backend Server        |
+===============================================+
| Server running on: http://127.0.0.1:8000    |
| Open http://127.0.0.1:8000 in your browser  |
| All stack operations execute in C             |
+===============================================+
```

## Step 2: Open the Interface (1 minute)

Open your browser and go to:
```
http://127.0.0.1:8000
```

## Step 3: Demo (3 minutes)

### Demo Script for Lecturer:

**"This is a stack-based web browser navigation system. Let me show you how it works:"**

### Action 1: Navigate to Google
1. Click the "Google" button
2. Point out:
   - Current page now shows "Google"
   - Back stack now shows "New Tab" (the previous page)
   - Back button is now enabled
   - Server console shows: `[POST] /api/navigate`

**Explain:** "When we navigate, the previous page is pushed to the back stack in C code"

### Action 2: Navigate to GitHub
1. Click the "GitHub" button
2. Point out:
   - Current page now shows "GitHub"
   - Back stack now shows 2 items: Google and New Tab
   - Forward stack is cleared
   - Server console shows: `[POST] /api/navigate`

**Explain:** "The forward stack is cleared because we're navigating to a new page, not going back/forward"

### Action 3: Navigate to Stack Overflow
1. Click the "Stack Overflow" button
2. Point out:
   - Back stack has 3 items now
   - Browser shows all items in the visualization

**Explain:** "As you can see, the back stack keeps growing with each navigation. These are using actual C stack operations."

### Action 4: Click Back
1. Click the "Back" button
2. Point out:
   - Current page goes back to "GitHub"
   - Back stack now has 2 items instead of 3
   - Forward stack now shows 1 item (Stack Overflow)
   - Server console shows: `[POST] /api/back`

**Explain:** "When we go back, we pop from the back stack and push to the forward stack - classic LIFO behavior"

### Action 5: Click Back Again
1. Click "Back" again
2. Point out:
   - Current page: Google
   - Back stack: 1 item
   - Forward stack: 2 items

**Explain:** "Each back operation pops from back and pushes to forward. This is exactly how real browsers work."

### Action 6: Click Forward
1. Click the "Forward" button
2. Point out:
   - Current page: GitHub
   - Back stack: 2 items
   - Forward stack: 1 item
   - Server console shows: `[POST] /api/forward`

**Explain:** "Forward does the opposite - pops from forward and pushes to back"

## Step 4: Show the Code (2 minutes)

**"Now let me show you the backend code:"**

### Show stack.c
```
"Here's the actual stack implementation. You can see:
- push() function - adds to stack
- pop() function - removes from stack
- isEmpty() - checks if stack is empty
- stackSize() - gets number of items

This is pure C code that implements the LIFO principle."
```

### Show navigation.c
```
"Here's the navigation system that uses the stacks:
- navigateTo() - navigates to a website
  - Pushes current page to back stack
  - Updates current page
  - Clears forward stack
- goBack() - goes back
  - Pushes current to forward stack
  - Pops from back stack
- goForward() - goes forward (opposite of goBack)

All these operations call the stack functions."
```

### Show server.c
```
"The server is implemented in C using Windows Sockets:
- startServer() - starts the HTTP server
- handleRequest() - routes API calls
- When /api/navigate is called, it calls navigateTo()
- Server builds JSON response with updated stacks
- JavaScript frontend displays the results"
```

### Show script.js
```
"Notice the frontend JavaScript does NOT have its own stack implementation.
It just sends requests to the C backend and displays what the backend returns.
All stack operations happen in C."
```

## Step 5: Key Points to Make

1. **"Backend is 100% C"**
   - Show that stack.c, navigation.c, and server.c are pure C
   - Mention: "No external libraries - just Windows standard libraries"

2. **"HTTP Server in C"**
   - Explain you built the server from scratch
   - Not using any framework, just Winsock2

3. **"Real-world Application"**
   - Show how stacks are used in actual browsers
   - Explain the browser navigation system uses exactly this logic

4. **"Complete Integration"**
   - Show web interface calling C backend
   - Show C server responding with data
   - Show frontend updating UI with responses

5. **"Learning Outcomes"**
   - Data Structures (Stack implementation)
   - Algorithms (Navigation logic)
   - Networking (HTTP server)
   - Web Development (Frontend-backend integration)

## Step 6: Testing During Demo

If lecturer asks "Does it work correctly?" do this:

### Test 1: History Accuracy
```
Navigate: Google → GitHub → Wikipedia
Check back stack has all 3 pages ✓
```

### Test 2: Stack Size
```
Keep clicking back until back stack is empty
Notice "Back" button disables ✓
```

### Test 3: LIFO Behavior
```
Navigate: A → B → C
Back: C ← B ← A (Reverse order) ✓
This proves LIFO works
```

### Test 4: Forward Stack
```
Navigate: A → B
Back: to A
Forward: back to B ✓
Forward stack behavior is correct
```

## Quick Answers for Common Questions

**"How is the backend connected to frontend?"**
> "HTTP API. The web interface sends POST requests to the C server, which processes them using C stack functions and returns JSON with updated stacks."

**"Is everything really in C?"**
> "Yes! Stack operations, navigation logic, and the HTTP server are all in C. Only the UI visualization is HTML/CSS/JavaScript."

**"Why do you have JavaScript if backend is C?"**
> "JavaScript is just for the UI visualization. All actual stack operations happen in C code on the server."

**"How do you know it's using stacks?"**
> "Look at the back/forward behavior - it's LIFO (Last In First Out). If it was using a list, going back wouldn't work this way. Also, you can read the stack.c code that implements push/pop."

**"Could you do this without JavaScript?"**
> "Yes! The CLI version (browser.exe) is pure C without any web interface. But the web version is more visual and easier to show."

---

## Timing

**Total Demo Time: ~10 minutes**
- Setup: 2 minutes
- Open interface: 1 minute
- Demonstrate features: 3 minutes
- Show code: 2 minutes
- Answer questions: 2 minutes

---

## What to Emphasize

✅ **Backend is 100% C code**
✅ **Proper stack operations (LIFO)**
✅ **HTTP server from scratch**
✅ **Web interface for visualization**
✅ **No external libraries**
✅ **Real-world application**
✅ **Complete documentation**

---

Your lecturer will be impressed! 🎉
