#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Level.hpp"

// ============================================================
//  DUNOPIA — LevelData
//
//  All tile-map string data lives here — one namespace per level.
//  This keeps level design 100% separate from game logic.
//
//  Map legend:
//    '.' = air       'G' = ground    'B' = brick
//    'Q' = question  'P' = pipe cap  'p' = pipe body
//    'E' = enemy     'F' = flag/goal
//
//  Map is 14 rows tall.
//    Row  0-1  = open sky
//    Row  2-5  = upper platform / question block zone
//    Row  6-7  = mid-air / enemy zone
//    Row  8-9  = pipe zone / lower platform
//    Row 10    = ground surface row  (player walks here)
//    Row 11-13 = underground fill
//
//  Width = 80 columns  (3200 px @ 40px tile)
// ============================================================

namespace LevelData {

    // Tile map strings for Level 1 and Level 2
    extern const std::vector<std::string> Level1Map;
    extern const std::vector<std::string> Level2Map;

    // Factory: build a ready-to-use Level object by index (0 or 1)
    std::unique_ptr<Level> make(int index);

} // namespace LevelData