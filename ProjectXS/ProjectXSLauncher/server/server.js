const express = require('express');
const cors = require('cors');
const Database = require('better-sqlite3');
const crypto = require('crypto');
const jwt = require('jsonwebtoken');
const passport = require('passport');
const GoogleStrategy = require('passport-google-oauth20').Strategy;
const AppleStrategy = require('passport-apple');
const session = require('express-session');
const nodemailer = require('nodemailer');
const path = require('path');
require('dotenv').config();

const app = express();
const PORT = 3001;

// JWT Secret (in production, use environment variable)
const JWT_SECRET = process.env.JWT_SECRET || 'projectxs-secret-key-change-in-production';

// Middleware
app.use(cors({
  origin: true,
  credentials: true
}));
app.use(express.json());
app.use(session({
  secret: 'projectxs-session-secret',
  resave: false,
  saveUninitialized: false
}));
app.use(passport.initialize());
app.use(passport.session());

// Initialize SQLite database
const db = new Database(path.join(__dirname, 'projectxs.db'));

// Create tables
db.exec(`
  CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    userId TEXT UNIQUE NOT NULL,
    password TEXT,
    email TEXT,
    emailVerified INTEGER DEFAULT 0,
    verificationCode TEXT,
    verificationExpires DATETIME,
    avatar TEXT DEFAULT 'P1',
    provider TEXT DEFAULT 'local',
    googleId TEXT UNIQUE,
    appleId TEXT UNIQUE,
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP
  );

  CREATE TABLE IF NOT EXISTS friend_requests (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    senderId INTEGER NOT NULL,
    receiverId INTEGER NOT NULL,
    status TEXT DEFAULT 'pending',
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (senderId) REFERENCES users(id),
    FOREIGN KEY (receiverId) REFERENCES users(id),
    UNIQUE(senderId, receiverId)
  );

  CREATE TABLE IF NOT EXISTS friends (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userId INTEGER NOT NULL,
    friendId INTEGER NOT NULL,
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (userId) REFERENCES users(id),
    FOREIGN KEY (friendId) REFERENCES users(id),
    UNIQUE(userId, friendId)
  );
`);

// Simple password hashing using crypto (built-in)
function hashPassword(password) {
  return crypto.createHash('sha256').update(password).digest('hex');
}

function verifyPassword(password, hash) {
  return hashPassword(password) === hash;
}

// Email transporter setup
// Prefer explicit SMTP host/port (Mailtrap) when provided; fall back to service shorthand
let emailTransporter;
if (process.env.EMAIL_HOST && process.env.EMAIL_PORT) {
  emailTransporter = nodemailer.createTransport({
    host: process.env.EMAIL_HOST,
    port: parseInt(process.env.EMAIL_PORT, 10) || 2525,
    secure: (process.env.EMAIL_SECURE === 'true'), // true for 465, false for other ports
    auth: {
      user: process.env.EMAIL_SMTP_USER || process.env.EMAIL_USER,
      pass: process.env.EMAIL_SMTP_PASS || process.env.EMAIL_PASSWORD
    }
  });
} else {
  emailTransporter = nodemailer.createTransport({
    service: process.env.EMAIL_SERVICE || 'gmail',
    auth: {
      user: process.env.EMAIL_USER || 'your-email@gmail.com',
      pass: process.env.EMAIL_PASSWORD || 'your-app-password'
    }
  });
}

// Generate 6-digit verification code
function generateVerificationCode() {
  return String(Math.floor(100000 + Math.random() * 900000));
}

// Send verification email
async function sendVerificationEmail(email, code, username) {
  const mailOptions = {
    from: process.env.EMAIL_USER || 'ProjectXS <noreply@projectxs.com>',
    to: email,
    subject: 'Verify Your ProjectXS Email',
    html: `
      <div style="font-family: Arial, sans-serif; max-width: 600px; margin: 0 auto;">
        <h2 style="color: #8B5CF6;">Welcome to ProjectXS!</h2>
        <p>Hi ${username},</p>
        <p>Thank you for signing up! Please verify your email address using the code below:</p>
        <div style="background: #f3f4f6; padding: 20px; text-align: center; border-radius: 8px; margin: 20px 0;">
          <h1 style="color: #8B5CF6; font-size: 32px; letter-spacing: 4px; margin: 0;">${code}</h1>
        </div>
        <p>This code will expire in 10 minutes.</p>
        <p>If you didn't request this, please ignore this email.</p>
        <hr style="border: none; border-top: 1px solid #e5e7eb; margin: 30px 0;">
        <p style="color: #6b7280; font-size: 12px;">ProjectXS - Enter the Arena</p>
      </div>
    `
  };

  try {
    await emailTransporter.sendMail(mailOptions);
    return true;
  } catch (error) {
    console.error('Email send error:', error);
    return false;
  }
}

