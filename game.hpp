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
    
private:
    sf::RenderWindow window;
    Player player;
    sf::VertexArray playerVertex;
    sf::Vector2u playerBottom;
    sf::Vector2u playerUp;
    sf::Vector2u playerLeft;
    sf::Vector2u playerRight;
    // needed for movement (in processEvents)
    bool isMovingLeft;
    bool isMovingRight;
    bool isJumping;
    bool hasBeenJumping;
    // variable for fixed framerate
    const sf::Time timePerFrame;
    sf::View view;
    sf::View miniMap;
};

#endif // GAME_H
