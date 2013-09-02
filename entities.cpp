#include "entities.h"

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

Player::Player(sf::Vector2f pos) : position(pos), jumpHeight(2), jumpTime(2.f), sf::Rect(pos.x, pos.y, 1.f, 2.f)
{
    
}

Player::Player(float px, float py) : position(px, py), jumpHeight(2), jumpTime(2.f), sf::Rect(pos.x, pos.y, 1.f, 2.f)
{
    
}
