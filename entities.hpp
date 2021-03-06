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
    Entities(sf::Vector2f pos);
    Entities(float px, float py);
    virtual ~Entities();
    virtual void setPosition(sf::Vector2f pos);
    virtual void setPosition(float px, float py);
    virtual sf::Vector2f getPosition() const;
    virtual void move(sf::Vector2f movement);
    virtual void setBoundingBox(sf::Rect<int> newBoundingBox);
    virtual sf::Rect<int> getBoundingBox();
    virtual void setOnGround(bool newOnGround);
    virtual bool getOnGround();
    
private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Rect<int> boundingBox;
    bool onGround;
};

/*!
 * \brief The Player class
 */
class Player : public Entities
{
public:
    Player();
    // construct the player object at a specific position
    Player(sf::Vector2f pos);
    Player(float px, float py);
    ~Player();
    void setJumpTime(float time);
    float getJumpTime() const;
    void setJumpHeight(int height);
    int getJumpHeight() const;
    
private:
    // how high is the player able to jump
    int jumpHeight;
    // how long may the player jump
    float jumpTime;
    /*
     * position --------------- rightCorner
     * |                                  |
     * |                                  |
     * leftCorner --------- rightDownCorner
     */
    sf::Vector2f position;
};

#endif // ENTITIES_H
