#!/bin/bash
set -e  # Exit immediately if a command fails

# Variables
REPO_URL="https://github.com/bokshi-gh/http-library"
TMP_DIR=$(mktemp -d)
BUILD_DIR="$TMP_DIR/build"

echo "Cloning repository into temporary folder..."
git clone "$REPO_URL" "$TMP_DIR"

echo "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Running CMake..."
cmake ..

echo "Building project..."
make -j$(nproc)

echo "Installing project (requires sudo)..."
sudo make install

echo "Cleaning up temporary folder..."
rm -rf "$TMP_DIR"

echo "HTTPLibrary installed successfully!"
