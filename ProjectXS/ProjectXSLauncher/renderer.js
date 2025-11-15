// Renderer Process - UI Logic (Simplified for Development)
const { ipcRenderer } = require('electron');

// Character Data with detailed stats
const characters = [
  { 
    id: 1, 
    name: 'Shadow', 
    fullName: 'Shadow Operative',
    role: 'Challenger', 
    icon: 'SO', 
    description: 'A skilled challenger specializing in aggressive plays and entry fragging.',
    weapon: {
      name: 'Shadow Strike Pistol',
      type: 'Pistol',
      description: 'Each kill increases fire rate and reload speed until the end of the round'
    },
    abilities: [
      {
        name: 'Silent Runner',
        type: 'Passive',
        description: 'Footsteps are reduced by 50% when crouch-walking, allowing for stealthier approaches'
      },
      {
        name: 'Seeking Spy',
        type: 'Ability 1',
        description: 'Dash forward while invisible for a short duration, allowing for aggressive repositioning'
      },
      {
        name: 'Dark Sword',
        type: 'Ability 2',
        description: 'Deploy a sword that creates a buffing zone. Allies within the radius gain faster attack and reload speed for 5 seconds (timer resets while in circle)'
      },
      {
        name: 'Phantom Strike',
        type: 'Ability 3',
        description: 'Mark an enemy. Your next shot against the marked target deals bonus damage and reveals them through walls for 3 seconds'
      },
      {
        name: 'Shadow Realm',
        type: 'Ultimate',
        description: 'Enter the Shadow Realm for 8 seconds. You become invisible, gain increased movement speed, and your weapon has unlimited ammo. Enemies you kill during this time cannot be revived'
      }
    ]
  },
  { 
    id: 2, 
    name: 'Void', 
    fullName: 'Void Walker',
    role: 'Controller', 
    icon: 'VW', 
    description: 'Master of area denial and smoke-based tactical control.',
    weapon: {
      name: 'Void Rifle',
      type: 'Assault Rifle',
      description: 'Energy-based rifle with phase rounds that penetrate thin surfaces'
    },
    abilities: [
      {
        name: 'Phase Shift',
        type: 'Passive',
        description: 'Reload while taking damage reduces incoming damage by 30%'
      },
      {
        name: 'Smoke Veil',
        type: 'Ability 1',
        description: 'Deploy smoke clouds that block vision and slowly heal allies inside'
      },
      {
        name: 'Void Step',
        type: 'Ability 2',
        description: 'Teleport short distances through walls, leaving a void marker at your original location'
      },
      {
        name: 'Gravity Well',
        type: 'Ability 3',
        description: 'Throw a device that pulls nearby enemies toward its center and grounds them for 2 seconds'
      },
      {
        name: 'Dimensional Rift',
        type: 'Ultimate',
        description: 'Create a massive area of distortion that slows, disorients enemies, and blocks enemy abilities for 7 seconds'
      }
    ]
  },
  { 
    id: 3, 
    name: 'Tech', 
    fullName: 'Tech Specialist',
    role: 'Analyst', 
    icon: 'TS', 
    description: 'Defensive expert with advanced technology and traps.',
    weapon: {
      name: 'Tech Scanner SMG',
      type: 'SMG',
      description: 'High fire rate weapon that reveals enemy positions on hit'
    },
    abilities: [
      {
        name: 'Tech Vision',
        type: 'Passive',
        description: 'See a brief directional indicator when enemies destroy your deployed equipment'
      },
      {
        name: 'Auto-Turret',
        type: 'Ability 1',
        description: 'Deploy automated turret that tracks and shoots enemies within range'
      },
      {
        name: 'Trap Mine',
        type: 'Ability 2',
        description: 'Place proximity mines that damage, slow, and reveal enemies who trigger them'
      },
      {
        name: 'EMP Drone',
        type: 'Ability 3',
        description: 'Deploy a drone that disables enemy equipment and reveals their utility through walls'
      },
      {
        name: 'Tactical Override',
        type: 'Ultimate',
        description: 'Hack all enemy electronics, reveal their positions through walls, and disable their abilities for 6 seconds'
      }
    ]
  },
  { 
    id: 4, 
    name: 'Energy', 
    fullName: 'Energy Wielder',
    role: 'Resister', 
    icon: 'EW', 
    description: 'Initiates team fights with powerful energy-based abilities.',
    weapon: {
      name: 'Plasma Cannon',
      type: 'Heavy Weapon',
      description: 'Charged energy weapon that deals explosive area damage'
    },
    abilities: [
      {
        name: 'Residual Energy',
        type: 'Passive',
        description: 'Gain 25 temporary shield points for 3 seconds after using an ability'
      },
      {
        name: 'Energy Shield',
        type: 'Ability 1',
        description: 'Deploy a directional shield that blocks incoming damage and can be picked up by allies'
      },
      {
        name: 'Pulse Blast',
        type: 'Ability 2',
        description: 'Fire an energy pulse that stuns enemies in its path and destroys enemy projectiles'
      },
      {
        name: 'Chain Lightning',
        type: 'Ability 3',
        description: 'Release lightning that chains between enemies, dealing damage and reducing their movement speed'
      },
      {
        name: 'Overcharge',
        type: 'Ultimate',
        description: 'Empower all allies with a massive energy surge granting 40% increased damage, movement speed, and shield regeneration for 10 seconds'
      }
    ]
  },
  {
    id: 5,
    name: 'Splinter',
    fullName: 'Splinter Agent',
    role: 'Spliter',
    icon: 'SP',
    description: 'Master of creating chaos and dividing enemy forces.',
    weapon: {
      name: 'Dual Blades',
      type: 'Melee',
      description: 'Twin energy blades with extended range and rapid strikes'
    },
    abilities: [
      {
        name: 'Shadow Step',
        type: 'Passive',
        description: 'Gain increased movement speed after getting a kill for 4 seconds'
      },
      {
        name: 'Decoy Clone',
        type: 'Ability 1',
        description: 'Create a holographic clone that mimics your movements and explodes when destroyed'
      },
      {
        name: 'Flash Step',
        type: 'Ability 2',
        description: 'Rapidly dash between multiple points, becoming briefly invulnerable during dashes'
      },
      {
        name: 'Smoke Bomb',
        type: 'Ability 3',
        description: 'Throw a smoke bomb that creates a smoke screen and disorients enemies caught in the initial blast'
      },
      {
        name: 'Fracture',
        type: 'Ultimate',
        description: 'Split into 3 controllable copies for 12 seconds. Each copy can attack independently and dying as one copy switches control to another'
      }
    ]
  }
];

