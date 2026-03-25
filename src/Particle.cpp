
//  Particle.cpp                        

#include "Particle.hpp"
#include "Renderer.hpp"
#include <cstdlib>
#include <cmath>
#include <algorithm>

void ParticleSystem::burst(float x, float y, sf::Color color,
                            int count, float speed,
                            float lifeMin, float lifeMax)
{
    for (int i = 0; i < count; ++i)
    {
        float ang   = (std::rand() % 360) * 3.14159f / 180.f;
        float spd   = speed * (0.4f + (std::rand() % 100) / 100.f);
        Particle p;
        p.x       = x;
        p.y       = y;
        p.vx      = std::cos(ang) * spd;
        p.vy      = std::sin(ang) * spd - 1.8f;
        p.life    = p.maxLife =
            lifeMin + (std::rand() % static_cast<int>(lifeMax - lifeMin + 1));
        p.color   = color;
        p.size    = 3.f + (std::rand() % 7);
        particles.push_back(p);
    }
}

void ParticleSystem::update()
{
    for (auto& p : particles)
    {
        p.x    += p.vx;
        p.y    += p.vy;
        p.vy   += 0.13f;
        p.life -= 1.f;
    }
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p){ return p.life <= 0.f; }),
        particles.end());
}

void ParticleSystem::draw(sf::RenderTarget& rt, float camX) const
{
    for (const auto& p : particles)
    {
        float     a  = p.life / p.maxLife;
        sf::Color c  = p.color;
        c.a          = static_cast<sf::Uint8>(255 * a);
        float     s  = p.size * a;
        rt.draw(Renderer::rect(p.x - camX, p.y, s, s, c));
    }
}

void ParticleSystem::drawScreen(sf::RenderTarget& rt) const
{
    draw(rt, 0.f);
}

void ParticleSystem::clear()
{
    particles.clear();
}
