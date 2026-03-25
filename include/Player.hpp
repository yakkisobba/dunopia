#pragma once

//   Player.hpp                         


#include "Entity.hpp"
#include "Renderer.hpp"

class Player final : public Entity
{
public:
    int   lives;
    int   score;
    int   invTimer;
    float animTimer;

    Player();

    void reset(float spawnX = Config::TILE * 2.f,
               float spawnY = Config::TILE * 5.f);

    bool visible() const;

    void draw(sf::RenderTarget& rt, float camX) const override;
};
