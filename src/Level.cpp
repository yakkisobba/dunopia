// ╔══════════════════════════════════════════════════════════════════╗
// ║                      DUNOPIA — Level.cpp                         ║
// ╚══════════════════════════════════════════════════════════════════╝

#include "Level.hpp"
#include <cmath>
#include <algorithm>

// ══════════════════════════════════════════════════════════════════════
//  Level — constructor
// ══════════════════════════════════════════════════════════════════════
Level::Level(const LevelDef& d) : def(d)
{
    // Spawn enemies: feet rest on top of groundRow tile
    for (const auto& es : d.enemySpawns)
    {
        float ex = static_cast<float>(es.col) * Config::TILE;
        float ey = static_cast<float>(es.groundRow) * Config::TILE
                   - static_cast<float>(Config::ENEMY_H);
        enemies.emplace_back(ex, ey);
    }

    // Scan map for 'Q' tiles and register them with their trivia index
    int qi = 0;
    for (int row = 0; row < Config::LEVEL_ROWS; ++row)
        for (int col = 0; col < Config::LEVEL_COLS; ++col)
            if (rawTileAt(col, row) == 'Q')
            {
                QBlockState qs;
                qs.triviaIndex = (qi < static_cast<int>(d.triviaIndices.size()))
                                 ? d.triviaIndices[qi] : qi % 15;
                qblocks[{col, row}] = qs;
                ++qi;
            }
}

// ── Raw tile (ignores used-Q state) ───────────────────────────────────
char Level::rawTileAt(int col, int row) const
{
    if (col < 0 || col >= Config::LEVEL_COLS) return '#';
    if (row < 0)                               return '.';
    if (row >= Config::LEVEL_ROWS)             return '#';
    if (row >= static_cast<int>(def.mapRows.size())) return '.';
    const auto& r = def.mapRows[row];
    if (col >= static_cast<int>(r.size()))     return '.';
    return r[col];
}

// ── Tile query (used Q shown as 'X', still solid) ─────────────────────
char Level::tileAt(int col, int row) const
{
    char t = rawTileAt(col, row);
    if (t == 'Q')
    {
        auto it = qblocks.find({col, row});
        if (it != qblocks.end() && it->second.used) return 'X';
    }
    return t;
}

// ── Solid test ────────────────────────────────────────────────────────
bool Level::isSolid(char t)
{
    return t == '#' || t == 'B' || t == 'S' || t == 'Q' || t == 'X';
}

// ── Bump a Q block → return trivia index, or -1 ──────────────────────
int Level::tryBumpQ(int col, int row)
{
    auto it = qblocks.find({col, row});
    if (it == qblocks.end()) return -1;
    if (it->second.used)     return -1;
    it->second.used      = true;
    it->second.bumpTimer = 10;
    return it->second.triviaIndex;
}

// ── Per-frame update (bump animations) ────────────────────────────────
void Level::update()
{
    for (auto& [coord, qs] : qblocks)
        if (qs.bumpTimer > 0) --qs.bumpTimer;
}

// ── Background: sky gradient + parallax hills ─────────────────────────
void Level::drawBackground(sf::RenderTarget& rt, float camX) const
{
    int W = Config::SCREEN_W, H = Config::SCREEN_H;
    const auto& sky = def.skyColor;

    for (int i = 0; i < H; i += 4)
    {
        float t = static_cast<float>(i) / H;
        rt.draw(Renderer::rect(0.f, static_cast<float>(i),
            static_cast<float>(W), 5.f,
            sf::Color(
                static_cast<sf::Uint8>(sky[0] * (1.f - t * 0.22f)),
                static_cast<sf::Uint8>(sky[1] * (1.f - t * 0.18f)),
                static_cast<sf::Uint8>(sky[2] * (1.f - t * 0.30f))
            )));
    }

    // Night: draw stars
    if (sky[2] > sky[0] && sky[0] < 50)
    {
        srand(42);
        for (int s = 0; s < 80; ++s)
        {
            float sx = static_cast<float>(rand() % W);
            float sy = static_cast<float>(rand() % (H / 2));
            float br = 160.f + rand() % 95;
            rt.draw(Renderer::rect(sx, sy, 2.f, 2.f,
                sf::Color(static_cast<sf::Uint8>(br),
                          static_cast<sf::Uint8>(br),
                          static_cast<sf::Uint8>(br))));
        }
    }

    // Parallax hills (30 % camera speed)
    float hOff = std::fmod(camX * 0.3f, 260.f);
    for (int m = -1; m < W / 260 + 2; ++m)
    {
        float mx = m * 260.f - hOff;
        float mh = 90.f + std::sin(m * 2.1f + 0.5f) * 32.f;
        sf::ConvexShape hill;
        hill.setPointCount(3);
        hill.setPoint(0, {mx,         static_cast<float>(H)});
        hill.setPoint(1, {mx + 240.f, static_cast<float>(H)});
        hill.setPoint(2, {mx + 120.f, static_cast<float>(H) - mh});
        hill.setFillColor(sf::Color(22, 85, 22, 115));
        rt.draw(hill);
    }
}

