#pragma once

// ╔══════════════════════════════════════════════════════════════════╗
// ║                   DUNOPIA — QuizManager.hpp                      ║
// ╚══════════════════════════════════════════════════════════════════╝

#include <string>
#include <array>
#include <vector>

struct Question
{
    std::string              text;
    std::array<std::string, 4> choices;
    int                      correctIndex;
};

class QuizManager
{
public:
    std::vector<Question> bank;

    QuizManager();

    const Question& get(int idx) const;
    bool checkAnswer(int qi, int answer) const;
    int  size() const;
};
