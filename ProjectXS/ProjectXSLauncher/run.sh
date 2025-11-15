#!/bin/bash

# ProjectXS Launcher - Easy Run Script

echo "======================================"
echo "  ProjectXS Launcher Setup & Run"
echo "======================================"
echo ""

# Check if we're in the right directory
if [ ! -f "package.json" ]; then
    echo "❌ Error: Not in ProjectXSLauncher directory"
    echo "Please run this from: /Users/jasonzhao/Documents/Unreal Projects/ProjectXS/ProjectXSLauncher"
    exit 1
fi

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo "❌ Error: Node.js is not installed"
    echo ""
    echo "Please install Node.js:"
    echo "  1. Visit: https://nodejs.org/"
    echo "  2. Download LTS version"
    echo "  3. Run installer"
    echo "  4. Restart terminal"
    exit 1
fi

echo "✅ Node.js found: $(node --version)"
echo ""

# Check if node_modules exists
if [ ! -d "node_modules" ]; then
    echo "📦 Installing dependencies..."
    echo "This will take 2-3 minutes on first run..."
    echo ""
    npm install
    
    if [ $? -ne 0 ]; then
        echo ""
        echo "❌ Installation failed"
        echo "Try manually:"
        echo "  npm install"
        exit 1
    fi
    
    echo ""
    echo "✅ Dependencies installed successfully!"
    echo ""
else
    echo "✅ Dependencies already installed"
    echo ""
fi

# Run the launcher
echo "🚀 Starting ProjectXS Launcher..."
echo ""
echo "You should see:"
echo "  1. Loading screen with rotating logo"
echo "  2. Main menu with character selection"
echo "  3. Click 'LAUNCH GAME' to start"
echo ""
echo "Press Ctrl+C to stop the launcher"
echo ""
echo "======================================"
echo ""

npm start