// ── Terrain: draw visible tiles ───────────────────────────────────────
void Level::drawTerrain(sf::RenderTarget& rt, float camX) const
{
    int T      = Config::TILE;
    int colMin = std::max(0, static_cast<int>(camX) / T - 1);
    int colMax = std::min(Config::LEVEL_COLS - 1,
                          colMin + Config::SCREEN_W / T + 2);

    for (int row = 0; row < Config::LEVEL_ROWS; ++row)
    {
        for (int col = colMin; col <= colMax; ++col)
        {
            // Use raw tile for rendering (so 'Q' always renders as Q)
            char t  = rawTileAt(col, row);
            if (t == '.') continue;

            float sx = col * T - camX;
            float sy = static_cast<float>(row * T);

            switch (t)
            {
                case '#': Renderer::drawGroundTile(rt, sx, sy); break;
                case 'B': Renderer::drawBambooTile(rt, sx, sy); break;
                case 'S': Renderer::drawStoneTile (rt, sx, sy); break;
                case 'Q': {
                    auto it  = qblocks.find({col, row});
                    bool used   = (it != qblocks.end() && it->second.used);
                    bool bumped = (it != qblocks.end() && it->second.bumpTimer > 0);
                    Renderer::drawQBlockTile(rt, sx, sy, used, bumped);
                    break;
                }
                default: break;
            }
        }
    }
}

// ── Flag ──────────────────────────────────────────────────────────────
void Level::drawFlag(sf::RenderTarget& rt, float camX) const
{
    float fx = def.flagCol * Config::TILE - camX;
    float groundY = static_cast<float>((Config::LEVEL_ROWS - 3) * Config::TILE);
    Renderer::drawFlag(rt, fx, groundY);
}

// ── Flag hit-box ──────────────────────────────────────────────────────
sf::FloatRect Level::flagBounds() const
{
    int T = Config::TILE;
    return {
        static_cast<float>(def.flagCol * T),
        static_cast<float>((Config::LEVEL_ROWS - 10) * T),
        static_cast<float>(T),
        static_cast<float>(T * 9)
    };
}

float Level::worldWidth() const
{
    return static_cast<float>(Config::LEVEL_COLS * Config::TILE);
}

// ══════════════════════════════════════════════════════════════════════
//  LevelFactory::makeLevels()
//
//  MAP RULES (enforced manually):
//   • Every row is padded to exactly LEVEL_COLS (62) characters
//   • Q blocks have open air on the row BELOW them so Rizal can
//     walk underneath and jump up to hit them
//   • Enemies: groundRow is the solid tile they stand ON;
//     they are placed one tile-width to the LEFT of any pit
//   • flagCol column is completely clear (no solid tiles above ground)
//
//  LAYOUT LEGEND:
//   '.' air   '#' ground   'B' bamboo   'S' stone   'Q' question block
// ══════════════════════════════════════════════════════════════════════

// Pad/truncate to exactly LEVEL_COLS chars
static std::string pad(std::string s)
{
    while (static_cast<int>(s.size()) < Config::LEVEL_COLS) s += '.';
    return s.substr(0, Config::LEVEL_COLS);
}