// Helper function to generate JWT token
function generateToken(user) {
  return jwt.sign(
    { 
      id: user.id, 
      username: user.username,
      userId: user.userId 
    },
    JWT_SECRET,
    { expiresIn: '30d' }
  );
}

// Passport serialization
passport.serializeUser((user, done) => {
  done(null, user.id);
});

passport.deserializeUser((id, done) => {
  const stmt = db.prepare('SELECT id, username, userId, email, avatar, provider FROM users WHERE id = ?');
  const user = stmt.get(id);
  done(null, user);
});

// Google OAuth Strategy
passport.use(new GoogleStrategy({
    clientID: process.env.GOOGLE_CLIENT_ID || 'YOUR_GOOGLE_CLIENT_ID',
    clientSecret: process.env.GOOGLE_CLIENT_SECRET || 'YOUR_GOOGLE_CLIENT_SECRET',
    callbackURL: 'http://localhost:3001/api/auth/google/callback'
  },
  async (accessToken, refreshToken, profile, done) => {
    try {
      // Check if user exists with Google ID
      let stmt = db.prepare('SELECT * FROM users WHERE googleId = ?');
      let user = stmt.get(profile.id);
      
      if (user) {
        return done(null, user);
      }
      
      // Check if user exists with same email
      const email = profile.emails && profile.emails[0] ? profile.emails[0].value : null;
      if (email) {
        stmt = db.prepare('SELECT * FROM users WHERE email = ?');
        user = stmt.get(email);
        
        if (user) {
          // Link Google account to existing user
          const updateStmt = db.prepare('UPDATE users SET googleId = ? WHERE id = ?');
          updateStmt.run(profile.id, user.id);
          user.googleId = profile.id;
          return done(null, user);
        }
      }
      
      // Create new user
      const username = profile.displayName.replace(/\s+/g, '') + Math.floor(Math.random() * 1000);
      const userId = generateUserId();
      const avatar = username.substring(0, 2).toUpperCase();
      
      const insertStmt = db.prepare(`
        INSERT INTO users (username, userId, email, avatar, provider, googleId)
        VALUES (?, ?, ?, ?, 'google', ?)
      `);
      
      const result = insertStmt.run(username, userId, email, avatar, profile.id);
      
      user = {
        id: result.lastInsertRowid,
        username,
        userId,
        email,
        avatar,
        provider: 'google',
        googleId: profile.id
      };
      
      done(null, user);
    } catch (error) {
      done(error, null);
    }
  }
));

// Apple OAuth Strategy
passport.use(new AppleStrategy({
    clientID: process.env.APPLE_CLIENT_ID || 'YOUR_APPLE_CLIENT_ID',
    teamID: process.env.APPLE_TEAM_ID || 'YOUR_APPLE_TEAM_ID',
    keyID: process.env.APPLE_KEY_ID || 'YOUR_APPLE_KEY_ID',
    privateKeyString: process.env.APPLE_PRIVATE_KEY || 'YOUR_APPLE_PRIVATE_KEY',
    callbackURL: 'http://localhost:3001/api/auth/apple/callback'
  },
  async (accessToken, refreshToken, idToken, profile, done) => {
    try {
      // Check if user exists with Apple ID
      let stmt = db.prepare('SELECT * FROM users WHERE appleId = ?');
      let user = stmt.get(profile.id);
      
      if (user) {
        return done(null, user);
      }
      
      // Check if user exists with same email
      const email = profile.email;
      if (email) {
        stmt = db.prepare('SELECT * FROM users WHERE email = ?');
        user = stmt.get(email);
        
        if (user) {
          // Link Apple account to existing user
          const updateStmt = db.prepare('UPDATE users SET appleId = ? WHERE id = ?');
          updateStmt.run(profile.id, user.id);
          user.appleId = profile.id;
          return done(null, user);
        }
      }
      
      // Create new user
      const username = (profile.name?.firstName || 'AppleUser') + Math.floor(Math.random() * 1000);
      const userId = generateUserId();
      const avatar = username.substring(0, 2).toUpperCase();
      
      const insertStmt = db.prepare(`
        INSERT INTO users (username, userId, email, avatar, provider, appleId)
        VALUES (?, ?, ?, ?, 'apple', ?)
      `);
      
      const result = insertStmt.run(username, userId, email, avatar, profile.id);
      
      user = {
        id: result.lastInsertRowid,
        username,
        userId,
        email,
        avatar,
        provider: 'apple',
        appleId: profile.id
      };
      
      done(null, user);
    } catch (error) {
      done(error, null);
    }
  }
));

