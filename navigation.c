#include "navigation.h"

// Create a new web browser
WebBrowser* createBrowser() {
    WebBrowser* browser = (WebBrowser*)malloc(sizeof(WebBrowser));
    if (browser == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    
    browser->backStack = createStack();
    browser->forwardStack = createStack();
    strcpy(browser->currentURL, "about:blank");
    strcpy(browser->currentTitle, "New Tab");
    
    return browser;
}

// Navigate to a new URL
void navigateTo(WebBrowser* browser, const char* url, const char* title) {
    if (strcmp(browser->currentURL, "about:blank") != 0) {
        // Push current page to back stack
        push(browser->backStack, browser->currentURL, browser->currentTitle);
    }
    
    // Clear forward stack when navigating to new page
    browser->forwardStack->top = -1;
    
    // Update current page
    strncpy(browser->currentURL, url, MAX_URL_LENGTH - 1);
    browser->currentURL[MAX_URL_LENGTH - 1] = '\0';
    strncpy(browser->currentTitle, title, MAX_URL_LENGTH - 1);
    browser->currentTitle[MAX_URL_LENGTH - 1] = '\0';
    
    printf("\n>>> Navigating to: %s\n", browser->currentTitle);
    printf("    URL: %s\n\n", browser->currentURL);
}

// Go back in history
int goBack(WebBrowser* browser) {
    if (isEmpty(browser->backStack)) {
        printf("Cannot go back! No previous pages in history.\n\n");
        return 0;
    }
    
    // Push current page to forward stack
    push(browser->forwardStack, browser->currentURL, browser->currentTitle);
    
    // Pop from back stack
    char url[MAX_URL_LENGTH], title[MAX_URL_LENGTH];
    pop(browser->backStack, url, title);
    
    // Update current page
    strcpy(browser->currentURL, url);
    strcpy(browser->currentTitle, title);
    
    printf("\n>>> Going back to: %s\n", browser->currentTitle);
    printf("    URL: %s\n\n", browser->currentURL);
    
    return 1;
}

// Go forward in history
int goForward(WebBrowser* browser) {
    if (isEmpty(browser->forwardStack)) {
        printf("Cannot go forward! No pages available.\n\n");
        return 0;
    }
    
    // Push current page to back stack
    push(browser->backStack, browser->currentURL, browser->currentTitle);
    
    // Pop from forward stack
    char url[MAX_URL_LENGTH], title[MAX_URL_LENGTH];
    pop(browser->forwardStack, url, title);
    
    // Update current page
    strcpy(browser->currentURL, url);
    strcpy(browser->currentTitle, title);
    
    printf("\n>>> Going forward to: %s\n", browser->currentTitle);
    printf("    URL: %s\n\n", browser->currentURL);
    
    return 1;
}

// Reset browser to initial state
void resetBrowser(WebBrowser* browser) {
    // Clear both stacks
    browser->backStack->top = -1;
    browser->forwardStack->top = -1;
    
    // Reset to home page
    strcpy(browser->currentURL, "about:blank");
    strcpy(browser->currentTitle, "New Tab");
    
    printf("\n>>> Browser Reset!\n");
    printf("    All history cleared\n\n");
}

// Display current page
void displayCurrentPage(WebBrowser* browser) {
    printf("\n+=============================================+\n");
    printf("|        CURRENT PAGE INFORMATION          |\n");
    printf("+=============================================+\n");
    printf("| Title: %-35s |\n", browser->currentTitle);
    printf("| URL:   %-35s |\n", browser->currentURL);
    printf("+=============================================+\n\n");
}

// Display browser status
void displayBrowserStatus(WebBrowser* browser) {
    printf("\n+============================================+\n");
    printf("|          BROWSER STATUS                   |\n");
    printf("+============================================+\n");
    printf("| Back History:    %d pages                  |\n", stackSize(browser->backStack));
    printf("| Forward History: %d pages                  |\n", stackSize(browser->forwardStack));
    printf("| Current Page:    %s                    |\n", browser->currentTitle);
    printf("+============================================+\n\n");
}

// Display all navigation history
void displayAllHistory(WebBrowser* browser) {
    printf("\n+============================================+\n");
    printf("|       COMPLETE NAVIGATION HISTORY         |\n");
    printf("+============================================+\n");
    
    printf("\n--- BACK STACK (History) ---\n");
    if (isEmpty(browser->backStack)) {
        printf("No back history available.\n");
    } else {
        displayStack(browser->backStack);
    }
    
    printf("\n--- FORWARD STACK (Future) ---\n");
    if (isEmpty(browser->forwardStack)) {
        printf("No forward history available.\n");
    } else {
        displayStack(browser->forwardStack);
    }
    printf("\n");
}

// Free browser memory
void freeBrowser(WebBrowser* browser) {
    if (browser != NULL) {
        freeStack(browser->backStack);
        freeStack(browser->forwardStack);
        free(browser);
    }
}
