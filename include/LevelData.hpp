#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Level.hpp"


//  LevelData.hpp


namespace LevelData {

    // Tile map strings for Level 1 and Level 2
    extern const std::vector<std::string> Level1Map;
    extern const std::vector<std::string> Level2Map;

    // Factory: build a ready-to-use Level object by index (0 or 1)
    std::unique_ptr<Level> make(int index);

} // namespace LevelData