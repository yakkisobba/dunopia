#pragma once

// ╔══════════════════════════════════════════════════════════════════╗
// ║                   DUNOPIA — Constants.hpp                        ║
// ╚══════════════════════════════════════════════════════════════════╝

namespace Config
{
    // Window
    constexpr int   SCREEN_W       = 960;
    constexpr int   SCREEN_H       = 540;
    constexpr int   FPS            = 60;

    // World / Tile
    constexpr int   TILE           = 48;
    constexpr int   LEVEL_COLS     = 62;
    constexpr int   LEVEL_ROWS     = 12;

    // Physics
    constexpr float GRAVITY        = 0.55f;
    constexpr float JUMP_VEL       = -13.5f;
    constexpr float MOVE_SPD       = 4.5f;
    constexpr float ENEMY_SPD      = 1.4f;
    constexpr float MAX_FALL       = 18.0f;

    // Player
    constexpr int   PLAYER_W       = 28;
    constexpr int   PLAYER_H       = 48;
    constexpr int   PLAYER_LIVES   = 3;
    constexpr int   INV_FRAMES     = 90;

    // Enemy
    constexpr int   ENEMY_W        = 28;
    constexpr int   ENEMY_H        = 48;

    // Scoring
    constexpr int   PTS_STOMP      = 50;
    constexpr int   PTS_CORRECT    = 100;
    constexpr int   PTS_WRONG      = -25;
    constexpr int   PTS_FLAG       = 300;

    // Trivia
    constexpr int   FEEDBACK_FRAMES = 130;
}

// ── Tile map legend ────────────────────────────────────────────────────
//   '.'  air
//   '#'  ground / dirt  (grass-topped)
//   'B'  bamboo brick
//   'S'  stone brick
//   'Q'  question block  (triggers trivia when hit from below)
//   'F'  flag pole base  (visual only — goal uses a hitbox)
