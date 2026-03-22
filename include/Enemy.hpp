#pragma once

// ╔══════════════════════════════════════════════════════════════════╗
// ║                      DUNOPIA — Enemy.hpp                         ║
// ╚══════════════════════════════════════════════════════════════════╝

#include "Entity.hpp"
#include "Renderer.hpp"

class Enemy final : public Entity
{
public:
    float patrolLeft;
    float patrolRight;
    int   dir;        // +1 right, -1 left
    float animTimer;

    Enemy(float spawnX, float spawnY);

    void patrol();   // sets vx, reverses at bounds

    void draw(sf::RenderTarget& rt, float camX) const override;
};
