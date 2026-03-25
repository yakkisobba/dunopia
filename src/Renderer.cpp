
//  Renderer.cpp                        

#include "Renderer.hpp"
#include <algorithm>
#include <cmath>

namespace Renderer
{

//  Primitives
sf::RectangleShape rect(float x, float y, float w, float h, sf::Color fill)
{
    sf::RectangleShape r({w, h});
    r.setPosition(x, y);
    r.setFillColor(fill);
    return r;
}

void drawText(sf::RenderTarget& rt, const sf::Font& font,
              const std::string& str,
              float x, float y, unsigned size,
              sf::Color color, bool bold, bool centered)
{
    sf::Text t;
    t.setFont(font);
    t.setString(str);
    t.setCharacterSize(size);
    t.setFillColor(color);
    if (bold) t.setStyle(sf::Text::Bold);
    float ox = centered ? x - t.getLocalBounds().width / 2.f : x;
    t.setPosition(ox, y);
    rt.draw(t);
}

void drawTextShadow(sf::RenderTarget& rt, const sf::Font& font,
                    const std::string& str,
                    float x, float y, unsigned size,
                    sf::Color color, bool bold, bool centered)
{
    sf::Text t;
    t.setFont(font);
    t.setString(str);
    t.setCharacterSize(size);
    if (bold) t.setStyle(sf::Text::Bold);
    float ox = centered ? x - t.getLocalBounds().width / 2.f : x;

    t.setFillColor(sf::Color(0, 0, 0, 180));
    t.setPosition(ox + 2.f, y + 2.f);
    rt.draw(t);

    t.setFillColor(color);
    t.setPosition(ox, y);
    rt.draw(t);
}

//  Panels
void drawPanel(sf::RenderTarget& rt, float x, float y, float w, float h)
{
    rt.draw(rect(x - 3, y - 3, w + 6, h + 6, Palette::UI_BORDER));
    rt.draw(rect(x - 1, y - 1, w + 2, h + 2, Palette::UI_BORDER2));
    rt.draw(rect(x, y, w, h, Palette::UI_BG));
    rt.draw(rect(x + 2, y + 2, w - 4, 2, sf::Color(255, 200, 80, 35)));
}

void drawPanelHeader(sf::RenderTarget& rt, float x, float y, float w, float h)
{
    rt.draw(rect(x, y, w, h, Palette::UI_HEADER));
    rt.draw(rect(x + 2, y + 2, w - 4, h * 0.5f, Palette::UI_HEADER2));
    rt.draw(rect(x, y + h - 2, w, 2, Palette::UI_BORDER2));
}

void drawButton(sf::RenderTarget& rt, float x, float y, float w, float h,
                sf::Color base, bool pressed)
{
    auto lighten = [](sf::Color c, int amt) {
        return sf::Color(
            static_cast<sf::Uint8>(std::min(255, c.r + amt)),
            static_cast<sf::Uint8>(std::min(255, c.g + amt)),
            static_cast<sf::Uint8>(std::min(255, c.b + amt)));
    };
    auto darken = [](sf::Color c, int amt) {
        return sf::Color(
            static_cast<sf::Uint8>(std::max(0, c.r - amt)),
            static_cast<sf::Uint8>(std::max(0, c.g - amt)),
            static_cast<sf::Uint8>(std::max(0, c.b - amt)));
    };

    float shift = pressed ? 2.f : 0.f;
    if (!pressed) rt.draw(rect(x + 2, y + 4, w, h, sf::Color(0, 0, 0, 90)));

    rt.draw(rect(x, y + shift, w, h, base));
    rt.draw(rect(x, y + shift, w, 4, lighten(base, 45)));
    rt.draw(rect(x, y + shift + h - 4, w, 4, darken(base, 55)));
    rt.draw(rect(x, y + shift, 3, h, lighten(base, 45)));
    rt.draw(rect(x + w - 3, y + shift, 3, h, darken(base, 55)));
}

// Tile drawing
void drawBevelBlock(sf::RenderTarget& rt, float x, float y,
                    float w, float h, sf::Color base)
{
    auto lighten = [](sf::Color c, int a) {
        return sf::Color(
            static_cast<sf::Uint8>(std::min(255, c.r + a)),
            static_cast<sf::Uint8>(std::min(255, c.g + a)),
            static_cast<sf::Uint8>(std::min(255, c.b + a)));
    };
    auto darken = [](sf::Color c, int a) {
        return sf::Color(
            static_cast<sf::Uint8>(std::max(0, c.r - a)),
            static_cast<sf::Uint8>(std::max(0, c.g - a)),
            static_cast<sf::Uint8>(std::max(0, c.b - a)));
    };
    rt.draw(rect(x, y, w, h, base));
    rt.draw(rect(x, y, w, 4, lighten(base, 45)));
    rt.draw(rect(x, y + h - 4, w, 4, darken(base, 45)));
    rt.draw(rect(x, y, 4, h, lighten(base, 45)));
    rt.draw(rect(x + w - 4, y, 4, h, darken(base, 45)));
}

void drawGroundTile(sf::RenderTarget& rt, float x, float y)
{
    float T = static_cast<float>(Config::TILE);
    drawBevelBlock(rt, x, y, T, T, Palette::DIRT);
    rt.draw(rect(x, y, T, 8.f, Palette::GRASS));
    rt.draw(rect(x, y, T, 3.f, sf::Color(60, 185, 60)));
    for (float gx = x + 6; gx < x + T - 4; gx += 14.f)
        rt.draw(rect(gx, y - 4.f, 3.f, 6.f, sf::Color(22, 120, 22)));
}

void drawBambooTile(sf::RenderTarget& rt, float x, float y)
{
    float T = static_cast<float>(Config::TILE);
    drawBevelBlock(rt, x, y, T, T, Palette::BAMBOO);
    rt.draw(rect(x, y + T * 0.5f - 1, T, 2, Palette::BAMBOO_DARK));
    rt.draw(rect(x + T * 0.33f, y, 2, T * 0.5f, Palette::BAMBOO_DARK));
    rt.draw(rect(x + T * 0.66f, y + T * 0.5f, 2, T * 0.5f, Palette::BAMBOO_DARK));
}

void drawStoneTile(sf::RenderTarget& rt, float x, float y)
{
    float T = static_cast<float>(Config::TILE);
    drawBevelBlock(rt, x, y, T, T, Palette::STONE);
    rt.draw(rect(x, y + T * 0.5f - 1, T, 2, Palette::STONE_DARK));
    rt.draw(rect(x + T * 0.34f, y, 2, T * 0.5f, Palette::STONE_DARK));
    rt.draw(rect(x + T * 0.67f, y + T * 0.5f, 2, T * 0.5f, Palette::STONE_DARK));
}

void drawQBlockTile(sf::RenderTarget& rt, float x, float y,
                    bool used, bool bumped)
{
    float T  = static_cast<float>(Config::TILE);
    float by = bumped ? y - 5.f : y;

    if (used)
    {
        drawBevelBlock(rt, x, by, T, T, Palette::QBLOCK_USED);
        return;
    }

    drawBevelBlock(rt, x, by, T, T, Palette::QBLOCK_BASE);
    rt.draw(rect(x + 6, by + 6, T - 12, T - 12, sf::Color(255, 235, 90, 70)));

    // "?" shape
    float qx = x + T * 0.32f;
    float qy = by + 6.f;
    rt.draw(rect(qx,            qy,         T * 0.36f, 5.f, Palette::TEXT_GOLD));
    rt.draw(rect(qx + T*0.20f, qy + 4.f,   T * 0.16f, 8.f, Palette::TEXT_GOLD));
    rt.draw(rect(qx + T*0.04f, qy + 11.f,  T * 0.28f, 5.f, Palette::TEXT_GOLD));
    rt.draw(rect(qx + T*0.12f, qy + 20.f,  T * 0.14f, 5.f, Palette::TEXT_GOLD));
}

// Flag
void drawFlag(sf::RenderTarget& rt, float x, float groundY)
{
    float poleH = static_cast<float>(Config::TILE * 7);
    float poleY = groundY - poleH;

    rt.draw(rect(x, poleY, 6.f, poleH, sf::Color(90, 68, 30)));
    rt.draw(rect(x + 1, poleY, 2.f, poleH, sf::Color(140, 105, 50)));

    sf::ConvexShape banner;
    banner.setPointCount(3);
    banner.setPoint(0, {x + 6.f,  poleY});
    banner.setPoint(1, {x + 52.f, poleY + 24.f});
    banner.setPoint(2, {x + 6.f,  poleY + 48.f});
    banner.setFillColor(sf::Color(30, 160, 55));
    rt.draw(banner);

    // Sun on banner
    sf::CircleShape sun(9.f);
    sun.setFillColor(sf::Color(255, 215, 0));
    sun.setPosition(x + 12.f, poleY + 15.f);
    rt.draw(sun);
}

// Hearts
void drawHeart(sf::RenderTarget& rt, float x, float y,
               float size, sf::Color c)
{
    float r  = size * 0.38f;
    float bx = size * 0.32f;

    sf::CircleShape L(r), R(r);
    L.setFillColor(c); L.setPosition(x,      y);
    R.setFillColor(c); R.setPosition(x + bx, y);
    rt.draw(L);
    rt.draw(R);

    sf::ConvexShape tri;
    tri.setPointCount(3);
    tri.setPoint(0, {x,             y + r * 1.2f});
    tri.setPoint(1, {x + size,      y + r * 1.2f});
    tri.setPoint(2, {x + size*0.5f, y + size * 1.1f});
    tri.setFillColor(c);
    rt.draw(tri);
}

// José Rizal sprite
void drawRizal(sf::RenderTarget& rt, float ox, float oy, float scale)
{
    auto px = [&](float x, float y, float w, float h, sf::Color c) {
        rt.draw(rect(ox + x*scale, oy + y*scale, w*scale, h*scale, c));
    };
    px( 4,  0, 20,  7, Palette::HAIR);
    px( 4,  6, 20, 18, Palette::SKIN);
    px( 8, 10,  4,  3, sf::Color(20,20,20));
    px(16, 10,  4,  3, sf::Color(20,20,20));
    px( 9, 18, 10,  2, Palette::HAIR);
    px( 2, 24, 24, 20, Palette::BARONG);
    px(11, 24,  6, 20, Palette::BARONG_LINE);
    px( 4, 28,  2,  8, Palette::BARONG_LINE);
    px(22, 28,  2,  8, Palette::BARONG_LINE);
    px( 0, 24,  2, 16, Palette::SKIN);
    px(26, 24,  2, 16, Palette::SKIN);
    px( 0, 38,  4,  6, Palette::SKIN);
    px(24, 38,  4,  6, Palette::SKIN);
    px( 3, 44, 10, 18, Palette::PANTS);
    px(15, 44, 10, 18, Palette::PANTS);
    px( 3, 43, 22,  3, sf::Color(60,45,15));
    px( 0, 60, 13,  6, Palette::SHOES);
    px(15, 60, 13,  6, Palette::SHOES);
}

// Spanish soldier sprite (enemy)
void drawEnemy(sf::RenderTarget& rt, float ox, float oy, float scale)
{
    auto px = [&](float x, float y, float w, float h, sf::Color c) {
        rt.draw(rect(ox + x*scale, oy + y*scale, w*scale, h*scale, c));
    };
    px( 4,  0, 20,  9, Palette::ENEMY_HELM);
    px( 2,  5, 24,  5, sf::Color(100,75,48));
    px(10,  0,  8,  5, Palette::ENEMY_PLUME);
    px( 5,  8, 18, 14, Palette::SKIN);
    px( 7, 11,  6,  2, Palette::HAIR);
    px(15, 11,  6,  2, Palette::HAIR);
    px( 8, 13,  4,  3, sf::Color(20,20,20));
    px(16, 13,  4,  3, sf::Color(20,20,20));
    px( 9, 18, 10,  2, Palette::HAIR);
    px( 2, 22, 24, 22, Palette::ENEMY_ARMOR);
    px(10, 22,  8, 22, Palette::ENEMY_DARK);
    px(12, 25,  4, 10, Palette::WHITE);
    px( 9, 29, 10,  4, Palette::WHITE);
    px( 0, 22,  2, 16, Palette::SKIN);
    px(26, 22,  2, 16, Palette::SKIN);
    px( 3, 44, 10, 16, Palette::ENEMY_HELM);
    px(15, 44, 10, 16, Palette::ENEMY_HELM);
    px( 1, 57, 13,  7, Palette::SHOES);
    px(14, 57, 13,  7, Palette::SHOES);
}

} 
