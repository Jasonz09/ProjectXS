#!/bin/bash
# Build script for ProjectXS
# This script compiles the project using Unreal's build system

echo "====================================="
echo "ProjectXS Build Script"
echo "====================================="
echo ""

PROJECT_DIR="/Users/jasonzhao/Documents/Unreal Projects/ProjectXS/ProjectXS"
UE_DIR="/Users/Shared/Epic Games/UE_5.7"
BUILD_TOOL="$UE_DIR/Engine/Build/BatchFiles/Mac/Build.sh"

if [ ! -f "$BUILD_TOOL" ]; then
    echo "ERROR: Unreal Build Tool not found at: $BUILD_TOOL"
    echo "Please verify Unreal Engine 5.7 is installed correctly."
    exit 1
fi

cd "$PROJECT_DIR"

echo "Building ProjectXSEditor (Development)..."
echo ""

"$BUILD_TOOL" ProjectXSEditor Mac Development -Project="$PROJECT_DIR/ProjectXS.uproject" -WaitMutex -FromMsBuild

BUILD_RESULT=$?

echo ""
echo "====================================="
if [ $BUILD_RESULT -eq 0 ]; then
    echo "✓ BUILD SUCCESSFUL!"
    echo "All C++ files compiled successfully."
    echo "You can now open ProjectXS.uproject in Unreal Editor."
else
    echo "✗ BUILD FAILED"
    echo "Please check the output above for errors."
    echo "Common issues:"
    echo "  - Missing header files"
    echo "  - Syntax errors in C++ code"
    echo "  - Circular dependencies"
fi
echo "====================================="

exit $BUILD_RESULT