// Backend API URL
const API_URL = 'http://localhost:3001/api';

// Current User Data (loaded from localStorage after login)
let currentUser = JSON.parse(localStorage.getItem('currentUser')) || {
  name: 'PlayerOne',
  userId: '12345',
  avatar: 'P1'
};

// Friends Data (loaded from backend)
let friendsData = [];

let selectedCharacterIndex = 0;
let currentTab = 'play';
let currentMode = 'unrated';

// DOM Elements
const loadingScreen = document.getElementById('loading-screen');
const mainMenu = document.getElementById('main-menu');
const progressFill = document.getElementById('progress-fill');
const progressPercentage = document.getElementById('progress-percentage');
const loadingSubtitle = document.getElementById('loading-subtitle');
const loadingDetails = document.getElementById('loading-details');

// Title Bar Controls
document.getElementById('minimize-btn').addEventListener('click', () => {
  ipcRenderer.send('minimize-window');
});

document.getElementById('close-btn').addEventListener('click', () => {
  ipcRenderer.send('close-window');
});

// Initialize Loading Screen (for launcher itself - fast)
function initLoadingScreen() {
  loadingScreen.classList.add('active', 'fullscreen');
  mainMenu.classList.remove('active');
  
  let progress = 0;
  const stages = [
    { progress: 33, text: 'Loading launcher...', details: 'Initializing components...' },
    { progress: 66, text: 'Loading assets...', details: 'Loading UI resources...' },
    { progress: 100, text: 'Ready!', details: 'Launcher ready!' }
  ];
  
  let currentStage = 0;
  
  const loadingInterval = setInterval(() => {
    if (currentStage < stages.length) {
      const stage = stages[currentStage];
      progress = stage.progress;
      
      progressFill.style.width = progress + '%';
      progressPercentage.textContent = progress + '%';
      loadingSubtitle.textContent = stage.text;
      loadingDetails.textContent = stage.details;
      
      loadingScreen.className = 'loading-screen active fullscreen ' + stage.text.toLowerCase().replace(/\s+/g, '-').replace(/\./g, '');
      
      currentStage++;
    } else {
      clearInterval(loadingInterval);
      
      setTimeout(() => {
        loadingScreen.classList.add('fade-out');
        setTimeout(() => {
          loadingScreen.classList.remove('active', 'fade-out', 'fullscreen');
          mainMenu.classList.add('active');
          initMainMenu();
        }, 500);
      }, 300);
    }
  }, 400);
  
  initParticles();
}

