#pragma once

// ╔══════════════════════════════════════════════════════════════════╗
// ║                      DUNOPIA — HUD.hpp                           ║
// ╚══════════════════════════════════════════════════════════════════╝

#include <SFML/Graphics.hpp>
#include <string>
#include "Player.hpp"

class HUD
{
public:
    static void draw(sf::RenderTarget& rt, const sf::Font& font,
                     const Player& player,
                     const std::string& levelName,
                     float playerX, float goalX,
                     int levelIdx, int totalLevels);

private:
    static void drawScorePanel(sf::RenderTarget& rt, const sf::Font& font,
                               const Player& player,
                               const std::string& levelName);

    static void drawLives(sf::RenderTarget& rt, int lives);

    static void drawProgressPanel(sf::RenderTarget& rt, const sf::Font& font,
                                  float playerX, float goalX,
                                  int levelIdx, int totalLevels);
};
