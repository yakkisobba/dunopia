#pragma once

// TriviaOverlay.hpp                     


#include <SFML/Graphics.hpp>
#include "QuizManager.hpp"

class TriviaOverlay
{
public:
    static void draw(sf::RenderTarget& rt, const sf::Font& font,
                     const Question& q,
                     int   selectedAnswer,
                     bool  answered,
                     bool  correct,
                     float feedbackTimer);
};