// Particle Animation
function initParticles() {
  const canvas = document.getElementById('particles-canvas');
  const ctx = canvas.getContext('2d');
  
  const isFullscreen = loadingScreen.classList.contains('fullscreen');
  canvas.width = window.innerWidth;
  canvas.height = isFullscreen ? window.innerHeight : window.innerHeight - 40;
  
  const particles = [];
  const particleCount = 50;
  
  class Particle {
    constructor() {
      this.reset();
    }
    
    reset() {
      this.x = Math.random() * canvas.width;
      this.y = Math.random() * canvas.height;
      this.vx = (Math.random() - 0.5) * 0.5;
      this.vy = (Math.random() - 0.5) * 0.5;
      this.size = Math.random() * 2 + 1;
      this.life = 1;
    }
    
    update() {
      this.x += this.vx;
      this.y += this.vy;
      this.life -= 0.002;
      
      if (this.life <= 0 || this.x < 0 || this.x > canvas.width || this.y < 0 || this.y > canvas.height) {
        this.reset();
      }
    }
    
    draw() {
      ctx.beginPath();
      ctx.arc(this.x, this.y, this.size, 0, Math.PI * 2);
      ctx.fillStyle = `rgba(139, 92, 246, ${this.life * 0.5})`;
      ctx.fill();
    }
  }
  
  for (let i = 0; i < particleCount; i++) {
    particles.push(new Particle());
  }
  
  function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    particles.forEach(particle => {
      particle.update();
      particle.draw();
    });
    
    requestAnimationFrame(animate);
  }
  
  animate();
}

// Initialize Main Menu
function initMainMenu() {
  renderCharactersGrid();
  loadFriends(); // Load friends from backend
  updatePlayerAgent();
  loadSettings();
}

// Update Player Agent Display
function updatePlayerAgent() {
  const char = characters[selectedCharacterIndex];
  const playerAgentElement = document.getElementById('player-agent');
  if (playerAgentElement) {
    playerAgentElement.textContent = char.name;
  }
  
  // Update agent icon in slot 1
  const agentIcon = document.querySelector('#slot-1 .agent-icon');
  if (agentIcon) {
    agentIcon.textContent = char.icon;
  }
}

// Load Friends from Backend
async function loadFriends() {
  if (!currentUser || !currentUser.userId) {
    console.log('No user logged in, skipping friend load');
    return;
  }
  
  try {
    const response = await fetch(`${API_URL}/friends/${currentUser.userId}`);
    const data = await response.json();
    
    if (response.ok && data.success) {
      // Convert backend format to frontend format
      friendsData = data.friends.map(friend => ({
        name: friend.username,
        userId: friend.userId,
        avatar: friend.avatar,
        online: Math.random() > 0.5, // TODO: implement real online status
        status: Math.random() > 0.5 ? 'Online' : 'Offline 1h ago'
      }));
      
      renderFriends();
    }
  } catch (error) {
    console.error('Failed to load friends:', error);
    // Keep empty friends list on error
    friendsData = [];
    renderFriends();
  }
}

