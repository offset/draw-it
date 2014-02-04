#ifndef GAME_H
#define GAME_H
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "entities.hpp"

class Game
{
public:
    Game();
    /*!
     * \brief In this function the game action happens.
     *
     * @return Error/Success code.
     */
    int run();
    /*!
     * \brief Accesses Game::window.
     */
    sf::RenderWindow& getRenderWindow();
    
private:
    /*!
     * \brief This function handles the events occuring during the gameplay (keypresses, etc.).
     */
    void processEvents();
    /*!
     * \brief Updates the game logic and the buffer.
     */
    void update(sf::Time deltaTime);
    /*!
     * \brief Draws everything in the buffer to the screen.
     */
    void render();
    
    /*!
     * \brief Handles the player input.
     */
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    
    /*!
     * \brief Calculates the starting position for the player.
     * \return Success/error.
     */
    int getStartingPosition();
    
    bool isValidLocation(float x, float y);
    
    bool isBlocked(float x, float y);
    
    // calculates the corresponding y value of a linear funcion at a specific position
    float calculateLinearFunction(float x, float y, float wantedX, float d = 0);
    // gives us the surrounding tiles' positions as well as their values
    std::vector<std::map<std::string, int> > getSurroundingTiles(sf::Vector2f position);
    
private:
    sf::RenderWindow window;
    Player player;
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    sf::Vector2u playerSize;
    sf::Vector2u levelSize;
    // needed for movement (in processEvents)
    bool isMovingLeft;
    bool isMovingRight;
    bool isMovingUp;
    bool isMovingDown;
    bool jumps;
    bool isJumping;
    // variable for fixed framerate
    const sf::Time timePerFrame;
    sf::View view;
    //float gravity;
    sf::Vector2f velocity;
    sf::Vector2u tileSize;
};

#endif // GAME_H
