#pragma once

// ╔══════════════════════════════════════════════════════════════════╗
// ║                    DUNOPIA — Particle.hpp                        ║
// ╚══════════════════════════════════════════════════════════════════╝

#include <SFML/Graphics.hpp>
#include <vector>

struct Particle
{
    float     x, y, vx, vy;
    float     life, maxLife;
    sf::Color color;
    float     size;
};

class ParticleSystem
{
public:
    std::vector<Particle> particles;

    void burst(float x, float y, sf::Color color,
               int count = 20, float speed = 3.f,
               float lifeMin = 25.f, float lifeMax = 50.f);

    void update();
    void draw(sf::RenderTarget& rt, float camX = 0.f) const;
    void drawScreen(sf::RenderTarget& rt) const;
    void clear();
};
