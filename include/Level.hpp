#pragma once

//  Level.hpp                         


#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <utility>
#include "Constants.hpp"
#include "Enemy.hpp"
#include "Renderer.hpp"

// ── Per-tile Q-block runtime state ────────────────────────────────────
struct QBlockState
{
    int  triviaIndex = 0;
    bool used        = false;
    int  bumpTimer   = 0;
};

// ── Level definition (plain data) ────────────────────────────────────
struct EnemySpawn
{
    int col;          // tile column
    int groundRow;    // the solid-tile row they stand ON
};

struct LevelDef
{
    std::string              name;
    std::string              subtitle;
    std::array<sf::Uint8, 3> skyColor;
    int                      flagCol;          // tile column of flag
    std::vector<std::string> mapRows;          // LEVEL_ROWS rows, each LEVEL_COLS chars
    std::vector<EnemySpawn>  enemySpawns;
    std::vector<int>         triviaIndices;    // one per 'Q' in left-to-right top-bottom order
};

// ── Runtime level ─────────────────────────────────────────────────────
class Level
{
public:
    LevelDef             def;
    std::vector<Enemy>   enemies;
    std::map<std::pair<int,int>, QBlockState> qblocks;

    explicit Level(const LevelDef& d);

    char rawTileAt(int col, int row) const;  // ignores used-Q state
    char tileAt(int col, int row) const;     // used Q shown as 'X'
    static bool isSolid(char t);

    // Returns trivia index if Q tile at (col,row) is unbumped; -1 otherwise
    int tryBumpQ(int col, int row);

    void update();   // tick bump animations

    void drawBackground(sf::RenderTarget& rt, float camX) const;
    void drawTerrain(sf::RenderTarget& rt, float camX) const;
    void drawFlag(sf::RenderTarget& rt, float camX) const;

    sf::FloatRect flagBounds() const;
    float worldWidth() const;
};

// ── LevelFactory ──────────────────────────────────────────────────────
namespace LevelFactory
{
    std::vector<LevelDef> makeLevels();
}
