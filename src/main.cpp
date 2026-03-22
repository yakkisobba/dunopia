// ╔══════════════════════════════════════════════════════════════════╗
// ║                       DUNOPIA — main.cpp                         ║
// ╚══════════════════════════════════════════════════════════════════╝

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include "Constants.hpp"
#include "Palette.hpp"
#include "Renderer.hpp"
#include "GameEngine.hpp"
#include "HUD.hpp"
#include "TriviaOverlay.hpp"
#include "Particle.hpp"

// ── Font loader ───────────────────────────────────────────────────────
static sf::Font loadFont()
{
    sf::Font f;
    const char* paths[] = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        nullptr
    };
    for (int i = 0; paths[i]; ++i)
        if (f.loadFromFile(paths[i])) break;
    return f;
}

// ══════════════════════════════════════════════════════════════════════
//  TITLE SCREEN
// ══════════════════════════════════════════════════════════════════════
static void drawTitle(sf::RenderTarget& rt, const sf::Font& font, float timer)
{
    int W = Config::SCREEN_W, H = Config::SCREEN_H;

    // Sky gradient
    for (int i = 0; i < H; i += 4)
    {
        float t = static_cast<float>(i) / H;
        rt.draw(Renderer::rect(0.f, static_cast<float>(i),
            static_cast<float>(W), 5.f,
            sf::Color(
                static_cast<sf::Uint8>(88  + t * 42),
                static_cast<sf::Uint8>(178 - t * 55),
                static_cast<sf::Uint8>(228 - t * 88)
            )));
    }

    // Animated sun
    float sunX = W - 145.f;
    float sunY = 58.f + std::sin(timer * 0.8f) * 7.f;
    sf::CircleShape sun(46.f);
    sun.setFillColor(sf::Color(255, 215, 45));
    sun.setPosition(sunX - 46.f, sunY - 46.f);
    rt.draw(sun);
    for (int r = 0; r < 8; ++r)
    {
        float ang = r * 3.14159f / 4.f + timer * 0.4f;
        float rx  = sunX + std::cos(ang) * 62.f - 5.f;
        float ry  = sunY + std::sin(ang) * 62.f - 5.f;
        rt.draw(Renderer::rect(rx, ry, 10.f, 10.f,
                               sf::Color(255, 240, 80, 165)));
    }

    // Hills (two layers)
    for (int layer = 0; layer < 2; ++layer)
    {
        float drift = std::sin(timer * 0.1f + layer) * 4.f;
        sf::Color hc = layer == 0
            ? sf::Color(20, 100, 20, 140)
            : sf::Color(30, 130, 30, 100);
        for (int m = 0; m < 7; ++m)
        {
            float mx = m * 155.f + drift;
            float mh = 80.f + std::sin(m * 1.8f + layer) * 36.f;
            sf::ConvexShape hill; hill.setPointCount(3);
            hill.setPoint(0, {mx,          static_cast<float>(H)});
            hill.setPoint(1, {mx + 155.f,  static_cast<float>(H)});
            hill.setPoint(2, {mx + 77.5f,  static_cast<float>(H) - mh});
            hill.setFillColor(hc);
            rt.draw(hill);
        }
    }

    // Ground strip
    rt.draw(Renderer::rect(0.f, H - 72.f, W, 72.f, Palette::DIRT));
    rt.draw(Renderer::rect(0.f, H - 72.f, W,  9.f, Palette::GRASS));
    rt.draw(Renderer::rect(0.f, H - 72.f, W,  3.f, sf::Color(60, 185, 60)));
    for (int gx = 8; gx < W - 4; gx += 16)
        rt.draw(Renderer::rect(static_cast<float>(gx), H - 76.f,
                               3.f, 6.f, sf::Color(22, 125, 22)));

    // Large Rizal on left
    Renderer::drawRizal(rt, 72.f, H - 72.f - 48.f, 2.f);

    // ── Title panel ───────────────────────────────────────────────────
    constexpr float TPW = 590.f, TPH = 158.f;
    float tpx = W / 2.f - TPW / 2.f + 65.f;
    float tpy = 48.f;
    Renderer::drawPanel(rt, tpx, tpy, TPW, TPH);
    Renderer::drawPanelHeader(rt, tpx, tpy, TPW, 48.f);

    // GT color dots
    sf::Color titleDots[] = {
        {220,50,50},{220,150,40},{50,200,70},{50,130,220},{150,50,220}
    };
    for (int i = 0; i < 5; ++i)
        rt.draw(Renderer::rect(tpx + 10.f + i * 16.f, tpy + 16.f,
                               12.f, 12.f, titleDots[i]));

    Renderer::drawTextShadow(rt, font, "DUNOPIA",
        W / 2.f + 65.f, tpy + 55.f, 66,
        Palette::TEXT_GOLD, true, true);
    Renderer::drawText(rt, font, "A Filipino History Platformer",
        W / 2.f + 65.f, tpy + 126.f, 20,
        Palette::TEXT_DIM, false, true);

    // ── Controls panel ────────────────────────────────────────────────
    constexpr float CPW = 560.f, CPH = 124.f;
    float cpx = W / 2.f - CPW / 2.f + 65.f;
    float cpy = static_cast<float>(H) - CPH - 20.f;
    Renderer::drawPanel(rt, cpx, cpy, CPW, CPH);
    Renderer::drawPanelHeader(rt, cpx, cpy, CPW, 26.f);
    Renderer::drawText(rt, font, "HOW TO PLAY",
                       cpx + 8.f, cpy + 5.f, 14, Palette::TEXT_GOLD, true);

    struct Row { const char* key; const char* desc; };
    Row rows[] = {
        {"Arrow Keys / WASD",  "Move & Jump"},
        {"Jump on enemy",      "Stomp to defeat  (+50 pts)"},
        {"Bump ? from below",  "Trivia question appears"},
        {"A  B  C  D",         "Answer trivia  (+100 / -25 pts)"},
    };
    for (int i = 0; i < 4; ++i)
    {
        float ry = cpy + 35.f + i * 22.f;
        Renderer::drawText(rt, font, rows[i].key,
                           cpx + 12.f, ry, 14, Palette::TEXT_GOLD, true);
        Renderer::drawText(rt, font, std::string("—  ") + rows[i].desc,
                           cpx + 195.f, ry, 14, Palette::TEXT_MAIN);
    }

    // Blinking start prompt
    if (static_cast<int>(timer * 2.f) % 2 == 0)
        Renderer::drawTextShadow(rt, font,
            "Press  ENTER  or  SPACE  to Start",
            W / 2.f + 65.f, static_cast<float>(H) - 215.f, 26,
            Palette::TEXT_GOLD, true, true);
}

