#include "stack.h"

// Create a new stack
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    stack->top = -1;
    return stack;
}

// Check if stack is full
int isFull(Stack* stack) {
    return stack->top == MAX_STACK_SIZE - 1;
}

// Check if stack is empty
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// Push element to stack
void push(Stack* stack, const char* url, const char* title) {
    if (isFull(stack)) {
        printf("Stack Overflow! Cannot add more pages.\n");
        return;
    }
    
    stack->top++;
    strncpy(stack->items[stack->top].url, url, MAX_URL_LENGTH - 1);
    stack->items[stack->top].url[MAX_URL_LENGTH - 1] = '\0';
    strncpy(stack->items[stack->top].title, title, MAX_URL_LENGTH - 1);
    stack->items[stack->top].title[MAX_URL_LENGTH - 1] = '\0';
    
    printf("[LOG] Pushed: %s - %s\n", url, title);
}

// Pop element from stack
int pop(Stack* stack, char* url, char* title) {
    if (isEmpty(stack)) {
        printf("Stack Underflow! No pages to go back.\n");
        return 0;
    }
    
    strcpy(url, stack->items[stack->top].url);
    strcpy(title, stack->items[stack->top].title);
    stack->top--;
    
    return 1;
}

// Peek at top element without removing
int peek(Stack* stack, char* url, char* title) {
    if (isEmpty(stack)) {
        return 0;
    }
    
    strcpy(url, stack->items[stack->top].url);
    strcpy(title, stack->items[stack->top].title);
    
    return 1;
}

// Get current stack size
int stackSize(Stack* stack) {
    return stack->top + 1;
}

// Display all items in stack
void displayStack(Stack* stack) {
    if (isEmpty(stack)) {
        printf("History is empty!\n");
        return;
    }
    
    printf("\n================================================\n");
    printf("Position | URL                          | Title\n");
    printf("================================================\n");
    
    for (int i = stack->top; i >= 0; i--) {
        printf("%d        | %-28s | %s\n", i + 1, 
               stack->items[i].url, stack->items[i].title);
    }
    printf("================================================\n\n");
}

// Free stack memory
void freeStack(Stack* stack) {
    if (stack != NULL) {
        free(stack);
    }
}
