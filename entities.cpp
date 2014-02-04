#include "entities.hpp"

Entities::Entities() : position(sf::Vector2f(0, 0)), velocity(0, -30), acceleration(0, 9.81), boundingBox(0, 0, 5, 5), onGround(false)
{
    
}

Entities::Entities(sf::Vector2f pos) : position(pos), velocity(0, -30), acceleration(0, 9.81), boundingBox(0, 0, 5, 5), onGround(false)
{
    
}

Entities::Entities(float px, float py) : position(px, py), velocity(0, -30), acceleration(0, 9.81), boundingBox(0, 0, 5, 5), onGround(false)
{
    
}

Entities::~Entities()
{
    
}

void Entities::setPosition(sf::Vector2f pos)
{
    position = pos;
}

void Entities::setPosition(float px, float py)
{
    position.x = px;
    position.y = py;
}

sf::Vector2f Entities::getPosition() const
{
    return position;
}

void Entities::move(sf::Vector2f movement)
{
    if(movement.x != 0.f)
    {
        position.x += movement.x;
    }
    if(movement.y != 0.f)
    {
        position.y += movement.y;
    }
}

void Entities::setBoundingBox(sf::Rect<int> newBoundingBox)
{
    boundingBox = newBoundingBox;
}

sf::Rect<int> Entities::getBoundingBox()
{
    return boundingBox;
}

void Entities::setOnGround(bool newOnGround)
{
    onGround = newOnGround;
}

bool Entities::getOnGround()
{
    return onGround;
}

Player::Player() : Entities(), jumpHeight(2), jumpTime(2.f)
{
    
}

Player::Player(sf::Vector2f pos) : Entities(pos), jumpHeight(2), jumpTime(2.f)/*, hitBox(position.x, position.y, 1.f, 2.f)*/
{
    
}

Player::Player(float px, float py) : Entities(px, py), jumpHeight(2), jumpTime(2.f)/*, hitBox(position.x, position.y, 1.f, 2.f)*/
{
    
}

Player::~Player()
{
    
}
