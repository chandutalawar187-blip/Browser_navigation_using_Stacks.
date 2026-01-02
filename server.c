#include "server.h"

// Global variables
WSADATA wsaData;
SOCKET serverSocket = INVALID_SOCKET;
WebBrowser* globalBrowser = NULL;

// Extract JSON string value
void extractJsonValue(const char* json, const char* key, char* value, int maxLen) {
    char searchKey[256];
    sprintf(searchKey, "\"%s\":\"", key);
    
    const char* start = strstr(json, searchKey);
    if (start == NULL) {
        strcpy(value, "");
        return;
    }
    
    start += strlen(searchKey);
    int len = 0;
    
    while (*start != '"' && len < maxLen - 1 && *start != '\0') {
        value[len++] = *start++;
    }
    value[len] = '\0';
}

// Start HTTP server
int startServer() {
    // Initialize Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        printf("socket failed: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Allow socket reuse
    int reuseAddr = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseAddr, sizeof(reuseAddr));

    // Set up server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    result = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        printf("bind failed: %ld\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for connections
    result = listen(serverSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        printf("listen failed: %ld\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("+===============================================+\n");
    printf("| Stack-Based Browser - C Backend Server        |\n");
    printf("+===============================================+\n");
    printf("| Server running on: http://127.0.0.1:%d      |\n", PORT);
    printf("| Open http://127.0.0.1:%d in your browser   |\n", PORT);
    printf("| All stack operations execute in C             |\n");
    printf("+===============================================+\n\n");

    // Create global browser instance
    globalBrowser = createBrowser();
    if (globalBrowser == NULL) {
        printf("Failed to create browser!\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Accept connections
    while (1) {
        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);

        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            printf("accept failed: %ld\n", WSAGetLastError());
            continue;
        }

        handleClient(clientSocket);
        closesocket(clientSocket);
    }

    return 0;
}

// Handle individual client request
void handleClient(SOCKET clientSocket) {
    char buffer[MAX_BUFFER];
    int bytesReceived = recv(clientSocket, buffer, MAX_BUFFER - 1, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';

        // Parse HTTP request
        char method[10], path[256], protocol[20];
        sscanf(buffer, "%s %s %s", method, path, protocol);

        printf("[%s] %s\n", method, path);

        char* response = handleRequest(buffer, globalBrowser);

        // Send response
        send(clientSocket, response, (int)strlen(response), 0);

        free(response);
    }
}

// Route and handle API requests
char* handleRequest(const char* request, WebBrowser* browser) {
    char path[256] = {0};
    char body[MAX_BUFFER] = {0};
    
    // Parse request line
    sscanf(request, "%*s %255s", path);

    // Extract POST body if present
    const char* bodyStart = strstr(request, "\r\n\r\n");
    if (bodyStart) {
        strncpy(body, bodyStart + 4, MAX_BUFFER - 1);
    }

    // CORS preflight
    if (strstr(path, "/api/") != NULL && strstr(request, "OPTIONS") != NULL) {
        return getHttpResponse("");
    }

    // Route handling
    if (strstr(path, "/api/navigate") != NULL) {
        char url[MAX_URL_LENGTH] = {0};
        char title[MAX_URL_LENGTH] = {0};
        
        extractJsonValue(body, "url", url, MAX_URL_LENGTH);
        extractJsonValue(body, "title", title, MAX_URL_LENGTH);

        if (strlen(url) > 0 && strlen(title) > 0) {
            navigateTo(browser, url, title);
            return createJsonResponse("success", "Navigated successfully", browser);
        }
        return getHttpResponse("{\"status\":\"error\",\"message\":\"Invalid URL or title\"}");
    }
    else if (strstr(path, "/api/back") != NULL) {
        if (goBack(browser)) {
            return createJsonResponse("success", "Went back", browser);
        }
        return getHttpResponse("{\"status\":\"error\",\"message\":\"Cannot go back\"}");
    }
    else if (strstr(path, "/api/forward") != NULL) {
        if (goForward(browser)) {
            return createJsonResponse("success", "Went forward", browser);
        }
        return getHttpResponse("{\"status\":\"error\",\"message\":\"Cannot go forward\"}");
    }
    else if (strstr(path, "/api/status") != NULL) {
        return createJsonResponse("success", "Status retrieved", browser);
    }
    else if (strstr(path, "/api/reset") != NULL) {
        resetBrowser(browser);
        return createJsonResponse("success", "Browser reset successfully", browser);
    }
    else if (strstr(path, "/index.html") != NULL || strcmp(path, "/") == 0) {
        FILE* file = fopen("index.html", "rb");
        if (file == NULL) {
            return getHttpResponse("<html><body><h1>File not found</h1></body></html>");
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* fileContent = (char*)malloc(fileSize + 1);
        fread(fileContent, 1, fileSize, file);
        fileContent[fileSize] = '\0';
        fclose(file);

        char* response = (char*)malloc(fileSize + 500);
        sprintf(response, "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "Content-Length: %ld\r\n"
                         "Connection: close\r\n"
                         "\r\n%s", fileSize, fileContent);

        free(fileContent);
        return response;
    }
    else if (strstr(path, "/styles.css") != NULL) {
        FILE* file = fopen("styles.css", "rb");
        if (file == NULL) {
            return getHttpResponse("");
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* fileContent = (char*)malloc(fileSize + 1);
        fread(fileContent, 1, fileSize, file);
        fileContent[fileSize] = '\0';
        fclose(file);

        char* response = (char*)malloc(fileSize + 500);
        sprintf(response, "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/css\r\n"
                         "Content-Length: %ld\r\n"
                         "Connection: close\r\n"
                         "\r\n%s", fileSize, fileContent);

        free(fileContent);
        return response;
    }
    else if (strstr(path, "/script.js") != NULL) {
        FILE* file = fopen("script.js", "rb");
        if (file == NULL) {
            return getHttpResponse("");
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* fileContent = (char*)malloc(fileSize + 1);
        fread(fileContent, 1, fileSize, file);
        fileContent[fileSize] = '\0';
        fclose(file);

        char* response = (char*)malloc(fileSize + 500);
        sprintf(response, "HTTP/1.1 200 OK\r\n"
                         "Content-Type: application/javascript\r\n"
                         "Content-Length: %ld\r\n"
                         "Connection: close\r\n"
                         "\r\n%s", fileSize, fileContent);

        free(fileContent);
        return response;
    }

    return getHttpResponse("{\"status\":\"error\",\"message\":\"Not found\"}");
}

// Create JSON response with current browser state
char* createJsonResponse(const char* status, const char* message, WebBrowser* browser) {
    char buffer[8192] = {0};
    int pos = 0;

    // Start JSON
    pos += sprintf(buffer + pos, "{\"status\":\"%s\",\"message\":\"%s\",\"currentPage\":{\"title\":\"%s\",\"url\":\"%s\"},",
                   status, message, browser->currentTitle, browser->currentURL);

    // Add back stack
    pos += sprintf(buffer + pos, "\"backStack\":[");
    for (int i = browser->backStack->top; i >= 0; i--) {
        if (i < browser->backStack->top) pos += sprintf(buffer + pos, ",");
        pos += sprintf(buffer + pos, "{\"title\":\"%s\",\"url\":\"%s\"}",
                      browser->backStack->items[i].title, browser->backStack->items[i].url);
    }
    pos += sprintf(buffer + pos, "],");

    // Add forward stack
    pos += sprintf(buffer + pos, "\"forwardStack\":[");
    for (int i = browser->forwardStack->top; i >= 0; i--) {
        if (i < browser->forwardStack->top) pos += sprintf(buffer + pos, ",");
        pos += sprintf(buffer + pos, "{\"title\":\"%s\",\"url\":\"%s\"}",
                      browser->forwardStack->items[i].title, browser->forwardStack->items[i].url);
    }
    pos += sprintf(buffer + pos, "],");

    // Add counts
    pos += sprintf(buffer + pos, "\"backCount\":%d,\"forwardCount\":%d}",
                   stackSize(browser->backStack), stackSize(browser->forwardStack));

    return getHttpResponse(buffer);
}

// Wrap JSON in HTTP response
char* getHttpResponse(const char* jsonBody) {
    size_t contentLen = strlen(jsonBody);
    char* response = (char*)malloc(contentLen + 400);

    sprintf(response, "HTTP/1.1 200 OK\r\n"
                     "Content-Type: application/json\r\n"
                     "Content-Length: %zu\r\n"
                     "Access-Control-Allow-Origin: *\r\n"
                     "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
                     "Access-Control-Allow-Headers: Content-Type\r\n"
                     "Connection: close\r\n"
                     "\r\n%s", contentLen, jsonBody);

    return response;
}

// Close server
void closeServer() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    if (globalBrowser != NULL) {
        freeBrowser(globalBrowser);
    }
    WSACleanup();
}
