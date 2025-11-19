const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const { exec } = require('child_process');
const fs = require('fs');

let mainWindow;
const API_URL = 'http://localhost:3001/api';

function createWindow() {
  mainWindow = new BrowserWindow({
    fullscreen: true, // Make app take up whole screen
    frame: false, // Frameless window for custom title bar
    transparent: false,
    backgroundColor: '#0a0e27',
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
      enableRemoteModule: true
    },
    icon: path.join(__dirname, 'assets', 'icon.png')
  });

  // Check if user is authenticated
  // Note: We'll check localStorage in the renderer process
  mainWindow.loadFile('auth.html');

  // Open DevTools in development
  // mainWindow.webContents.openDevTools();

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

// IPC Handlers
ipcMain.on('minimize-window', () => {
  mainWindow.minimize();
});

// Graceful close: replace default close handler so watchdog can detect clean exits
ipcMain.on('close-window', () => {
  if (mainWindow) {
    mainWindow.close();
  }
  // Ensure we exit with code 0 for clean shutdowns initiated by the user
  process.exitCode = 0;
  app.quit();
});

// Handle OAuth login
ipcMain.on('oauth-login', async (event, provider) => {
  try {
    const authWindow = new BrowserWindow({
      width: 500,
      height: 600,
      show: true,
      webPreferences: {
        nodeIntegration: false,
        contextIsolation: true
      }
    });

    const authUrl = `${API_URL}/auth/${provider}`;
    authWindow.loadURL(authUrl);

    // Listen for navigation to callback URL
    const handleCallback = (url) => {
      if (url.includes('/auth/callback/success')) {
        // Extract data from URL
        const urlParams = new URLSearchParams(url.split('?')[1]);
        const token = urlParams.get('token');
        const userData = urlParams.get('user');

        if (token && userData) {
          authWindow.close();
          mainWindow.webContents.send('oauth-success', {
            token,
            user: JSON.parse(decodeURIComponent(userData))
          });
        }
      } else if (url.includes('/auth/callback/error')) {
        const urlParams = new URLSearchParams(url.split('?')[1]);
        const error = urlParams.get('error');
        authWindow.close();
        mainWindow.webContents.send('oauth-error', error || 'Authentication failed');
      }
    };

    authWindow.webContents.on('will-redirect', (event, url) => {
      handleCallback(url);
    });

    authWindow.webContents.on('did-navigate', (event, url) => {
      handleCallback(url);
    });

    authWindow.on('closed', () => {
      // Window closed without completing auth
    });

  } catch (error) {
    console.error('OAuth error:', error);
    mainWindow.webContents.send('oauth-error', 'Failed to open authentication window');
  }
});

ipcMain.on('launch-game', (event, gameConfig) => {
  // Path to your Unreal game executable
  const gamePath = path.join(__dirname, '..', 'Binaries', 'Mac', 'ProjectXS.app');
  
  // Check if game exists
  if (!fs.existsSync(gamePath)) {
    event.reply('game-launch-error', 'Game executable not found. Please build the project first.');
    return;
  }

  // Send initial loading status
  event.reply('game-launching', { stage: 'initializing', progress: 0 });

  // Launch the game immediately
  const command = process.platform === 'darwin' 
    ? `open "${gamePath}"` 
    : `"${gamePath}"`;

  exec(command, (error, stdout, stderr) => {
    if (error) {
      console.error('Error launching game:', error);
      event.reply('game-launch-error', error.message);
      return;
    }
    
    console.log('Game process started');
    event.reply('game-launching', { stage: 'process-started', progress: 10 });
  });

  // Monitor game process and update progress
  let progress = 10;
  let checkCount = 0;
  const maxChecks = 60; // Check for up to 60 seconds
  
  const monitorInterval = setInterval(() => {
    checkCount++;
    
    // Check if Unreal Engine process is running
    const checkCommand = process.platform === 'darwin'
      ? 'ps aux | grep -i "ProjectXS" | grep -v grep'
      : 'tasklist | findstr "ProjectXS"';
    
    exec(checkCommand, (error, stdout, stderr) => {
      if (stdout && stdout.length > 0) {
        // Game process is running - simulate loading progress
        if (progress < 95) {
          progress += Math.random() * 5 + 2; // Increment by 2-7%
          progress = Math.min(progress, 95); // Cap at 95% until fully loaded
          
          let stage = 'loading-assets';
          let details = 'Loading game resources...';
          
          if (progress < 20) {
            stage = 'initializing';
            details = 'Starting Unreal Engine...';
          } else if (progress < 40) {
            stage = 'loading-assets';
            details = 'Loading shaders and textures...';
          } else if (progress < 60) {
            stage = 'initializing-engine';
            details = 'Initializing game systems...';
          } else if (progress < 80) {
            stage = 'loading-level';
            details = 'Loading game world...';
          } else {
            stage = 'starting-game';
            details = 'Almost ready...';
          }
          
          event.reply('game-launching', { 
            stage: stage, 
            progress: Math.floor(progress),
            details: details
          });
        }
      }
      
      // Check if game window is visible (indicates fully loaded)
      if (checkCount > 10) { // Give it at least 10 seconds
        exec('ps aux | grep -i "ProjectXS" | grep -v grep | wc -l', (err, count) => {
          if (count && parseInt(count.trim()) > 0 && progress >= 50) {
            // Game is running and has had time to load
            progress = 100;
            event.reply('game-launching', { 
              stage: 'complete', 
              progress: 100,
              details: 'Game ready!'
            });
            event.reply('game-launched');
            clearInterval(monitorInterval);
          }
        });
      }
      
      // Timeout after max checks
      if (checkCount >= maxChecks) {
        console.log('Monitor timeout - assuming game loaded');
        progress = 100;
        event.reply('game-launching', { 
          stage: 'complete', 
          progress: 100,
          details: 'Game ready!'
        });
        event.reply('game-launched');
        clearInterval(monitorInterval);
      }
    });
  }, 1000); // Check every second
});

// Handle settings save
ipcMain.on('save-settings', (event, settings) => {
  const settingsPath = path.join(app.getPath('userData'), 'settings.json');
  fs.writeFileSync(settingsPath, JSON.stringify(settings, null, 2));
  event.reply('settings-saved');
});

// Handle settings load
ipcMain.on('load-settings', (event) => {
  const settingsPath = path.join(app.getPath('userData'), 'settings.json');
  
  if (fs.existsSync(settingsPath)) {
    const settings = JSON.parse(fs.readFileSync(settingsPath, 'utf8'));
    event.reply('settings-loaded', settings);
  } else {
    // Default settings
    event.reply('settings-loaded', {
      selectedCharacter: 0,
      volume: 50,
      graphics: 'high'
    });
  }
});

// Log and set non-zero exitCode on uncaught exceptions so watchdog restarts on crashes
process.on('uncaughtException', (err) => {
  console.error('Uncaught exception in launcher:', err);
  // Set a non-zero exit code so the watchdog will attempt a restart
  process.exitCode = 1;
  // Allow process to exit naturally (watchdog will restart)
});