// Render Friends List
function renderFriends() {
  const friendsList = document.getElementById('friends-list');
  const onlineCount = document.getElementById('friends-online');
  
  if (!friendsList) return;
  
  const onlineFriends = friendsData.filter(f => f.online).length;
  onlineCount.textContent = `${onlineFriends} ONLINE`;
  
  friendsList.innerHTML = friendsData.map(friend => `
    <div class="friend-item">
      <div class="friend-avatar ${friend.online ? 'online' : 'offline'}">${friend.avatar}</div>
      <div class="friend-info">
        <div class="friend-name">${friend.name}</div>
        <div class="friend-status ${friend.online ? 'online' : ''}">${friend.status}</div>
      </div>
      <div class="friend-actions">
        ${friend.online ? '<button class="friend-action-btn" title="Invite">📧</button>' : ''}
        <button class="friend-action-btn" title="Profile">👤</button>
      </div>
    </div>
  `).join('');
}

// Toggle Friend Request Modal
function toggleFriendRequestModal() {
  const modal = document.getElementById('friend-request-modal');
  modal.classList.toggle('active');
  
  // Clear inputs when opening
  if (modal.classList.contains('active')) {
    document.getElementById('friend-name-input').value = '';
    document.getElementById('friend-id-input').value = '';
    document.getElementById('friend-request-error').textContent = '';
    document.getElementById('friend-request-success').textContent = '';
  }
}

// Send Friend Request (Real Backend API call)
async function sendFriendRequest() {
  const nameInput = document.getElementById('friend-name-input').value.trim();
  const idInput = document.getElementById('friend-id-input').value.trim();
  const errorEl = document.getElementById('friend-request-error');
  const successEl = document.getElementById('friend-request-success');
  
  // Clear previous messages
  errorEl.textContent = '';
  successEl.textContent = '';
  
  // Validation
  if (!nameInput || !idInput) {
    errorEl.textContent = 'Please fill in both fields';
    return;
  }
  
  if (idInput.length !== 5 || !/^\d+$/.test(idInput)) {
    errorEl.textContent = 'User ID must be exactly 5 digits';
    return;
  }
  
  try {
    // Send friend request to backend
    const response = await fetch(`${API_URL}/friends/request`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        senderUserId: currentUser.userId,
        receiverUsername: nameInput,
        receiverUserId: idInput
      })
    });
    
    const data = await response.json();
    
    if (!response.ok) {
      errorEl.textContent = data.error || 'Failed to send friend request';
      return;
    }
    
    // Success!
    successEl.textContent = data.message || `Friend request sent to ${nameInput}!`;
    
    // Clear inputs after 2 seconds and close modal
    setTimeout(() => {
      toggleFriendRequestModal();
    }, 2000);
    
  } catch (error) {
    console.error('Friend request error:', error);
    errorEl.textContent = 'Failed to connect to server. Please try again.';
  }
}

// Close modal when clicking outside
function closeFriendRequestModal(event) {
  if (event.target.id === 'friend-request-modal') {
    toggleFriendRequestModal();
  }
}

// Make functions globally accessible
window.toggleFriendRequestModal = toggleFriendRequestModal;
window.sendFriendRequest = sendFriendRequest;
window.closeFriendRequestModal = closeFriendRequestModal;

// Render Characters Grid
function renderCharactersGrid() {
  const grid = document.getElementById('characters-grid');
  if (!grid) return;
  
  grid.innerHTML = characters.map((char, index) => `
    <div class="character-card-large ${index === selectedCharacterIndex ? 'selected' : ''}" 
         data-char-index="${index}">
      <div class="character-icon-large">${char.icon}</div>
      <div class="character-name-large">${char.name}</div>
      <div class="character-role-large">${char.role}</div>
      <div class="character-description">${char.description}</div>
      <button class="view-details-btn" onclick="showCharacterDetails(${index}); event.stopPropagation();">
        View Details
      </button>
    </div>
  `).join('');
  
  // Character cards are now view-only, no selection on click
  // Users can only view details, not select characters from the lobby
}

