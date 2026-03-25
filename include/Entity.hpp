#pragma once

// Entity.hpp                 

#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Entity
{
public:
    float x, y;
    float vx, vy;
    int   w, h;
    bool  onGround;
    bool  alive;
    bool  facingRight;

    Entity(float x = 0.f, float y = 0.f,
           int   w = Config::TILE,
           int   h = Config::TILE);

    virtual ~Entity() = default;

    sf::FloatRect bounds() const;
    bool overlaps(const Entity& other) const;

    virtual void draw(sf::RenderTarget& rt, float camX) const = 0;
};
