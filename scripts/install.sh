#!/usr/bin/env bash
set -e

REPO_URL="https://github.com/bokshi-gh/http-library.git"
LIB_NAME="http_library"
PREFIX="/usr/local"

TMP_DIR=$(mktemp -d)

GREEN='\033[0;32m'
RESET='\033[0m'

echo -e "${GREEN}Cloning repository...${RESET}"
git clone --depth 1 "$REPO_URL" "$TMP_DIR"
cd "$TMP_DIR"

echo -e "${GREEN}Building $LIB_NAME...${RESET}"
make

echo -e "${GREEN}Installing $LIB_NAME system-wide to $PREFIX...${RESET}"
make install

cd /
rm -rf "$TMP_DIR"

echo -e "${GREEN}Installation complete!${RESET}"
