
//  Enemy.cpp                         


#include "Enemy.hpp"
#include <cmath>
#include <algorithm>

Enemy::Enemy(float spawnX, float spawnY)
    : Entity(spawnX, spawnY, Config::ENEMY_W, Config::ENEMY_H),
      patrolLeft (std::max(0.f, spawnX - Config::TILE * 4.f)),
      patrolRight(spawnX + Config::TILE * 4.f),
      dir(-1),
      animTimer(0.f)
{}

void Enemy::patrol()
{
    vx = dir * Config::ENEMY_SPD;
    if (x          <= patrolLeft  && dir < 0) dir =  1;
    if (x + w      >= patrolRight && dir > 0) dir = -1;
    facingRight = (dir > 0);
}

void Enemy::draw(sf::RenderTarget& rt, float camX) const
{
    if (!alive) return;
    float wobble = std::sin(animTimer) * 2.5f;
    Renderer::drawEnemy(rt, x - camX, y + wobble);
}