// Show Character Details Popup
function showCharacterDetails(index) {
  const char = characters[index];
  const popup = document.getElementById('character-popup');
  
  // Store current character index for selection
  popup.dataset.charIndex = index;
  
  // Populate popup content
  document.getElementById('popup-char-icon').textContent = char.icon;
  document.getElementById('popup-char-name').textContent = char.fullName;
  document.getElementById('popup-char-role').textContent = char.role;
  document.getElementById('popup-char-description').textContent = char.description;
  
  // Weapon info
  document.getElementById('popup-weapon-name').textContent = char.weapon.name;
  document.getElementById('popup-weapon-type').textContent = char.weapon.type;
  document.getElementById('popup-weapon-desc').textContent = char.weapon.description;
  
  // Abilities
  const abilitiesContainer = document.getElementById('popup-abilities');
  abilitiesContainer.innerHTML = char.abilities.map(ability => `
    <div class="ability-item">
      <div class="ability-header">
        <span class="ability-name">${ability.name}</span>
        <span class="ability-type">${ability.type}</span>
      </div>
      <p class="ability-description">${ability.description}</p>
    </div>
  `).join('');
  
  // Show popup
  popup.classList.add('active');
  document.body.style.overflow = 'hidden';
}

// Select character from popup
function selectCharacterFromPopup() {
  const popup = document.getElementById('character-popup');
  const charIndex = parseInt(popup.dataset.charIndex);
  selectCharacter(charIndex);
  closeCharacterDetails();
  switchTab('play');
}

// Close Character Details Popup
function closeCharacterDetails() {
  const popup = document.getElementById('character-popup');
  popup.classList.remove('active');
  document.body.style.overflow = '';
}

// Make functions globally accessible
window.showCharacterDetails = showCharacterDetails;
window.closeCharacterDetails = closeCharacterDetails;
window.selectCharacterFromPopup = selectCharacterFromPopup;

// Select Character
function selectCharacter(index) {
  selectedCharacterIndex = index;
  updatePlayerAgent();
  renderCharactersGrid();
}

// Change Agent Button
const changeAgentBtn = document.getElementById('change-agent-btn');
if (changeAgentBtn) {
  changeAgentBtn.addEventListener('click', () => {
    switchTab('characters');
  });
}

// Start Game Button
const startGameBtn = document.getElementById('start-game-btn');
if (startGameBtn) {
  startGameBtn.addEventListener('click', () => {
    launchGame();
  });
}

// Mode Tabs
document.querySelectorAll('.mode-tab').forEach(tab => {
  tab.addEventListener('click', () => {
    const mode = tab.dataset.mode;
    switchMode(mode);
  });
});

function switchMode(mode) {
  currentMode = mode;
  
  document.querySelectorAll('.mode-tab').forEach(tab => {
    tab.classList.toggle('active', tab.dataset.mode === mode);
  });
}

// Tab Navigation
document.querySelectorAll('.sidebar-tab').forEach(tab => {
  tab.addEventListener('click', () => {
    const tabName = tab.dataset.tab;
    switchTab(tabName);
  });
});

function switchTab(tabName) {
  currentTab = tabName;
  
  // Update tab buttons
  document.querySelectorAll('.sidebar-tab').forEach(tab => {
    tab.classList.toggle('active', tab.dataset.tab === tabName);
  });
  
  // Update tab content
  document.querySelectorAll('.tab-content').forEach(content => {
    content.classList.toggle('active', content.id === `tab-${tabName}`);
  });
}

// Launch Game
function launchGame() {
  mainMenu.classList.remove('active');
  loadingScreen.classList.add('active', 'fullscreen');
  
  progressFill.style.width = '0%';
  progressPercentage.textContent = '0%';
  loadingSubtitle.textContent = 'Launching Game...';
  loadingDetails.textContent = 'Preparing to launch...';
  
  const gameConfig = {
    selectedCharacter: characters[selectedCharacterIndex].name,
    mode: currentMode
  };
  
  ipcRenderer.send('launch-game', gameConfig);
}

