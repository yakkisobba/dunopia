// ╔══════════════════════════════════════════════════════════════════╗
// ║                     DUNOPIA — Player.cpp                         ║
// ╚══════════════════════════════════════════════════════════════════╝

#include "Player.hpp"
#include <cmath>

Player::Player()
    : Entity(Config::TILE * 2.f, Config::TILE * 5.f,
             Config::PLAYER_W, Config::PLAYER_H),
      lives(Config::PLAYER_LIVES), score(0),
      invTimer(0), animTimer(0.f)
{}

void Player::reset(float spawnX, float spawnY)
{
    x = spawnX; y = spawnY;
    vx = vy    = 0.f;
    onGround   = false;
    alive      = true;
    invTimer   = 0;
    animTimer  = 0.f;
}

bool Player::visible() const
{
    return invTimer == 0 || (invTimer / 5) % 2 == 0;
}

void Player::draw(sf::RenderTarget& rt, float camX) const
{
    if (!alive || !visible()) return;
    float wobble = onGround ? std::sin(animTimer) * 1.8f : 0.f;
    Renderer::drawRizal(rt, x - camX, y + wobble);
}
