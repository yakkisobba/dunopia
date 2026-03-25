#pragma once

//  Renderer.hpp                        


#include <SFML/Graphics.hpp>
#include <string>
#include "Constants.hpp"
#include "Palette.hpp"

namespace Renderer
{
    // Primitives
    sf::RectangleShape rect(float x, float y, float w, float h, sf::Color fill);

    void drawText(sf::RenderTarget& rt, const sf::Font& font,
                  const std::string& str,
                  float x, float y, unsigned size,
                  sf::Color color, bool bold = false, bool centered = false);

    void drawTextShadow(sf::RenderTarget& rt, const sf::Font& font,
                        const std::string& str,
                        float x, float y, unsigned size,
                        sf::Color color, bool bold = false, bool centered = false);

    // Growtopia-style UI panels
    void drawPanel(sf::RenderTarget& rt, float x, float y, float w, float h);
    void drawPanelHeader(sf::RenderTarget& rt, float x, float y, float w, float h = 44.f);
    void drawButton(sf::RenderTarget& rt, float x, float y, float w, float h,
                    sf::Color base, bool pressed = false);

    // Tile drawing
    void drawBevelBlock(sf::RenderTarget& rt, float x, float y, float w, float h, sf::Color base);
    void drawGroundTile(sf::RenderTarget& rt, float x, float y);
    void drawBambooTile(sf::RenderTarget& rt, float x, float y);
    void drawStoneTile(sf::RenderTarget& rt, float x, float y);
    void drawQBlockTile(sf::RenderTarget& rt, float x, float y, bool used, bool bumped);

    // World objects
    void drawFlag(sf::RenderTarget& rt, float x, float groundY);
    void drawHeart(sf::RenderTarget& rt, float x, float y, float size, sf::Color c);

    // Sprites
    void drawRizal(sf::RenderTarget& rt, float ox, float oy, float scale = 1.f);
    void drawEnemy(sf::RenderTarget& rt, float ox, float oy, float scale = 1.f);
}
