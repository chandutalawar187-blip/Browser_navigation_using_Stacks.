#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "stack.h"

#define MAX_WEBSITES 20

// Browser structure
typedef struct {
    Stack* backStack;           // Back navigation history
    Stack* forwardStack;        // Forward navigation history
    char currentURL[MAX_URL_LENGTH];
    char currentTitle[MAX_URL_LENGTH];
} WebBrowser;

// Function declarations
WebBrowser* createBrowser();
void navigateTo(WebBrowser* browser, const char* url, const char* title);
int goBack(WebBrowser* browser);
int goForward(WebBrowser* browser);
void resetBrowser(WebBrowser* browser);
void displayCurrentPage(WebBrowser* browser);
void displayBrowserStatus(WebBrowser* browser);
void displayAllHistory(WebBrowser* browser);
void freeBrowser(WebBrowser* browser);

#endif
