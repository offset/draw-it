#include "entities.hpp"

Entities::Entities(sf::Vector2f pos) : position(pos)
{
    
}

Entities::Entities(float px, float py) : position(px, py)
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


Player::Player() : Entities(), jumpHeight(2), jumpTime(2.f)
{
    
}

Player::Player(sf::Vector2f pos) : Entities(pos), jumpHeight(2), jumpTime(2.f)/*, hitBox(position.x, position.y, 1.f, 2.f)*/
{
    
}

Player::Player(float px, float py) : Entities(px, py), jumpHeight(2), jumpTime(2.f)/*, hitBox(position.x, position.y, 1.f, 2.f)*/
{
    
}

void Player::move(sf::Vector2f movement)
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
