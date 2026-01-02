#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAX_URL_LENGTH 256

// Stack node structure
typedef struct {
    char url[MAX_URL_LENGTH];
    char title[MAX_URL_LENGTH];
} StackItem;

// Stack structure
typedef struct {
    StackItem items[MAX_STACK_SIZE];
    int top;
} Stack;

// Function declarations
Stack* createStack();
int isFull(Stack* stack);
int isEmpty(Stack* stack);
void push(Stack* stack, const char* url, const char* title);
int pop(Stack* stack, char* url, char* title);
int peek(Stack* stack, char* url, char* title);
int stackSize(Stack* stack);
void displayStack(Stack* stack);
void freeStack(Stack* stack);

#endif