// Helper function to generate unique 5-digit user ID
function generateUserId() {
  let userId;
  let exists = true;
  
  while (exists) {
    userId = String(Math.floor(10000 + Math.random() * 90000));
    const stmt = db.prepare('SELECT userId FROM users WHERE userId = ?');
    exists = stmt.get(userId) !== undefined;
  }
  
  return userId;
}

// Seed some test users if database is empty
const userCount = db.prepare('SELECT COUNT(*) as count FROM users').get();
if (userCount.count === 0) {
  console.log('Seeding database with test users...');
  
  const testUsers = [
    { username: 'PlayerOne', userId: '12345', password: 'password123' },
    { username: 'GhostRider', userId: '33445', password: 'password123' },
    { username: 'BlitzKing', userId: '66778', password: 'password123' },
    { username: 'EchoWolf', userId: '99001', password: 'password123' },
    { username: 'NovaStar', userId: '22334', password: 'password123' },
    { username: 'PhantomAce', userId: '55667', password: 'password123' },
    { username: 'ShadowBlade', userId: '88990', password: 'password123' },
    { username: 'CyberNinja', userId: '11223', password: 'password123' }
  ];
  
  const insertStmt = db.prepare('INSERT INTO users (username, userId, password, avatar) VALUES (?, ?, ?, ?)');
  
  for (const user of testUsers) {
    const hashedPassword = hashPassword(user.password);
    const avatar = user.username.substring(0, 2).toUpperCase();
    insertStmt.run(user.username, user.userId, hashedPassword, avatar);
  }
  
  console.log('Test users created successfully!');
}

// ==================== API ROUTES ====================

// Register new user
app.post('/api/auth/register', async (req, res) => {
  try {
    const { username, password, email } = req.body;
    
    if (!username || !password) {
      return res.status(400).json({ error: 'Username and password required' });
    }
    
    if (username.length < 3 || username.length > 20) {
      return res.status(400).json({ error: 'Username must be 3-20 characters' });
    }
    
    const userId = generateUserId();
    const hashedPassword = hashPassword(password);
    const avatar = username.substring(0, 2).toUpperCase();
    
    const stmt = db.prepare('INSERT INTO users (username, userId, password, email, avatar, provider) VALUES (?, ?, ?, ?, ?, ?)');
    const result = stmt.run(username, userId, hashedPassword, email || null, avatar, 'local');
    
    const user = {
      id: result.lastInsertRowid,
      username,
      userId,
      email,
      avatar,
      provider: 'local'
    };
    
    const token = generateToken(user);
    
    res.json({ success: true, user, token });
  } catch (error) {
    if (error.message.includes('UNIQUE constraint failed')) {
      res.status(400).json({ error: 'Username already exists' });
    } else {
      console.error('Register error:', error);
      res.status(500).json({ error: 'Server error' });
    }
  }
});