// ══════════════════════════════════════════════════════════════════════
//  LEVEL CLEAR
// ══════════════════════════════════════════════════════════════════════
static void drawLevelClear(sf::RenderTarget& rt, const sf::Font& font,
                            int score, int levelIdx, int total,
                            const ParticleSystem& fx)
{
    int W = Config::SCREEN_W, H = Config::SCREEN_H;
    rt.draw(Renderer::rect(0, 0, W, H, sf::Color(0, 0, 0, 178)));
    fx.drawScreen(rt);

    constexpr float PW = 460.f, PH = 200.f;
    float px = W / 2.f - PW / 2.f;
    float py = H / 2.f - PH / 2.f;
    Renderer::drawPanel(rt, px, py, PW, PH);
    Renderer::drawPanelHeader(rt, px, py, PW, 46.f);

    Renderer::drawTextShadow(rt, font, "LEVEL CLEAR!",
        W / 2.f, py + 10.f, 24, Palette::TEXT_GOLD, true, true);
    Renderer::drawText(rt, font,
        "Level " + std::to_string(levelIdx + 1) + " of " + std::to_string(total),
        W / 2.f, py + 62.f, 18, Palette::TEXT_DIM, false, true);
    Renderer::drawTextShadow(rt, font,
        "Score: " + std::to_string(score),
        W / 2.f, py + 92.f, 30, Palette::TEXT_MAIN, true, true);
    Renderer::drawText(rt, font, "Loading next level...",
        W / 2.f, py + 152.f, 16, Palette::TEXT_DIM, false, true);
}

