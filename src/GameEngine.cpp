// ╔══════════════════════════════════════════════════════════════════╗
// ║                   DUNOPIA — GameEngine.cpp                       ║
// ╚══════════════════════════════════════════════════════════════════╝

#include "GameEngine.hpp"
#include "Palette.hpp"
#include <algorithm>
#include <cmath>

GameEngine::GameEngine()
{
    levelDefs = LevelFactory::makeLevels();
}

void GameEngine::startGame()
{
    player.lives = Config::PLAYER_LIVES;
    player.score = 0;
    loadLevel(0);
    phase = Phase::PLAYING;
}

void GameEngine::loadLevel(int idx)
{
    levelIdx = idx;
    camX     = 0.f;
    particles.clear();
    level    = std::make_unique<Level>(levelDefs[idx]);
    player.reset();
}

int GameEngine::totalLevels() const
{
    return static_cast<int>(levelDefs.size());
}

float GameEngine::goalX() const
{
    return level ? level->flagBounds().left : 1.f;
}

// ── Submit trivia answer ──────────────────────────────────────────────
void GameEngine::submitAnswer(int choice)
{
    if (phase != Phase::TRIVIA) return;

    trivia.selectedAnswer = choice;
    trivia.answered       = true;
    trivia.correct        = quizMgr.checkAnswer(trivia.questionIdx, choice);

    player.score = std::max(0,
        player.score + (trivia.correct ? Config::PTS_CORRECT
                                       : Config::PTS_WRONG));

    trivia.feedbackTimer = static_cast<float>(Config::FEEDBACK_FRAMES);
    phase = Phase::FEEDBACK;

    particles.burst(
        Config::SCREEN_W / 2.f,
        Config::SCREEN_H / 2.f,
        trivia.correct ? Palette::UI_CORRECT : Palette::UI_WRONG,
        45, 4.5f);
}

// ── Main per-frame update ─────────────────────────────────────────────
void GameEngine::update(bool left, bool right, bool jumpEdge)
{
    titleTimer += 1.f / Config::FPS;

    // ── FEEDBACK phase: wait, then resume playing ─────────────────────
    if (phase == Phase::FEEDBACK)
    {
        particles.update();
        trivia.feedbackTimer -= 1.f;
        if (trivia.feedbackTimer <= 0.f)
            phase = Phase::PLAYING;
        return;
    }

    // ── LEVEL_CLEAR phase: show banner, then load next level ──────────
    if (phase == Phase::LEVEL_CLEAR)
    {
        particles.update();
        if (--clearTimer <= 0)
        {
            int next = levelIdx + 1;
            if (next < static_cast<int>(levelDefs.size()))
            {
                loadLevel(next);
                phase = Phase::PLAYING;
            }
            else
            {
                phase = Phase::VICTORY;
            }
        }
        return;
    }

    if (phase != Phase::PLAYING) return;

    // ── Tick Q-block animations ───────────────────────────────────────
    level->update();

    // ── Player input → velocity ───────────────────────────────────────
    player.vx = 0.f;
    if (left)  { player.vx = -Config::MOVE_SPD; player.facingRight = false; }
    if (right) { player.vx =  Config::MOVE_SPD; player.facingRight = true;  }
    if (jumpEdge && player.onGround)
        player.vy = Config::JUMP_VEL;

    // ── Move player (tile physics) ────────────────────────────────────
    int  bCol = 0, bRow = 0;
    bool hitBelow = false;
    moveEntity(player, &bCol, &bRow, &hitBelow);

    if (player.invTimer > 0) --player.invTimer;

    // Fell into pit → die (Level 2 rule)
    constexpr int GROUND_ROW = 10; // bottom solid row in your level
    float deathY = (GROUND_ROW + 1) * Config::TILE;

    if (player.y > deathY)
    {
        onPlayerHit();
        return;
    }

    // ── Q-block head-bump ─────────────────────────────────────────────
    if (hitBelow)
    {
        int qi = level->tryBumpQ(bCol, bRow);
        if (qi >= 0)
        {
            // Particle burst at block position
            particles.burst(
                static_cast<float>(bCol * Config::TILE + Config::TILE / 2),
                static_cast<float>(bRow * Config::TILE),
                Palette::TEXT_GOLD, 18, 2.8f);

            trivia.questionIdx    = qi;
            trivia.selectedAnswer = -1;
            trivia.answered       = false;
            trivia.feedbackTimer  = 0.f;
            phase = Phase::TRIVIA;
            return;
        }
    }

    // ── Enemy AI + collision ──────────────────────────────────────────
    for (auto& en : level->enemies)
    {
        if (!en.alive) continue;
        en.patrol();
        en.animTimer += 0.14f;
        moveEntity(en);

        if (!player.overlaps(en)) continue;

        float pFeet   = player.y + player.h;
        float eCenter = en.y + en.h * 0.5f;

        if (player.vy > 0.f && pFeet < eCenter)
        {
            // Stomp
            en.alive    = false;
            player.vy   = Config::JUMP_VEL * 0.55f;
            player.score += Config::PTS_STOMP;
            particles.burst(en.x + en.w / 2.f,
                            en.y + en.h / 2.f,
                            Palette::ENEMY_ARMOR, 28, 3.5f);
        }
        else if (player.invTimer == 0)
        {
            onPlayerHit();
            return;
        }
    }

    // ── Goal flag ─────────────────────────────────────────────────────
    if (player.bounds().intersects(level->flagBounds()))
    {
        player.score += Config::PTS_FLAG;
        clearTimer    = 160;
        phase         = Phase::LEVEL_CLEAR;
        particles.burst(player.x, player.y,
                        Palette::TEXT_GOLD, 65, 5.f, 40.f, 80.f);
        return;
    }

    // ── Animation + particles ─────────────────────────────────────────
    player.animTimer += std::abs(player.vx) * 0.18f;
    particles.update();

    // ── Camera: smooth follow, clamped to world width ─────────────────
    float target = player.x - Config::SCREEN_W * 0.35f;
    float maxCam = level->worldWidth() - Config::SCREEN_W;
    camX = std::max(0.f, std::min(target, maxCam));
}

