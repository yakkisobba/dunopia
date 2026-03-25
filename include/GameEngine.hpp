#pragma once

// GameEngine.hpp                       

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.hpp"
#include "Level.hpp"
#include "QuizManager.hpp"
#include "Particle.hpp"

enum class Phase
{
    TITLE,
    PLAYING,
    TRIVIA,
    FEEDBACK,
    LEVEL_CLEAR,
    GAME_OVER,
    VICTORY
};

struct TriviaState
{
    int   questionIdx    = 0;
    int   selectedAnswer = -1;
    bool  answered       = false;
    bool  correct        = false;
    float feedbackTimer  = 0.f;
};

class GameEngine
{
public:
    Phase                  phase      = Phase::TITLE;
    Player                 player;
    QuizManager            quizMgr;
    std::vector<LevelDef>  levelDefs;
    std::unique_ptr<Level> level;
    int                    levelIdx   = 0;
    float                  camX       = 0.f;
    TriviaState            trivia;
    ParticleSystem         particles;
    float                  titleTimer = 0.f;
    int                    clearTimer = 0;

    GameEngine();

    void startGame();
    void loadLevel(int idx);
    void update(bool left, bool right, bool jumpEdge);
    void submitAnswer(int choice);

    int   totalLevels() const;
    float goalX()       const;

private:
    void moveEntity(Entity& e,
                    int*  outBCol     = nullptr,
                    int*  outBRow     = nullptr,
                    bool* outHitBelow = nullptr);

    void onPlayerHit();
};
