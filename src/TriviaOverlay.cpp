
//  TriviaOverlay.cpp                     

#include "TriviaOverlay.hpp"
#include "Renderer.hpp"
#include "Palette.hpp"
#include "Constants.hpp"

void TriviaOverlay::draw(sf::RenderTarget& rt, const sf::Font& font,
                          const Question& q,
                          int   selectedAnswer,
                          bool  answered,
                          bool  correct,
                          float /*feedbackTimer*/)
{
    int W = Config::SCREEN_W;
    int H = Config::SCREEN_H;

    // Dim the world
    rt.draw(Renderer::rect(0, 0, W, H, sf::Color(0, 0, 0, 165)));

    // Panel geometry
    constexpr float PW = 820.f, PH = 390.f;
    float px = (W - PW) / 2.f;
    float py = (H - PH) / 2.f;

    // Outer glow
    rt.draw(Renderer::rect(px - 6, py - 6, PW + 12, PH + 12,
                           sf::Color(255, 185, 40, 45)));

    // Panel
    Renderer::drawPanel(rt, px, py, PW, PH);

    // Header
    Renderer::drawPanelHeader(rt, px, py, PW, 48.f);

    // Decorative dots (GT style)
    sf::Color dots[] = {
        {220,60,60}, {220,160,40}, {60,200,80}, {60,140,220}, {160,60,220}
    };
    for (int i = 0; i < 5; ++i)
        rt.draw(Renderer::rect(px + 10.f + i * 15.f, py + 16.f,
                               11.f, 11.f, dots[i]));

    Renderer::drawTextShadow(rt, font,
                              "ALAM MO BA?",
                              W / 2.f, py + 13.f, 20,
                              Palette::TEXT_GOLD, true, true);

    // Question text
    Renderer::drawText(rt, font, q.text,
                       px + 20.f, py + 60.f, 18,
                       Palette::TEXT_MAIN);

    // Answer buttons
    const char*    keys[]     = {"A", "B", "C", "D"};
    const sf::Color baseCols[] = {
        Palette::UI_BTN_A, Palette::UI_BTN_B,
        Palette::UI_BTN_C, Palette::UI_BTN_D
    };

    for (int i = 0; i < 4; ++i)
    {
        float by = py + 105.f + i * 60.f;
        float bx = px + 16.f;
        float bw = PW - 32.f;
        float bh = 52.f;

        sf::Color bg = baseCols[i];
        if (answered)
        {
            if (i == q.correctIndex)
                bg = Palette::UI_CORRECT;
            else if (i == selectedAnswer)
                bg = Palette::UI_WRONG;
            else
                bg = Palette::UI_DISABLED;
        }

        Renderer::drawButton(rt, bx, by, bw, bh, bg);

        // Key badge
        rt.draw(Renderer::rect(bx + 4.f, by + 6.f, 30.f, bh - 12.f,
                               sf::Color(0, 0, 0, 80)));
        Renderer::drawTextShadow(rt, font, keys[i],
                                  bx + 10.f, by + 14.f, 20,
                                  Palette::TEXT_MAIN, true);

        // Choice label
        bool highlight = answered && (i == q.correctIndex || i == selectedAnswer);
        Renderer::drawText(rt, font, q.choices[i],
                           bx + 48.f, by + 16.f, 17,
                           highlight ? Palette::WHITE : Palette::TEXT_MAIN,
                           true);
    }

    // Feedback / hint bar
    float fbY = py + PH - 38.f;
    rt.draw(Renderer::rect(px, fbY - 4.f, PW, 42.f, Palette::UI_HEADER));
    rt.draw(Renderer::rect(px, fbY - 4.f, PW,  2.f, Palette::UI_BORDER2));

    if (!answered)
    {
        Renderer::drawText(rt, font,
                            "Press  A / B / C / D   or   1 / 2 / 3 / 4  to answer",
                            W / 2.f, fbY + 2.f, 14,
                            Palette::TEXT_DIM, false, true);
    }
    else
    {
        const char* key = keys[q.correctIndex];
        std::string fb  = correct
            ? "  TAMA!  +100 points!"
            : std::string("  MALI!  -25 points   |   Correct: ") + key;
        sf::Color fc = correct ? Palette::UI_CORRECT : Palette::UI_WRONG;
        Renderer::drawTextShadow(rt, font, fb,
                                  W / 2.f, fbY, 19,
                                  fc, true, true);
    }
}