// ══════════════════════════════════════════════════════════════════════
//  Tile-map AABB physics
//
//  Handles both horizontal and vertical movement.
//  When entity rises into a solid tile the hit column/row are reported
//  so GameEngine can check if it was a Q block.
// ══════════════════════════════════════════════════════════════════════
void GameEngine::moveEntity(Entity& e,
                             int*  outBCol,
                             int*  outBRow,
                             bool* outHitBelow)
{
    if (outHitBelow) *outHitBelow = false;

    int T = Config::TILE;

    // ── Horizontal ───────────────────────────────────────────────────
    e.x += e.vx;
    if (e.vx != 0.f)
    {
        bool goRight = e.vx > 0.f;
        int  col     = goRight ? static_cast<int>(e.x + e.w) / T
                               : static_cast<int>(e.x)       / T;

        for (int r = static_cast<int>(e.y) / T;
                 r <= static_cast<int>(e.y + e.h - 1) / T; ++r)
        {
            if (Level::isSolid(level->tileAt(col, r)))
            {
                e.x  = goRight ? static_cast<float>(col * T - e.w)
                               : static_cast<float>((col + 1) * T);
                e.vx = 0.f;

                // Enemies reverse at walls
                if (auto* en = dynamic_cast<Enemy*>(&e))
                    en->dir = -en->dir;
                break;
            }
        }
    }

    // ── Vertical ─────────────────────────────────────────────────────
    e.vy       = std::min(e.vy + Config::GRAVITY, Config::MAX_FALL);
    e.y       += e.vy;
    e.onGround = false;

    if (e.vy > 0.f)
    {
        // Falling — land on floor tile
        int row = static_cast<int>(e.y + e.h) / T;
        for (int c = static_cast<int>(e.x)       / T;
                 c <= static_cast<int>(e.x + e.w - 1) / T; ++c)
        {
            if (Level::isSolid(level->tileAt(c, row)))
            {
                e.y        = static_cast<float>(row * T - e.h);
                e.vy       = 0.f;
                e.onGround = true;
                break;
            }
        }
    }
    else if (e.vy < 0.f)
    {
        // Rising — hit ceiling / Q block from below
        int row = static_cast<int>(e.y) / T;
        for (int c = static_cast<int>(e.x)       / T;
                 c <= static_cast<int>(e.x + e.w - 1) / T; ++c)
        {
            if (Level::isSolid(level->tileAt(c, row)))
            {
                e.y  = static_cast<float>((row + 1) * T);
                e.vy = 1.f;   // small downward nudge

                if (outHitBelow) *outHitBelow = true;
                if (outBCol)     *outBCol     = c;
                if (outBRow)     *outBRow     = row;
                break;
            }
        }
    }

    // ── World horizontal bounds ───────────────────────────────────────
    float ww = level->worldWidth();
    if (e.x < 0.f)       { e.x = 0.f;       e.vx = 0.f; }
    if (e.x + e.w > ww)  { e.x = ww - e.w;  e.vx = 0.f; }
}

// ── Handle player hit ─────────────────────────────────────────────────
void GameEngine::onPlayerHit()
{
    particles.burst(player.x + player.w / 2.f,
                    player.y + player.h / 2.f,
                    Palette::HEART_ON, 30, 3.f);

    if (--player.lives <= 0)
    {
        phase = Phase::GAME_OVER;
    }
    else
    {
        player.reset();
        camX            = 0.f;
        player.invTimer = Config::INV_FRAMES;
    }
}
