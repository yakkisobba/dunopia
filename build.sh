#!/bin/bash
# DUNOPIA v4 — Build Script
set -e
echo "Building DUNOPIA..."

SRCS="src/main.cpp src/Renderer.cpp src/Entity.cpp src/Player.cpp \
      src/Enemy.cpp src/QuizManager.cpp src/Level.cpp src/Particle.cpp \
      src/GameEngine.cpp src/HUD.cpp src/TriviaOverlay.cpp"

g++ -O2 -std=c++17 $SRCS -o dunopia -Iinclude \
    $(pkg-config --cflags --libs sfml-graphics sfml-window sfml-system)

echo "Done! Run: ./dunopia"
