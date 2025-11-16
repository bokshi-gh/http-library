#!/bin/bash

GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}[CLEANING BUILD AND DIST DIRECTORIES]${NC}"
rm -rfv build dist
echo -e "${GREEN}[CLEAN COMPLETE]${NC}"
