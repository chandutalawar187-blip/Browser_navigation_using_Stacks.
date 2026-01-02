// API Configuration
const API_BASE_URL = 'http://localhost:8000';

// Make API call to C backend
async function apiCall(endpoint, method = 'GET', data = null) {
    try {
        const options = {
            method: method,
            headers: {
                'Content-Type': 'application/json',
            }
        };

        if (data) {
            options.body = JSON.stringify(data);
        }

        const response = await fetch(`${API_BASE_URL}${endpoint}`, options);
        return await response.json();
    } catch (error) {
        console.error('API Error:', error);
        return { status: 'error', message: 'Server connection failed' };
    }
}

// Browser Navigation System - Now using C Backend
class StackBrowser {
    constructor() {
        // State is now managed on C server
        this.currentURL = 'about:blank';
        this.currentTitle = 'New Tab';
        this.totalVisited = 0;
        this.initializeEventListeners();
        this.addLog('Connected to C Backend Server');
        this.addLog('All stack operations running in C');
    }

    initializeEventListeners() {
        document.getElementById('navigateBtn').addEventListener('click', () => this.handleNavigate());
        document.getElementById('urlInput').addEventListener('keypress', (e) => {
            if (e.key === 'Enter') this.handleNavigate();
        });
        document.getElementById('backBtn').addEventListener('click', () => this.goBack());
        document.getElementById('forwardBtn').addEventListener('click', () => this.goForward());
        document.getElementById('refreshBtn').addEventListener('click', () => this.refresh());
        document.getElementById('resetBtn').addEventListener('click', () => this.reset());
    }

    async handleNavigate() {
        const urlInput = document.getElementById('urlInput');
        const url = urlInput.value.trim();

        if (!url) {
            alert('Please enter a URL');
            return;
        }

        let title = url.includes('://') 
            ? new URL(url).hostname 
            : url;
        
        await this.navigateTo(url, title);
    }

    async navigateTo(url, title) {
        // Call C backend API
        const response = await apiCall('/api/navigate', 'POST', { url, title });

        if (response.status === 'success') {
            this.currentURL = response.currentPage.url;
            this.currentTitle = response.currentPage.title;
            this.totalVisited++;

            document.getElementById('urlInput').value = url;
            this.updateDisplay(response);
            this.addLog(`✓ [C Backend] Navigated to: ${title} (${url})`);
        } else {
            this.addLog(`✗ [C Backend] Navigation failed: ${response.message}`);
        }
    }

    async goBack() {
        const response = await apiCall('/api/back', 'POST');

        if (response.status === 'success') {
            this.currentURL = response.currentPage.url;
            this.currentTitle = response.currentPage.title;

            document.getElementById('urlInput').value = this.currentURL;
            this.updateDisplay(response);
            this.addLog(`← [C Backend] Went back to: ${this.currentTitle}`);
        } else {
            this.addLog(`✗ [C Backend] Cannot go back - no history`);
        }
    }

    async goForward() {
        const response = await apiCall('/api/forward', 'POST');

        if (response.status === 'success') {
            this.currentURL = response.currentPage.url;
            this.currentTitle = response.currentPage.title;

            document.getElementById('urlInput').value = this.currentURL;
            this.updateDisplay(response);
            this.addLog(`→ [C Backend] Went forward to: ${this.currentTitle}`);
        } else {
            this.addLog(`✗ [C Backend] Cannot go forward - no pages`);
        }
    }

    refresh() {
        this.addLog(`🔄 [C Backend] Refreshed: ${this.currentTitle}`);
    }

    async reset() {
        const confirm = window.confirm('Are you sure you want to reset all browsing history? This action cannot be undone.');
        
        if (confirm) {
            const response = await apiCall('/api/reset', 'POST');
            
            if (response.status === 'success') {
                this.updateDisplay(response);
                this.addLog('🔄 [C Backend] Browser reset - all history cleared!');
            } else {
                this.addLog('✗ [C Backend] Reset failed');
            }
        }
    }

    updateDisplay(data) {
        // Update current page
        document.getElementById('currentTitle').textContent = data.currentPage.title;
        document.getElementById('currentURL').textContent = data.currentPage.url;

        // Update button states
        const hasBackStack = data.backCount > 0;
        const hasForwardStack = data.forwardCount > 0;
        document.getElementById('backBtn').disabled = !hasBackStack;
        document.getElementById('forwardBtn').disabled = !hasForwardStack;

        // Update stacks display
        this.updateStackDisplay('backStack', data.backStack, data.backCount);
        this.updateStackDisplay('forwardStack', data.forwardStack, data.forwardCount);

        // Update status
        this.updateStatus(data);
    }

    updateStackDisplay(stackId, stackItems, count) {
        const stackDiv = document.getElementById(stackId);
        const emptyDiv = document.getElementById(stackId === 'backStack' ? 'backEmpty' : 'forwardEmpty');
        const countDiv = document.getElementById(stackId === 'backStack' ? 'backCount' : 'forwardCount');

        // Clear previous items
        stackDiv.innerHTML = '';

        if (!stackItems || stackItems.length === 0) {
            stackDiv.style.display = 'none';
            emptyDiv.style.display = 'flex';
            countDiv.textContent = '0';
        } else {
            stackDiv.style.display = 'flex';
            emptyDiv.style.display = 'none';
            countDiv.textContent = count;

            // Display stack items
            stackItems.forEach(item => {
                const itemDiv = document.createElement('div');
                itemDiv.className = 'stack-item';
                itemDiv.innerHTML = `
                    <div class="stack-item-title">${this.truncate(item.title, 40)}</div>
                    <div class="stack-item-url">${this.truncate(item.url, 50)}</div>
                `;
                stackDiv.appendChild(itemDiv);
            });
        }
    }

    updateStatus(data) {
        document.getElementById('statusBack').textContent = `${data.backCount} page${data.backCount !== 1 ? 's' : ''}`;
        document.getElementById('statusForward').textContent = `${data.forwardCount} page${data.forwardCount !== 1 ? 's' : ''}`;
        document.getElementById('statusTotal').textContent = `${this.totalVisited} page${this.totalVisited !== 1 ? 's' : ''}`;
    }

    addLog(message) {
        const logDiv = document.getElementById('historyLog');
        const entry = document.createElement('p');
        entry.className = 'log-entry';

        // Determine log type for styling
        if (message.includes('←')) entry.classList.add('back');
        else if (message.includes('→')) entry.classList.add('forward');
        else if (message.includes('✓')) entry.classList.add('navigate');

        const timestamp = new Date().toLocaleTimeString();
        entry.textContent = `[${timestamp}] ${message}`;

        logDiv.insertBefore(entry, logDiv.firstChild);

        // Keep only last 50 entries
        while (logDiv.children.length > 50) {
            logDiv.removeChild(logDiv.lastChild);
        }
    }

    truncate(str, length) {
        return str.length > length ? str.substring(0, length) + '...' : str;
    }
}

// Global browser instance
let browser;

// Initialize when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    browser = new StackBrowser();
    
    // Check if server is running
    apiCall('/api/status', 'POST').then(response => {
        if (response.status === 'success') {
            browser.updateDisplay(response);
        } else {
            browser.addLog('⚠️ C Backend Server not running!');
            browser.addLog('Start the server: gcc -o server server_main.c server.c stack.c navigation.c && ./server');
        }
    });
});

// Expose navigateTo for quick links
async function navigateTo(url, title) {
    await browser.navigateTo(url, title);
}
