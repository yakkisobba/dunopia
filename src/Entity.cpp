
// Entity.cpp                        


#include "Entity.hpp"

Entity::Entity(float x, float y, int w, int h)
    : x(x), y(y), vx(0.f), vy(0.f), w(w), h(h),
      onGround(false), alive(true), facingRight(true)
{}

sf::FloatRect Entity::bounds() const
{
    return {x, y, static_cast<float>(w), static_cast<float>(h)};
}

bool Entity::overlaps(const Entity& other) const
{
    return bounds().intersects(other.bounds());
}