// Game Launch Listeners
ipcRenderer.on('game-launching', (event, data) => {
  const stageMessages = {
    'initializing': { text: 'Initializing...', details: 'Starting game engine...' },
    'process-started': { text: 'Game Starting...', details: 'Unreal Engine process launched...' },
    'loading-assets': { text: 'Loading Assets...', details: 'Loading game resources...' },
    'initializing-engine': { text: 'Initializing Engine...', details: 'Starting Unreal Engine...' },
    'loading-level': { text: 'Loading World...', details: 'Preparing game environment...' },
    'starting-game': { text: 'Starting Game...', details: 'Almost ready...' },
    'complete': { text: 'Ready!', details: 'Game loaded successfully!' }
  };
  
  const stage = stageMessages[data.stage] || { text: 'Loading...', details: data.details || 'Please wait...' };
  const progress = data.progress || 0;
  
  progressFill.style.width = progress + '%';
  progressPercentage.textContent = progress + '%';
  loadingSubtitle.textContent = stage.text;
  loadingDetails.textContent = data.details || stage.details;
  loadingScreen.className = 'loading-screen active fullscreen ' + data.stage;
});

ipcRenderer.on('game-launched', () => {
  progressFill.style.width = '100%';
  progressPercentage.textContent = '100%';
  loadingSubtitle.textContent = 'Game Launched!';
  loadingDetails.textContent = 'Enjoy playing ProjectXS!';
  
  setTimeout(() => {
    loadingScreen.classList.add('fade-out');
    setTimeout(() => {
      loadingScreen.classList.remove('active', 'fade-out', 'fullscreen');
      mainMenu.classList.add('active');
    }, 500);
  }, 1500);
});

ipcRenderer.on('game-launch-error', (event, error) => {
  alert('Failed to launch game: ' + error);
  loadingScreen.classList.remove('active', 'fullscreen');
  mainMenu.classList.add('active');
});

// Settings
const volumeSlider = document.getElementById('volume-setting');
const volumeValue = document.getElementById('volume-value');

if (volumeSlider && volumeValue) {
  volumeSlider.addEventListener('input', (e) => {
    volumeValue.textContent = e.target.value + '%';
  });
}

const saveSettingsBtn = document.getElementById('save-settings');
if (saveSettingsBtn) {
  saveSettingsBtn.addEventListener('click', () => {
    const settings = {
      selectedCharacter: selectedCharacterIndex,
      volume: volumeSlider ? volumeSlider.value : 50,
      graphics: document.getElementById('graphics-setting') ? document.getElementById('graphics-setting').value : 'high'
    };
    
    ipcRenderer.send('save-settings', settings);
  });
}

ipcRenderer.on('settings-saved', () => {
  alert('Settings saved successfully!');
});

// Load Settings
function loadSettings() {
  ipcRenderer.send('load-settings');
}

ipcRenderer.on('settings-loaded', (event, settings) => {
  selectedCharacterIndex = settings.selectedCharacter || 0;
  if (volumeSlider && volumeValue) {
    volumeSlider.value = settings.volume || 50;
    volumeValue.textContent = volumeSlider.value + '%';
  }
  const graphicsSelect = document.getElementById('graphics-setting');
  if (graphicsSelect) {
    graphicsSelect.value = settings.graphics || 'high';
  }
  
  selectCharacter(selectedCharacterIndex);
});

// Start the launcher
window.addEventListener('DOMContentLoaded', () => {
  // Check if user is authenticated
  const currentUser = localStorage.getItem('currentUser');
  const authToken = localStorage.getItem('authToken');
  
  if (!currentUser || !authToken) {
    // Not authenticated, redirect to login
    window.location.href = 'auth.html';
    return;
  }
  
  // Authenticated, start the launcher
  initLoadingScreen();
});

// Handle window resize
window.addEventListener('resize', () => {
  const canvas = document.getElementById('particles-canvas');
  if (canvas) {
    const isFullscreen = loadingScreen.classList.contains('fullscreen');
    canvas.width = window.innerWidth;
    canvas.height = isFullscreen ? window.innerHeight : window.innerHeight - 40;
  }
});