// ══════════════════════════════════════════════════════════════════════
//  GAME OVER
// ══════════════════════════════════════════════════════════════════════
static void drawGameOver(sf::RenderTarget& rt, const sf::Font& font,
                          int score)
{
    int W = Config::SCREEN_W, H = Config::SCREEN_H;
    rt.draw(Renderer::rect(0, 0, W, H, sf::Color(0, 0, 0, 212)));

    constexpr float PW = 490.f, PH = 210.f;
    float px = W / 2.f - PW / 2.f;
    float py = H / 2.f - PH / 2.f;
    Renderer::drawPanel(rt, px, py, PW, PH);

    // Red header
    rt.draw(Renderer::rect(px - 1, py - 1, PW + 2, 48.f, sf::Color(100, 15, 15)));
    rt.draw(Renderer::rect(px - 1, py + 46.f, PW + 2, 2.f, sf::Color(160, 30, 30)));

    Renderer::drawTextShadow(rt, font, "PATAY...",
        W / 2.f, py + 10.f, 26, Palette::TEXT_RED, true, true);
    Renderer::drawText(rt, font, "Jose Rizal has fallen.",
        W / 2.f, py + 62.f, 18, Palette::TEXT_DIM, false, true);
    Renderer::drawTextShadow(rt, font,
        "Final Score: " + std::to_string(score),
        W / 2.f, py + 94.f, 28, Palette::TEXT_GOLD, true, true);
    Renderer::drawText(rt, font, "Press  ENTER  or  R  to try again",
        W / 2.f, py + 158.f, 18, Palette::TEXT_MAIN, false, true);
}

// ══════════════════════════════════════════════════════════════════════
//  VICTORY
// ══════════════════════════════════════════════════════════════════════
static void drawVictory(sf::RenderTarget& rt, const sf::Font& font,
                         int score, const ParticleSystem& fx)
{
    int W = Config::SCREEN_W, H = Config::SCREEN_H;

    // Warm sunrise gradient
    for (int i = 0; i < H; i += 4)
    {
        float t = static_cast<float>(i) / H;
        rt.draw(Renderer::rect(0.f, static_cast<float>(i),
            static_cast<float>(W), 5.f,
            sf::Color(
                static_cast<sf::Uint8>(218 * (1.f - t * 0.4f)),
                static_cast<sf::Uint8>(128 + 82 * t),
                static_cast<sf::Uint8>(18  + 32 * t)
            )));
    }

    Renderer::drawRizal(rt, W / 2.f - 28.f, H / 2.f - 55.f, 2.f);
    fx.drawScreen(rt);

    constexpr float PW = 530.f, PH = 185.f;
    float px = W / 2.f - PW / 2.f;
    float py = 36.f;
    Renderer::drawPanel(rt, px, py, PW, PH);

    // Gold header
    rt.draw(Renderer::rect(px - 1, py - 1, PW + 2, 50.f, sf::Color(85, 58, 5)));
    rt.draw(Renderer::rect(px - 1, py + 48.f, PW + 2, 2.f, Palette::UI_BORDER2));

    Renderer::drawTextShadow(rt, font, "KALAYAAN!",
        W / 2.f, py + 12.f, 28, Palette::TEXT_GOLD, true, true);
    Renderer::drawText(rt, font, "Jose Rizal has reached freedom!",
        W / 2.f, py + 64.f, 18, Palette::TEXT_MAIN, false, true);
    Renderer::drawTextShadow(rt, font,
        "Final Score: " + std::to_string(score),
        W / 2.f, py + 96.f, 28, Palette::TEXT_GOLD, true, true);
    Renderer::drawText(rt, font, "Press  ENTER  to return to title",
        W / 2.f, py + 146.f, 16, Palette::TEXT_DIM, false, true);

    // Quote panel
    constexpr float QPW = 570.f;
    float qpx = W / 2.f - QPW / 2.f;
    float qpy = static_cast<float>(H) - 76.f;
    Renderer::drawPanel(rt, qpx, qpy, QPW, 58.f);
    Renderer::drawText(rt, font,
        "\"The youth is the hope of the fatherland.\"",
        W / 2.f, qpy + 10.f, 16, Palette::TEXT_DIM, false, true);
    Renderer::drawText(rt, font, "— Jose Rizal",
        W / 2.f, qpy + 34.f, 15, Palette::TEXT_GOLD, true, true);
}