// Login
app.post('/api/auth/login', async (req, res) => {
  try {
    const { username, password } = req.body;
    
    if (!username || !password) {
      return res.status(400).json({ error: 'Username and password required' });
    }
    
    const stmt = db.prepare('SELECT * FROM users WHERE username = ?');
    const user = stmt.get(username);
    
    if (!user) {
      return res.status(401).json({ error: 'Invalid credentials' });
    }
    
    if (!user.password) {
      return res.status(401).json({ error: 'This account uses social login. Please sign in with Google or Apple.' });
    }
    
    const validPassword = verifyPassword(password, user.password);
    
    if (!validPassword) {
      return res.status(401).json({ error: 'Invalid credentials' });
    }
    
    const token = generateToken(user);
    
    res.json({
      success: true,
      user: {
        id: user.id,
        username: user.username,
        userId: user.userId,
        email: user.email,
        avatar: user.avatar,
        provider: user.provider
      },
      token
    });
  } catch (error) {
    console.error('Login error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Google OAuth Routes
app.get('/api/auth/google',
  // Include 'openid' along with profile and email to satisfy modern Google OAuth requirements
  // Use prompt:'select_account' so users see the account chooser and avoid automatic passkey flows
  passport.authenticate('google', { scope: ['openid', 'profile', 'email'], prompt: 'select_account' })
);

app.get('/api/auth/google/callback',
  passport.authenticate('google', { failureRedirect: '/api/auth/callback/error?error=Authentication%20failed' }),
  async (req, res) => {
    try {
      const token = generateToken(req.user);
      const user = req.user;
      
      // If Google sign-in and email not verified, send verification code
      if (!user.emailVerified && user.email) {
        const verificationCode = generateVerificationCode();
        const expiresAt = new Date(Date.now() + 10 * 60 * 1000); // 10 minutes
        
        // Save verification code to database
        const updateStmt = db.prepare('UPDATE users SET verificationCode = ?, verificationExpires = ? WHERE id = ?');
        updateStmt.run(verificationCode, expiresAt.toISOString(), user.id);
        
        // Send verification email
        await sendVerificationEmail(user.email, verificationCode, user.username);
      }
      
      // Redirect with data
      const userData = encodeURIComponent(JSON.stringify(user));
      res.redirect(`/api/auth/callback/success?token=${token}&user=${userData}`);
    } catch (error) {
      console.error('OAuth callback error:', error);
      res.redirect('/api/auth/callback/error?error=Authentication%20failed');
    }
  }
);

// Apple OAuth Routes
app.get('/api/auth/apple',
  passport.authenticate('apple')
);

app.post('/api/auth/apple/callback',
  passport.authenticate('apple', { failureRedirect: '/api/auth/callback/error?error=Authentication%20failed' }),
  async (req, res) => {
    try {
      const token = generateToken(req.user);
      const userData = encodeURIComponent(JSON.stringify(req.user));
      res.redirect(`/api/auth/callback/success?token=${token}&user=${userData}`);
    } catch (error) {
      console.error('OAuth callback error:', error);
      res.redirect('/api/auth/callback/error?error=Authentication%20failed');
    }
  }
);

// OAuth success page
app.get('/api/auth/callback/success', (req, res) => {
  res.send(`
    <html>
      <body>
        <h2>Authentication Successful!</h2>
        <p>Redirecting...</p>
        <script>
          // Page will be detected by Electron
          setTimeout(() => window.close(), 1000);
        </script>
      </body>
    </html>
  `);
});

// OAuth failure page
app.get('/api/auth/callback/error', (req, res) => {
  const error = req.query.error || 'Authentication failed';
  res.send(`
    <html>
      <body>
        <h2>Authentication Failed</h2>
        <p>${error}</p>
        <p>This window will close automatically...</p>
        <script>
          setTimeout(() => window.close(), 3000);
        </script>
      </body>
    </html>
  `);
});

// Verify email with code
app.post('/api/auth/verify-email', (req, res) => {
  try {
    const { userId, code } = req.body;
    
    if (!userId || !code) {
      return res.status(400).json({ error: 'User ID and verification code required' });
    }
    
    const stmt = db.prepare('SELECT * FROM users WHERE userId = ?');
    const user = stmt.get(userId);
    
    if (!user) {
      return res.status(404).json({ error: 'User not found' });
    }
    
    if (user.emailVerified) {
      return res.json({ success: true, message: 'Email already verified' });
    }
    
    if (!user.verificationCode) {
      return res.status(400).json({ error: 'No verification code found. Please request a new one.' });
    }
    
    // Check if code expired
    const now = new Date();
    const expires = new Date(user.verificationExpires);
    if (now > expires) {
      return res.status(400).json({ error: 'Verification code expired. Please request a new one.' });
    }
    
    // Verify code
    if (user.verificationCode !== code) {
      return res.status(400).json({ error: 'Invalid verification code' });
    }
    
    // Mark email as verified
    const updateStmt = db.prepare('UPDATE users SET emailVerified = 1, verificationCode = NULL, verificationExpires = NULL WHERE id = ?');
    updateStmt.run(user.id);
    
    res.json({ success: true, message: 'Email verified successfully!' });
  } catch (error) {
    console.error('Verify email error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Resend verification code
app.post('/api/auth/resend-verification', async (req, res) => {
  try {
    const { userId } = req.body;
    
    if (!userId) {
      return res.status(400).json({ error: 'User ID required' });
    }
    
    const stmt = db.prepare('SELECT * FROM users WHERE userId = ?');
    const user = stmt.get(userId);
    
    if (!user) {
      return res.status(404).json({ error: 'User not found' });
    }
    
    if (user.emailVerified) {
      return res.json({ success: true, message: 'Email already verified' });
    }
    
    if (!user.email) {
      return res.status(400).json({ error: 'No email address on file' });
    }
    
    // Generate new code
    const verificationCode = generateVerificationCode();
    const expiresAt = new Date(Date.now() + 10 * 60 * 1000); // 10 minutes
    
    // Save to database
    const updateStmt = db.prepare('UPDATE users SET verificationCode = ?, verificationExpires = ? WHERE id = ?');
    updateStmt.run(verificationCode, expiresAt.toISOString(), user.id);
    
    // Send email
    const sent = await sendVerificationEmail(user.email, verificationCode, user.username);
    
    if (!sent) {
      return res.status(500).json({ error: 'Failed to send verification email' });
    }
    
    res.json({ success: true, message: 'Verification code sent to your email' });
  } catch (error) {
    console.error('Resend verification error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Search user by username and userId
app.get('/api/users/search', (req, res) => {
  try {
    const { username, userId } = req.query;
    
    if (!username && !userId) {
      return res.status(400).json({ error: 'Username or userId required' });
    }
    
    let stmt;
    let user;
    
    if (username && userId) {
      stmt = db.prepare('SELECT id, username, userId, avatar FROM users WHERE username = ? AND userId = ?');
      user = stmt.get(username, userId);
    } else if (userId) {
      stmt = db.prepare('SELECT id, username, userId, avatar FROM users WHERE userId = ?');
      user = stmt.get(userId);
    } else {
      stmt = db.prepare('SELECT id, username, userId, avatar FROM users WHERE username = ?');
      user = stmt.get(username);
    }
    
    if (!user) {
      return res.status(404).json({ error: 'User not found' });
    }
    
    res.json({ success: true, user });
  } catch (error) {
    console.error('Search error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Get user's friends
app.get('/api/friends/:userId', (req, res) => {
  try {
    const { userId } = req.params;
    
    const userStmt = db.prepare('SELECT id FROM users WHERE userId = ?');
    const currentUser = userStmt.get(userId);
    
    if (!currentUser) {
      return res.status(404).json({ error: 'User not found' });
    }
    
    const stmt = db.prepare(`
      SELECT u.id, u.username, u.userId, u.avatar
      FROM friends f
      JOIN users u ON f.friendId = u.id
      WHERE f.userId = ?
    `);
    
    const friends = stmt.all(currentUser.id);
    
    res.json({ success: true, friends });
  } catch (error) {
    console.error('Get friends error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Send friend request
app.post('/api/friends/request', (req, res) => {
  try {
    const { senderUserId, receiverUsername, receiverUserId } = req.body;
    
    if (!senderUserId || !receiverUsername || !receiverUserId) {
      return res.status(400).json({ error: 'Missing required fields' });
    }
    
    // Get sender
    const senderStmt = db.prepare('SELECT id FROM users WHERE userId = ?');
    const sender = senderStmt.get(senderUserId);
    
    if (!sender) {
      return res.status(404).json({ error: 'Sender not found' });
    }
    
    // Get receiver
    const receiverStmt = db.prepare('SELECT id, username, userId FROM users WHERE username = ? AND userId = ?');
    const receiver = receiverStmt.get(receiverUsername, receiverUserId);
    
    if (!receiver) {
      return res.status(404).json({ error: 'User not found' });
    }
    
    // Check if trying to add self
    if (sender.id === receiver.id) {
      return res.status(400).json({ error: 'Cannot add yourself as friend' });
    }
    
    // Check if already friends
    const friendCheckStmt = db.prepare('SELECT id FROM friends WHERE userId = ? AND friendId = ?');
    const alreadyFriends = friendCheckStmt.get(sender.id, receiver.id);
    
    if (alreadyFriends) {
      return res.status(400).json({ error: 'Already friends with this user' });
    }
    
    // Check if request already exists
    const requestCheckStmt = db.prepare('SELECT id FROM friend_requests WHERE senderId = ? AND receiverId = ? AND status = "pending"');
    const existingRequest = requestCheckStmt.get(sender.id, receiver.id);
    
    if (existingRequest) {
      return res.status(400).json({ error: 'Friend request already sent' });
    }
    
    // Create friend request
    const insertStmt = db.prepare('INSERT INTO friend_requests (senderId, receiverId, status) VALUES (?, ?, "pending")');
    const result = insertStmt.run(sender.id, receiver.id);
    
    res.json({
      success: true,
      message: `Friend request sent to ${receiver.username}`,
      requestId: result.lastInsertRowid
    });
  } catch (error) {
    console.error('Send friend request error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Get pending friend requests
app.get('/api/friends/requests/:userId', (req, res) => {
  try {
    const { userId } = req.params;
    
    const userStmt = db.prepare('SELECT id FROM users WHERE userId = ?');
    const currentUser = userStmt.get(userId);
    
    if (!currentUser) {
      return res.status(404).json({ error: 'User not found' });
    }
    
    const stmt = db.prepare(`
      SELECT fr.id, fr.createdAt, u.id as senderId, u.username, u.userId, u.avatar
      FROM friend_requests fr
      JOIN users u ON fr.senderId = u.id
      WHERE fr.receiverId = ? AND fr.status = 'pending'
      ORDER BY fr.createdAt DESC
    `);
    
    const requests = stmt.all(currentUser.id);
    
    res.json({ success: true, requests });
  } catch (error) {
    console.error('Get friend requests error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Accept friend request
app.post('/api/friends/accept/:requestId', (req, res) => {
  try {
    const { requestId } = req.params;
    
    // Get request details
    const requestStmt = db.prepare('SELECT senderId, receiverId FROM friend_requests WHERE id = ? AND status = "pending"');
    const request = requestStmt.get(requestId);
    
    if (!request) {
      return res.status(404).json({ error: 'Friend request not found' });
    }
    
    // Start transaction
    const addFriend1 = db.prepare('INSERT INTO friends (userId, friendId) VALUES (?, ?)');
    const addFriend2 = db.prepare('INSERT INTO friends (userId, friendId) VALUES (?, ?)');
    const updateRequest = db.prepare('UPDATE friend_requests SET status = "accepted" WHERE id = ?');
    
    // Add both friendship entries (bidirectional)
    addFriend1.run(request.receiverId, request.senderId);
    addFriend2.run(request.senderId, request.receiverId);
    updateRequest.run(requestId);
    
    res.json({ success: true, message: 'Friend request accepted' });
  } catch (error) {
    console.error('Accept friend request error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Reject friend request
app.post('/api/friends/reject/:requestId', (req, res) => {
  try {
    const { requestId } = req.params;
    
    const stmt = db.prepare('UPDATE friend_requests SET status = "rejected" WHERE id = ? AND status = "pending"');
    const result = stmt.run(requestId);
    
    if (result.changes === 0) {
      return res.status(404).json({ error: 'Friend request not found' });
    }
    
    res.json({ success: true, message: 'Friend request rejected' });
  } catch (error) {
    console.error('Reject friend request error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Remove friend
app.delete('/api/friends/:userId/:friendUserId', (req, res) => {
  try {
    const { userId, friendUserId } = req.params;
    
    const userStmt = db.prepare('SELECT id FROM users WHERE userId = ?');
    const user = userStmt.get(userId);
    const friend = userStmt.get(friendUserId);
    
    if (!user || !friend) {
      return res.status(404).json({ error: 'User not found' });
    }
    
    const deleteStmt = db.prepare('DELETE FROM friends WHERE (userId = ? AND friendId = ?) OR (userId = ? AND friendId = ?)');
    deleteStmt.run(user.id, friend.id, friend.id, user.id);
    
    res.json({ success: true, message: 'Friend removed' });
  } catch (error) {
    console.error('Remove friend error:', error);
    res.status(500).json({ error: 'Server error' });
  }
});

// Health check
app.get('/api/health', (req, res) => {
  res.json({ status: 'ok', message: 'ProjectXS Backend Server Running' });
});

// Start server
app.listen(PORT, () => {
  console.log(`🚀 ProjectXS Backend Server running on http://localhost:${PORT}`);
  console.log(`📊 Database: projectxs.db`);
  console.log(`✅ API Ready`);
});

// Graceful shutdown
process.on('SIGINT', () => {
  db.close();
  console.log('\n👋 Server shutting down...');
  process.exit(0);
});
