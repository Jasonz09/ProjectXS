// Authentication Renderer
const { ipcRenderer } = require('electron');

const API_URL = 'http://localhost:3001/api';

// DOM Elements
const loginTab = document.querySelector('[data-tab="login"]');
const registerTab = document.querySelector('[data-tab="register"]');
const loginFormContainer = document.getElementById('login-form');
const registerFormContainer = document.getElementById('register-form');
const loginFormElement = document.getElementById('login-form-element');
const registerFormElement = document.getElementById('register-form-element');

// Tab Switching
loginTab.addEventListener('click', () => switchTab('login'));
registerTab.addEventListener('click', () => switchTab('register'));

function switchTab(tab) {
  if (tab === 'login') {
    loginTab.classList.add('active');
    registerTab.classList.remove('active');
    loginFormContainer.classList.add('active');
    registerFormContainer.classList.remove('active');
  } else {
    registerTab.classList.add('active');
    loginTab.classList.remove('active');
    registerFormContainer.classList.add('active');
    loginFormContainer.classList.remove('active');
  }
}

// Login Form Submission
loginFormElement.addEventListener('submit', async (e) => {
  e.preventDefault();
  
  const username = document.getElementById('login-username').value.trim();
  const password = document.getElementById('login-password').value;
  const errorEl = document.getElementById('login-error');
  const successEl = document.getElementById('login-success');
  
  // Clear messages
  errorEl.textContent = '';
  successEl.textContent = '';
  
  if (!username || !password) {
    errorEl.textContent = 'Please fill in all fields';
    return;
  }
  
  try {
    const response = await fetch(`${API_URL}/auth/login`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ username, password })
    });
    
    const data = await response.json();
    
    if (!response.ok) {
      errorEl.textContent = data.error || 'Login failed';
      return;
    }
    
    // Success! Store user data and navigate to main launcher
    localStorage.setItem('currentUser', JSON.stringify(data.user));
    localStorage.setItem('authToken', data.token);
    
    successEl.textContent = 'Login successful! Loading launcher...';
    
    // Navigate to main launcher after short delay
    setTimeout(() => {
      window.location.href = 'index.html';
    }, 1000);
    
  } catch (error) {
    console.error('Login error:', error);
    errorEl.textContent = 'Failed to connect to server. Please make sure the backend is running.';
  }
});

// Register Form Submission
registerFormElement.addEventListener('submit', async (e) => {
  e.preventDefault();
  
  const username = document.getElementById('register-username').value.trim();
  const email = document.getElementById('register-email').value.trim();
  const password = document.getElementById('register-password').value;
  const passwordConfirm = document.getElementById('register-password-confirm').value;
  const errorEl = document.getElementById('register-error');
  const successEl = document.getElementById('register-success');
  
  // Clear messages
  errorEl.textContent = '';
  successEl.textContent = '';
  
  // Validation
  if (!username || !password || !passwordConfirm) {
    errorEl.textContent = 'Please fill in all required fields';
    return;
  }
  
  if (username.length < 3 || username.length > 20) {
    errorEl.textContent = 'Username must be 3-20 characters';
    return;
  }
  
  if (password.length < 6) {
    errorEl.textContent = 'Password must be at least 6 characters';
    return;
  }
  
  if (password !== passwordConfirm) {
    errorEl.textContent = 'Passwords do not match';
    return;
  }
  
  try {
    const response = await fetch(`${API_URL}/auth/register`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ username, password, email })
    });
    
    const data = await response.json();
    
    if (!response.ok) {
      errorEl.textContent = data.error || 'Registration failed';
      return;
    }
    
    // Success! Store user data and navigate to main launcher
    localStorage.setItem('currentUser', JSON.stringify(data.user));
    localStorage.setItem('authToken', data.token);
    
    successEl.textContent = `Account created! Your User ID is: ${data.user.userId}`;
    
    // Navigate to main launcher after short delay
    setTimeout(() => {
      window.location.href = 'index.html';
    }, 2000);
    
  } catch (error) {
    console.error('Registration error:', error);
    errorEl.textContent = 'Failed to connect to server. Please make sure the backend is running.';
  }
});

// Google OAuth Login
document.getElementById('google-login-btn').addEventListener('click', async () => {
  const errorEl = document.getElementById('login-error');
  errorEl.textContent = '';
  
  try {
    // Send OAuth request to main process
    ipcRenderer.send('oauth-login', 'google');
  } catch (error) {
    console.error('Google OAuth error:', error);
    errorEl.textContent = 'Failed to initialize Google sign in';
  }
});

// Google OAuth Register
document.getElementById('google-register-btn').addEventListener('click', async () => {
  document.getElementById('google-login-btn').click(); // Same flow
});

// Apple OAuth Login
document.getElementById('apple-login-btn').addEventListener('click', async () => {
  const errorEl = document.getElementById('login-error');
  errorEl.textContent = '';
  
  try {
    // Send OAuth request to main process
    ipcRenderer.send('oauth-login', 'apple');
  } catch (error) {
    console.error('Apple OAuth error:', error);
    errorEl.textContent = 'Failed to initialize Apple sign in';
  }
});

// Apple OAuth Register
document.getElementById('apple-register-btn').addEventListener('click', async () => {
  document.getElementById('apple-login-btn').click(); // Same flow
});

// Listen for OAuth responses from main process
ipcRenderer.on('oauth-success', (event, data) => {
  // Check if email needs verification
  if (data.user.provider === 'google' && !data.user.emailVerified) {
    // Store temp data and show email verification
    localStorage.setItem('tempUser', JSON.stringify(data.user));
    localStorage.setItem('tempToken', data.token);
    window.location.href = 'verify-email.html';
  } else {
    // No verification needed or already verified
    localStorage.setItem('currentUser', JSON.stringify(data.user));
    localStorage.setItem('authToken', data.token);
    window.location.href = 'index.html';
  }
});

ipcRenderer.on('oauth-error', (event, error) => {
  document.getElementById('login-error').textContent = error;
  document.getElementById('register-error').textContent = error;
});

// Check if already logged in
window.addEventListener('DOMContentLoaded', () => {
  const currentUser = localStorage.getItem('currentUser');
  const authToken = localStorage.getItem('authToken');
  
  if (currentUser && authToken) {
    // Already logged in, redirect to main launcher
    window.location.href = 'index.html';
  }
});