namespace LevelFactory
{

std::vector<LevelDef> makeLevels()
{
    std::vector<LevelDef> levels;

    // ──────────────────────────────────────────────────────────────────
    //  LEVEL 1 — Calamba, Laguna  (Day — Rizal's Birthplace)
    //
    //  Layout:
    //   Row 9  = ground (solid)
    //   Row 8  = gaps in ground for pits
    //   Row 6  = mid-height platforms (S)
    //   Row 4  = Q blocks — AIR on row 5, platforms on row 6
    //   Row 3  = isolated Q — AIR below on rows 4-5
    //   Enemies on row 9 ground between pits
    //   Flag at col 59, ground is solid there
    // ──────────────────────────────────────────────────────────────────
    {
        LevelDef L;
        L.name          = "Calamba, Laguna";
        L.subtitle      = "Jose Rizal's Birthplace — 1861";
        L.skyColor      = {135, 206, 235};
        L.flagCol       = 59;
        L.triviaIndices = {0, 1, 2, 3, 4};

        //              0         1         2         3         4         5         6
        //              0123456789012345678901234567890123456789012345678901234567890 1
       L.mapRows =
{
    pad(".............................................................."), // 0
    pad(".............................................................."), // 1
    pad(".............................................................."), // 2
    pad(".............................................................."), // 3
    pad(".............................................................."), // 4

    // Row 5 — QUESTION BLOCKS (floating)
    pad(".......Q..............Q..............Q......................."), // 5

    // Row 6 — AIR (MANDATORY under Q blocks)
    pad(".............................................................."), // 6

    // Row 7 — JUMP PLATFORMS (2 blocks above ground)
    pad(".....SSSSS........SSS........SSSS........................."), // 7

    // Row 8 — AIR
    pad(".............................................................."), // 8

    // Row 9 — GROUND (TOP)
    pad("##############################################################"), // 9

    // Row 10 — GROUND (BOTTOM)
    pad("##############################################################"), // 10

    pad("##############################################################"), // 11 (extra safety row)
};

        // Enemies: groundRow=9, placed on solid ground sections
        L.enemySpawns =
        {
            {6,  9},   // first ground section
            {22, 9},   // second ground section
            {38, 9},   // third ground section
        };

        levels.push_back(L);
    }

    // ──────────────────────────────────────────────────────────────────
    //  LEVEL 2 — Intramuros, Manila  (Dusk — Walled City)
    //
    //  Layout:
    //   Row 9  = ground with gaps
    //   Row 7  = tall stone wall tops (platforms)
    //   Row 5  = higher stone wall tops
    //   Row 3  = Q blocks — floating in air, reachable by wall-top jump
    //   Row 1  = topmost Q block — reward for highest platform
    //   Enemies on ground (row 9) and wall tops (row 7)
    //   flagCol = 59, fully clear column
    // ──────────────────────────────────────────────────────────────────
    {
        LevelDef L;
        L.name          = "Intramuros, Manila";
        L.subtitle      = "The Walled City — Spanish Colonial Rule";
        L.skyColor      = {230, 120, 55};
        L.flagCol       = 59;
        L.triviaIndices = {5, 6, 7, 8};

        //              0         1         2         3         4         5         6
        //              0123456789012345678901234567890123456789012345678901234567890 1
        L.mapRows =
        {
            pad(".............................................................."),// 0
            pad("..........Q.......................Q............................"),// 1 Q (reachable from row-3 platforms)
            pad(".............................................................."),// 2 AIR under row-1 Q
            pad(".............................................................."),// 3 AIR
            pad(".....SSSS...................SSSS..............................."),// 4 mid platform
            pad(".............................................................."),// 5 AIR
            pad("..Q...............Q..........................................."),// 6 Q blocks (reachable from row-7 platforms)
            pad("..SSS...........SSS...........SSS...........SSS................"),// 7 wall tops (platforms)
            pad("..SSS...........SSS...........SSS...........SSS................"),// 8 wall body
            pad("##....########....########....########....########............"),// 9 ground with gaps
            pad("##############################################################"),//10
            pad("##############################################################"),//11
        };

        // Enemies on ground (row 9) solid sections and wall tops (row 7)
        L.enemySpawns =
        {
            {5,  9},   // ground section 1
            {18, 9},   // ground section 2
            {34, 9},   // ground section 3
        };

        levels.push_back(L);
    }

    // ──────────────────────────────────────────────────────────────────
    //  LEVEL 3 — Bagumbayan (Luneta)  (Night — Rizal's Sacrifice)
    //
    //  Layout:
    //   Row 9  = ground
    //   Row 7  = mid platforms (S)
    //   Row 5  = upper platforms (S)
    //   Row 3  = Q blocks — reached by double-platform jump
    //   Row 1  = highest Q — requires upper platform + jump
    //   More enemies, tighter gaps, night sky with stars
    //   flagCol = 59
    // ──────────────────────────────────────────────────────────────────
    {
        LevelDef L;
        L.name          = "Bagumbayan (Luneta)";
        L.subtitle      = "December 30, 1896 — Rizal's Sacrifice";
        L.skyColor      = {12, 12, 50};
        L.flagCol       = 59;
        L.triviaIndices = {9, 10, 11, 12, 13, 14};

        //              0         1         2         3         4         5         6
        //              0123456789012345678901234567890123456789012345678901234567890 1
        L.mapRows =
        {
            pad(".............................................................."),// 0
            pad("......Q.......................Q................................"),// 1 top Q blocks
            pad(".............................................................."),// 2 AIR under row-1 Q
            pad(".............................................................."),// 3 AIR
            pad(".....SSSS....................SSSS.............................."),// 4 upper platforms
            pad(".............................................................."),// 5 AIR
            pad("..Q.................Q........................................."),// 6 Q blocks (reachable from row-7 platforms)
            pad("..SSS..........SSS..........SSS..........SSS.................."),// 7 mid platforms
            pad(".............................................................."),// 8 AIR (pit gap challenge)
            pad("######....######....######....######....######................"),// 9 ground chunks
            pad("##############################################################"),//10
            pad("##############################################################"),//11
        };

        L.enemySpawns =
        {
            {4,  9},
            {14, 9},
            {24, 9},
            {34, 9},
        };

        levels.push_back(L);
    }

    return levels;
}

} // namespace LevelFactory
