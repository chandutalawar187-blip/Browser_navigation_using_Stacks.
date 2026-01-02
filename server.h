#ifndef SERVER_H
#define SERVER_H

#include "navigation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8000
#define MAX_BUFFER 8192

// Function declarations
int startServer();
void handleClient(SOCKET clientSocket);
char* handleRequest(const char* request, WebBrowser* browser);
char* createJsonResponse(const char* status, const char* message, WebBrowser* browser);
char* getHttpResponse(const char* jsonBody);
void closeServer();
void extractJsonValue(const char* json, const char* key, char* value, int maxLen);

#endif
