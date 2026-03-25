
//  HUD.cpp                           


#include "HUD.hpp"
#include "Renderer.hpp"
#include "Palette.hpp"
#include "Constants.hpp"
#include <algorithm>
#include <string>

void HUD::draw(sf::RenderTarget& rt, const sf::Font& font,
               const Player& player, const std::string& levelName,
               float playerX, float goalX,
               int levelIdx, int totalLevels)
{
    drawScorePanel(rt, font, player, levelName);
    drawLives(rt, player.lives);
    drawProgressPanel(rt, font, playerX, goalX, levelIdx, totalLevels);
}

//  Score panel 
void HUD::drawScorePanel(sf::RenderTarget& rt, const sf::Font& font,
                          const Player& player,
                          const std::string& levelName)
{
    constexpr float PX = 10.f, PY = 10.f;
    constexpr float PW = 245.f, PH = 58.f;

    Renderer::drawPanel(rt, PX, PY, PW, PH);
    Renderer::drawPanelHeader(rt, PX, PY, PW, 26.f);

    Renderer::drawText(rt, font, levelName,
                       PX + 8.f, PY + 4.f, 13,
                       Palette::TEXT_GOLD, true);

    Renderer::drawTextShadow(rt, font,
                              "Score: " + std::to_string(player.score),
                              PX + 10.f, PY + 30.f, 20,
                              Palette::TEXT_MAIN, true);
}

// Hearts panel
void HUD::drawLives(sf::RenderTarget& rt, int lives)
{
    constexpr float HEART_SZ  = 22.f;
    constexpr float SPACING   = 32.f;
    constexpr float TOTAL_W   = Config::PLAYER_LIVES * SPACING - 4.f;

    float startX = Config::SCREEN_W / 2.f - TOTAL_W / 2.f;
    constexpr float Y = 12.f;

    Renderer::drawPanel(rt, startX - 10.f, Y - 4.f,
                        TOTAL_W + 20.f, HEART_SZ + 16.f);

    for (int i = 0; i < Config::PLAYER_LIVES; ++i)
    {
        sf::Color c = (i < lives) ? Palette::HEART_ON : Palette::HEART_OFF;
        Renderer::drawHeart(rt, startX + i * SPACING, Y + 2.f, HEART_SZ, c);
    }
}

// Progress bar
void HUD::drawProgressPanel(sf::RenderTarget& rt, const sf::Font& font,
                             float playerX, float goalX,
                             int levelIdx, int totalLevels)
{
    constexpr float PW = 215.f, PH = 58.f;
    float PX = Config::SCREEN_W - PW - 10.f;
    constexpr float PY = 10.f;

    Renderer::drawPanel(rt, PX, PY, PW, PH);
    Renderer::drawPanelHeader(rt, PX, PY, PW, 26.f);

    std::string badge = "Level " + std::to_string(levelIdx + 1)
                      + " / "   + std::to_string(totalLevels);
    Renderer::drawText(rt, font, badge,
                       PX + 8.f, PY + 4.f, 13,
                       Palette::TEXT_GOLD, true);

    // Bar track
    constexpr float BX = 8.f, BY = 32.f;
    constexpr float BW = PW - 16.f, BH = 14.f;
    rt.draw(Renderer::rect(PX + BX, PY + BY, BW, BH, Palette::HUD_BAR_BG));

    float progress = std::min(1.f, playerX / std::max(1.f, goalX));
    if (progress > 0.f)
        rt.draw(Renderer::rect(PX + BX, PY + BY,
                               BW * progress, BH, Palette::HUD_BAR_FG));

    // Marker on bar
    float mxPos = PX + BX + BW * progress - 4.f;
    rt.draw(Renderer::rect(mxPos, PY + BY - 2.f, 8.f, BH + 4.f,
                           sf::Color(255, 215, 50)));

    Renderer::drawText(rt, font, "GOAL",
                       PX + PW - 38.f, PY + BY, 11,
                       Palette::TEXT_DIM);
}
