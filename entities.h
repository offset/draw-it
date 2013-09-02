#ifndef ENTITIES_H
#define ENTITIES_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

/*!
 * \brief The entities class
 *
 * This is a base class for different entities appearing in the game. It only specifies common attributes.
 */
class Entities
{
public:
    Entities();
    void setPosition(sf::Vector2f pos);
    void setPosition(float px, float py);
    sf::Vector2f getPosition() const;
    
private:
    sf::Vector2f position;
};

class Player : public Entities
{
public:
    // construct the player object at a specific position
    Player(sf::Vector2f pos);
    Player(float px, float py);
    void setJumpTime(float time);
    float getJumpTime() const;
    void setJumpHeight(int height);
    int getJumpHeight() const;
    
private:
    // how high is the player able to jump
    int jumpHeight;
    // how long may the player jump
    float jumpTime;
    // maybe used in further implementations with other obstacles than falling down (enemies, etc.)
    sf::Rect hitBox;
};

#endif // ENTITIES_H
