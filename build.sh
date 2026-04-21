#!/usr/bin/env bash
set -e
g++ -std=c++17 -Wall -Wextra -o main main.cpp
echo "Build successful: ./main"