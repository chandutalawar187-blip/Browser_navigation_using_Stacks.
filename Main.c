#include "server.h"

// HTTP Server entry point
int startServer();

int main() {
    printf("+===============================================+\n");
    printf("| Stack-Based Browser - HTTP Server             |\n");
    printf("+===============================================+\n");
    printf("| Starting server...                            |\n");
    printf("+===============================================+\n\n");
    
    return startServer();
}