// ══════════════════════════════════════════════════════════════════════
//  MAIN
// ══════════════════════════════════════════════════════════════════════
int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(
        sf::VideoMode(Config::SCREEN_W, Config::SCREEN_H),
        "DUNOPIA — Filipino History Adventure",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(Config::FPS);

    const sf::Font font    = loadFont();
    GameEngine     engine;
    ParticleSystem victoryFX;
    float          victoryTimer = 0.f;
    bool           prevJump     = false;

    while (window.isOpen())
    {
        // ── Events ───────────────────────────────────────────────────
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();

            if (ev.type == sf::Event::KeyPressed)
            {
                switch (engine.phase)
                {
                    case Phase::TITLE:
                        if (ev.key.code == sf::Keyboard::Enter ||
                            ev.key.code == sf::Keyboard::Space)
                            engine.startGame();
                        break;

                    case Phase::TRIVIA: {
                        int choice = -1;
                        if (ev.key.code == sf::Keyboard::A ||
                            ev.key.code == sf::Keyboard::Num1) choice = 0;
                        if (ev.key.code == sf::Keyboard::B ||
                            ev.key.code == sf::Keyboard::Num2) choice = 1;
                        if (ev.key.code == sf::Keyboard::C ||
                            ev.key.code == sf::Keyboard::Num3) choice = 2;
                        if (ev.key.code == sf::Keyboard::D ||
                            ev.key.code == sf::Keyboard::Num4) choice = 3;
                        if (choice >= 0) engine.submitAnswer(choice);
                        break;
                    }

                    case Phase::GAME_OVER:
                        if (ev.key.code == sf::Keyboard::Enter ||
                            ev.key.code == sf::Keyboard::R)
                            engine.startGame();
                        break;

                    case Phase::VICTORY:
                        if (ev.key.code == sf::Keyboard::Enter)
                        {
                            engine.phase = Phase::TITLE;
                            victoryFX.clear();
                            victoryTimer = 0.f;
                        }
                        break;

                    default: break;
                }
            }
        }

        // ── Input ─────────────────────────────────────────────────────
        bool left  = sf::Keyboard::isKeyPressed(sf::Keyboard::Left)  ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool jump  = sf::Keyboard::isKeyPressed(sf::Keyboard::Up)    ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::W)     ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

        bool jumpEdge = jump && !prevJump;
        prevJump      = jump;

        engine.update(left, right, jumpEdge);

        // ── Victory fireworks ─────────────────────────────────────────
        if (engine.phase == Phase::VICTORY)
        {
            victoryTimer += 1.f / Config::FPS;
            victoryFX.update();
            if (static_cast<int>(victoryTimer * 60.f) % 7 == 0)
            {
                float rx = static_cast<float>(std::rand() % Config::SCREEN_W);
                float ry = static_cast<float>(std::rand() % (Config::SCREEN_H / 2));
                sf::Color cols[] = {
                    Palette::TEXT_GOLD,   Palette::UI_CORRECT,
                    {100,180,255,255},    {255,120,200,255}
                };
                victoryFX.burst(rx, ry, cols[std::rand() % 4], 22, 4.5f);
            }
        }

        // ── Render ────────────────────────────────────────────────────
        window.clear(sf::Color(10, 8, 6));

        switch (engine.phase)
        {
            // ── Title ──────────────────────────────────────────────────
            case Phase::TITLE:
                drawTitle(window, font, engine.titleTimer);
                break;

            // ── In-game ────────────────────────────────────────────────
            case Phase::PLAYING:
            case Phase::TRIVIA:
            case Phase::FEEDBACK:
            case Phase::LEVEL_CLEAR:
            {
                engine.level->drawBackground(window, engine.camX);
                engine.level->drawTerrain(window, engine.camX);
                engine.level->drawFlag(window, engine.camX);

                for (const auto& en : engine.level->enemies)
                    en.draw(window, engine.camX);

                engine.player.draw(window, engine.camX);

                if (engine.phase == Phase::PLAYING)
                    engine.particles.draw(window, engine.camX);

                HUD::draw(window, font,
                          engine.player,
                          engine.level->def.name,
                          engine.player.x,
                          engine.goalX(),
                          engine.levelIdx,
                          engine.totalLevels());

                if (engine.phase == Phase::TRIVIA ||
                    engine.phase == Phase::FEEDBACK)
                {
                    const Question& q =
                        engine.quizMgr.get(engine.trivia.questionIdx);
                    TriviaOverlay::draw(window, font, q,
                                       engine.trivia.selectedAnswer,
                                       engine.trivia.answered,
                                       engine.trivia.correct,
                                       engine.trivia.feedbackTimer);
                    engine.particles.drawScreen(window);
                }

                if (engine.phase == Phase::LEVEL_CLEAR)
                    drawLevelClear(window, font,
                                   engine.player.score,
                                   engine.levelIdx,
                                   engine.totalLevels(),
                                   engine.particles);
                break;
            }

            // ── Game Over ──────────────────────────────────────────────
            case Phase::GAME_OVER:
                drawGameOver(window, font, engine.player.score);
                break;

            // ── Victory ────────────────────────────────────────────────
            case Phase::VICTORY:
                drawVictory(window, font, engine.player.score, victoryFX);
                break;
        }

        window.display();
    }

    return 0;
}
