// OAuth Helper for Electron
const { BrowserWindow } = require('electron');

class OAuthHandler {
  constructor(apiUrl) {
    this.apiUrl = apiUrl;
  }

  // Open OAuth window and handle callback
  async authenticate(provider) {
    return new Promise((resolve, reject) => {
      const authWindow = new BrowserWindow({
        width: 500,
        height: 600,
        show: true,
        webPreferences: {
          nodeIntegration: false,
          contextIsolation: true
        }
      });

      const authUrl = `${this.apiUrl}/auth/${provider}`;
      authWindow.loadURL(authUrl);

      // Listen for navigation to callback URL
      authWindow.webContents.on('will-redirect', (event, url) => {
        this.handleCallback(url, authWindow, resolve, reject);
      });

      authWindow.webContents.on('did-navigate', (event, url) => {
        this.handleCallback(url, authWindow, resolve, reject);
      });

      // Handle window close
      authWindow.on('closed', () => {
        reject(new Error('Authentication window closed'));
      });
    });
  }

  handleCallback(url, authWindow, resolve, reject) {
    // Check if this is the callback URL
    if (url.includes('/auth/callback/success')) {
      // Extract token from URL
      const urlParams = new URLSearchParams(url.split('?')[1]);
      const token = urlParams.get('token');
      const userData = urlParams.get('user');

      if (token && userData) {
        authWindow.close();
        resolve({
          token,
          user: JSON.parse(decodeURIComponent(userData))
        });
      }
    } else if (url.includes('/auth/callback/error')) {
      const urlParams = new URLSearchParams(url.split('?')[1]);
      const error = urlParams.get('error');
      authWindow.close();
      reject(new Error(error || 'Authentication failed'));
    }
  }
}

module.exports = OAuthHandler;
